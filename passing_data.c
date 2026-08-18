#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

/*
 * Etape 2 : faire entrer une donnee, faire sortir un resultat.
 *
 * A retenir :
 *   - L'argument passe a la routine est un void* : on transtype
 *     dans les deux sens (ici un pointeur vers une struct).
 *   - La valeur retournee par la routine (un void*) se recupere
 *     via le second parametre de pthread_join.
 *   - Question de propriete : le thread alloue, main() libere.
 */

typedef struct s_task
{
	int	id;
	int	base;
}	t_task;

void	*compute(void *arg)
{
	t_task	*task;
	long	*result;
	int		exit;

	exit = 0;
	task = (t_task *)arg;
	result = malloc(sizeof(long));
	if (!result)
		return (NULL);
	while (task->base < 10 && exit < 10)
	{
		*result = (long)task->base * task->base;
		exit += 1;
		printf("  [thread %d] calcul de %d au carre\n", task->id, task->base);
	}
	return (result);
}

void	*add(void *arg)
{
	t_task	*task;

	task = (t_task *)arg;
	while (task->base < 10)
	{
		task->base = task->base + 1;
		printf("  \033[31m[thread %d]\033[0m calcul de la base plus 1 : %d\n", task->id, task->base);
	}
	return (NULL);
}

int	main(void)
{
	pthread_t	tid;
	pthread_t	tid2;
	t_task		task;
	void		*ret;

	task.id = 1;
	task.base = 7;
	pthread_create(&tid, NULL, compute, &task);
	pthread_create(&tid2, NULL, add, &task);
	pthread_join(tid, &ret);
	if (ret)
	{
		printf("[main]   resultat recu : %ld\n", *(long *)ret);
		free(ret);
	}
}