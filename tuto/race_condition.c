#include <pthread.h>
#include <stdio.h>

/*
 * Etape 4 : le probleme. Une ressource partagee sans protection.
 *
 * A retenir :
 *   - counter++ n'est PAS atomique : c'est lire, ajouter, ecrire.
 *     Deux threads qui s'entrelacent perdent des increments.
 *   - Le resultat obtenu est inferieur a l'attendu, et varie a
 *     chaque execution. C'est exactement ce que le mutex corrige
 *     a l'etape suivante.
 */

#define N 8
#define ITER 2000000

typedef struct s_shared
{
	long	counter;
}	t_shared;

void	*increment(void *arg)
{
	t_shared	*shared;
	int			i;

	shared = (t_shared *)arg;
	i = 0;
	while (i < ITER)
	{
		shared->counter++;
		i++;
	}
	return (NULL);
}

int	main(void)
{
	pthread_t	tids[N];
	t_shared	shared;
	int			i;

	shared.counter = 0;
	i = 0;
	while (i < N)
		pthread_create(&tids[i++], NULL, increment, &shared);
	i = 0;
	while (i < N)
		pthread_join(tids[i++], NULL);
	printf("attendu : %d\n", N * ITER);
	printf("obtenu  : %ld\n", shared.counter);
	return (0);
}