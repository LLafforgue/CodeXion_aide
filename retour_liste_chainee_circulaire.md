# Retour sur le code de création de la liste chaînée circulaire

## 1. Le principe général

Le principe est valide : le pointeur `coders` n'a pas besoin d'être alloué avec `malloc`.

Il faut distinguer :

- le **pointeur** qui contient l'adresse du premier nœud ;
- les **nœuds** eux-mêmes, qui peuvent être alloués dynamiquement avec `malloc`.

Avec :

```c
t_coder *coders;
```

`coders` peut être une variable locale, tandis que les `t_coder` sont dans le heap.

Schématiquement :

```text
STACK                         HEAP

coders ───────────────────→ [ coder 0 ]
                              next ─────→ [ coder 1 ]
                                            next ───→ [ coder 2 ]
                                                         next ──┐
                                                                  │
                              ←───────────────────────────────────┘
```

Il n'est donc pas nécessaire de faire un `malloc` pour `coders` lui-même.

---

## 2. Ce que fait actuellement `init_coders`

Le code actuel :

```c
void	init_coders(t_data	**data, t_coder **coders)
{
	int		nbr;
	t_coder	*temp;
	t_coder	*new_coder;

	nbr = 0;
	while (nbr < (*data)->coder_nbr)
	{
		if (nbr == 0)
		{
			*coders = create_coder(*data, nbr);
			temp = *coders;
		}
		else
		{
			new_coder = create_coder(*data, nbr);
			new_coder->prev = *coders;
			(*coders)->next = new_coder;
			*coders = new_coder;
		}
		nbr++;
	}
	*coders = temp;
}
```

construit une **liste doublement chaînée**, mais elle n'est pas encore circulaire.

Avec trois coders, on obtient conceptuellement :

```text
[ coder 0 ] ⇄ [ coder 1 ] ⇄ [ coder 2 ]
```

Puis :

```c
*coders = temp;
```

fait simplement revenir `coders` sur le premier coder :

```text
coders
  │
  ▼
[0] ⇄ [1] ⇄ [2]
```

Mais pour obtenir une véritable liste circulaire, il faut également relier le dernier et le premier nœud :

```text
[0].prev → [2]
[2].next → [0]
```

Ce qui donne :

```text
          ┌─────────────────────┐
          │                     ▼
        [0] ⇄ [1] ⇄ [2]
          ▲                     │
          └─────────────────────┘
```

---

## 3. Construction de la liste circulaire

Une version correspondant à la logique actuelle est :

```c
void	init_coders(t_data *data, t_coder **coders)
{
	int		nbr;
	t_coder	*temp;
	t_coder	*new_coder;

	nbr = 0;
	while (nbr < data->coder_nbr)
	{
		if (nbr == 0)
		{
			*coders = create_coder(data, nbr);
			temp = *coders;
		}
		else
		{
			new_coder = create_coder(data, nbr);
			new_coder->prev = *coders;
			(*coders)->next = new_coder;
			*coders = new_coder;
		}
		nbr++;
	}

	(*coders)->next = temp;
	temp->prev = *coders;
	*coders = temp;
}
```

À la fin :

```text
                 coders
                   │
                   ▼
              ┌─────────┐
              │ coder 0 │
              └────┬────┘
                   │
                   ▼
              ┌─────────┐
         ┌───►│ coder 1 │
         │    └────┬────┘
         │         │
         │         ▼
         │    ┌─────────┐
         └────│ coder 2 │
              └────┬────┘
                   │
                   └──────────► coder 0
```

Les liens `prev` sont également circulaires :

```text
0.prev → 2
1.prev → 0
2.prev → 1
```

---

## 4. Problème de fuite mémoire dans `create_coder`

Le code initial contient :

```c
coder = malloc(sizeof(t_coder));
if (!coder)
	return (coder = NULL);
```

puis :

```c
coder->dongles = malloc(sizeof(t_dongle) * 2);
if (!coder->dongles)
	return (NULL);
```

Si le deuxième `malloc` échoue, l'adresse de `coder` est perdue sans avoir été libérée.

Cela provoque une **fuite mémoire**.

Il faut libérer `coder` avant de retourner :

```c
if (!coder->dongles)
{
	free(coder);
	return (NULL);
}
```

Il est également inutile d'écrire :

```c
return (coder = NULL);
```

Un simple :

```c
return (NULL);
```

suffit.

---

## 5. Initialiser `next` et `prev`

Il est important d'initialiser les champs `next` et `prev`.

Sinon, avant qu'ils soient correctement configurés par `init_coders`, ils contiennent des valeurs indéterminées.

