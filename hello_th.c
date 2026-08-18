#include <pthread.h>
#include <stdio.h>

/*
 * Etape 1 : creer un thread, l'attendre.
 *
 * A retenir :
 *   - main() est deja un thread (le thread principal).
 *   - pthread_create lance un second flot d'execution qui demarre
 *     dans la fonction "routine".
 *   - pthread_join bloque main() jusqu'a ce que le thread se termine.
 *     Sans join, main() pourrait finir avant, et le thread serait tue.
 */

void	*routine(void *arg)
{
	int	*test;

	test = (int *)arg;
	printf("  [thread] bonjour depuis le thread secondaire\n");
	while (*test < 20)
	{
		printf("\033[1m%d\033[0m\n", *test);
		*test += 1;
	}
	return (NULL);
}

int	main(void)
{
	pthread_t	tid;
	pthread_t	tid2;
	int			counter;

	counter = 0;
	printf("[main]   avant la creation du thread\n");
	pthread_create(&tid, NULL, routine, &counter);
	pthread_create(&tid2, NULL, routine, &counter);
	pthread_join(tid, NULL);
	pthread_join(tid2, NULL);
	printf("[main]   thread termine, fin du programme\n");
	return (0);
}
