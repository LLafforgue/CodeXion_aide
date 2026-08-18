#include <pthread.h>
#include <stdio.h>

/*
 * Etape 3 : lancer plusieurs threads dans une boucle.
 *
 * A retenir :
 *   - Chaque thread doit recevoir SA propre case memoire stable
 *     (ici ids[i]). Passer &i ferait lire a tous les threads la
 *     meme variable, modifiee par la boucle => piege classique.
 *   - L'ordre d'execution n'est pas deterministe : relance le
 *     programme plusieurs fois, les lignes sortent dans un ordre
 *     different a chaque fois.
 */

#define N 4

void	*worker(void *arg)
{
	int	id;

	id = *(int *)arg;
	printf("  [thread] je suis le worker numero %d\n", id);
	return (NULL);
}

int	main(void)
{
	pthread_t	tids[N];
	int			ids[N];
	int			i;

	i = 0;
	while (i < N)
	{
		ids[i] = i;
		pthread_create(&tids[i], NULL, worker, &ids[i]);
		i++;
	}
	i = 0;
	while (i < N)
		pthread_join(tids[i++], NULL);
	return (0);
}