Dans `create_coder` :

```c
coder->prev = NULL;
coder->next = NULL;
```

Une version améliorée serait donc :

```c
t_coder	*create_coder(t_data *data, int nbr)
{
	t_coder	*coder;

	coder = malloc(sizeof(t_coder));
	if (!coder)
		return (NULL);

	coder->id = nbr;
	coder->nbr_compile = 0;
	coder->stage = 0;
	coder->t_burnout = data->t_burnout;
	coder->prev = NULL;
	coder->next = NULL;

	coder->dongles = malloc(sizeof(t_dongle) * 2);
	if (!coder->dongles)
	{
		free(coder);
		return (NULL);
	}

	return (coder);
}
```

---

## 6. Le pointeur `dongle` dans `main`

Le code initial contient :

```c
t_dongle	*dongle;
```

mais ce pointeur n'est pas utilisé.

Il peut donc être supprimé pour le moment :

```c
int	main(int argc, char **argv)
{
	t_data	*data;
	t_coder	*coders;

	coders = NULL;
	if (arg_nbr_error(argc) || arg_value_error(argv))
		return (1);
	data = init_data(argv);
	init_coders(data, &coders);
	return (0);
}
```

---

## 7. `t_data **data` n'est probablement pas nécessaire

La fonction initiale utilise :

```c
void	init_coders(t_data **data, t_coder **coders)
```

et est appelée avec :

```c
init_coders(&data, &coders);
```

Mais dans `init_coders`, le pointeur `data` lui-même n'est jamais modifié.

On utilise seulement les données auxquelles il pointe :

```c
(*data)->coder_nbr
```

Il est donc plus logique de passer simplement :

```c
t_data *data
```

La signature devient :

```c
void	init_coders(t_data *data, t_coder **coders)
```

L'appel devient :

```c
init_coders(data, &coders);
```

Et dans la boucle :

```c
while (nbr < data->coder_nbr)
```

### Pourquoi `t_coder **coders` est-il en revanche nécessaire ?

Parce que `init_coders` doit modifier la valeur du pointeur `coders` qui se trouve dans `main`.

Dans `main` :

```text
data
 │
 └────────────────→ [ t_data ]

coders
 │
 └────────────────→ [ coder 0 ] ⇄ [ coder 1 ] ⇄ [ coder 2 ]
```

La fonction doit pouvoir faire évoluer `coders`. Elle reçoit donc son adresse :

```c
init_coders(data, &coders);
```

C'est pourquoi :

- `data` → `t_data *` suffit ;
- `coders` → `t_coder **` est justifié.

---

## 8. Architecture mémoire finale

Avec trois coders, l'organisation peut être vue ainsi :

```text
STACK

data ─────────────────────────→ [ t_data ]

coders ────────────────────────→ [ coder 0 ]
                                  │
                                  │ next
                                  ▼
                               [ coder 1 ]
                                  │
                                  │ next
                                  ▼
                               [ coder 2 ]
                                  │
                                  │ next
                                  └──────────→ [ coder 0 ]

prev :

coder 0.prev ───────────────────────────────→ coder 2
coder 1.prev ───────────────────────────────→ coder 0
coder 2.prev ───────────────────────────────→ coder 1
```

Les `t_coder` sont dans le **heap** car ils ont été créés avec `malloc`.

En revanche, le pointeur `coders` lui-même peut rester une variable locale dans `main`, donc sur la **stack**.

---

## 9. Conclusion

La conception de départ est bonne.

Les points principaux à retenir sont :

1. **Pas besoin de `malloc` pour `coders` lui-même.**
2. Les `t_coder` sont alloués avec `malloc`.
3. La liste actuelle est doublement chaînée, mais il faut ajouter les deux liens entre le premier et le dernier nœud pour la rendre circulaire.
4. Il faut `free(coder)` si l'allocation de `coder->dongles` échoue.
5. Il est préférable d'initialiser `next` et `prev` à `NULL` dans `create_coder`.
6. `t_data **data` peut être simplifié en `t_data *data` puisque `data` n'est pas modifié.
7. `t_coder **coders` est nécessaire parce que `init_coders` doit modifier le pointeur `coders` du `main`.
8. Le pointeur `dongle` inutilisé dans `main` peut être supprimé pour le moment.

La distinction essentielle est donc :

```text
malloc
  │
  ├── t_coder        → OUI
  ├── t_dongle[2]    → OUI
  │
  └── coders         → NON
```

`coders` est simplement une variable qui contient l'adresse du premier élément de ta structure dynamique.
