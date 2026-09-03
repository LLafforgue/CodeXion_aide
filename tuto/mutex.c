#include <pthread.h>
#include <stdio.h>

/*
 * Etape 5 : la solution. Un mutex serialise la section critique.
 *
 * A retenir :
 *   - pthread_mutex_init avant tout usage, pthread_mutex_destroy apres.
 *   - lock/unlock encadrent EXACTEMENT l'acces partage, ni plus ni
 *     moins : garder la section critique la plus courte possible.
 *   - Le resultat est desormais exact a chaque execution.
 */

#define N 8
#define ITER 2000000

typedef struct s_shared
{
	long			counter;
	pthread_mutex_t	lock;
}	t_shared;

void	*increment(void *arg)
{
	t_shared	*shared;
	int			i;

	shared = (t_shared *)arg;
	i = 0;
	while (i < ITER)
	{
		pthread_mutex_lock(&shared->lock);
		shared->counter++;
		pthread_mutex_unlock(&shared->lock);
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
	pthread_mutex_init(&shared.lock, NULL);
	i = 0;
	while (i < N)
		pthread_create(&tids[i++], NULL, increment, &shared);
	i = 0;
	while (i < N)
		pthread_join(tids[i++], NULL);
	pthread_mutex_destroy(&shared.lock);
	printf("attendu : %d\n", N * ITER);
	printf("obtenu  : %ld\n", shared.counter);
	return (0);
}
