#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

typedef struct s_data	t_data;

typedef struct s_coder
{
	int				id;
	pthread_t		thread_id;      /* ton choix : le handle vit dans le coder */
	t_data			*data;          /* AJOUT : pointeur retour vers le partage */
	struct s_coder	*next;
}	t_coder;

struct s_data
{
	int		coder_nbr;
	long	start;                  /* ton current_time : instant 0 */
	t_coder	*coders;              /* tete de l'anneau */
};

long	now_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

/* signature imposee : void *(void *) */
void	*routine(void *arg)
{
	t_coder	*coder;

	coder = (t_coder *)arg;
	printf("%ld  coder %d  thread cree\n",
		now_ms() - coder->data->start, coder->id);
	return (NULL);
}

/* le manager : creer TOUS, puis joindre TOUS */
void	run_threads(t_data *data)
{
	t_coder	*cur;
	int		i;

	data->start = now_ms();               /* on fixe l'instant 0 */
	cur = data->coders;
	i = 0;
	while (i < data->coder_nbr)            /* phase 1 : lancer */
	{
		pthread_create(&cur->thread_id, NULL, routine, cur);
		cur = cur->next;
		i++;
	}
	cur = data->coders;
	i = 0;
	while (i < data->coder_nbr)            /* phase 2 : attendre */
	{
		pthread_join(cur->thread_id, NULL);
		cur = cur->next;
		i++;
	}
}

/* --- petit harnais : anneau de n coders, chacun relie a data --- */
t_coder	*build_ring(t_data *data, int n)
{
	t_coder	*head;
	t_coder	*cur;
	t_coder	*node;
	int		i;

	head = malloc(sizeof(t_coder));
	head->id = 0;
	head->data = data;
	cur = head;
	i = 1;
	while (i < n)
	{
		node = malloc(sizeof(t_coder));
		node->id = i;
		node->data = data;
		cur->next = node;
		cur = node;
		i++;
	}
	cur->next = head;
	return (head);
}

int	main(void)
{
	t_data	data;

	data.coder_nbr = 4;
	data.coders = build_ring(&data, data.coder_nbr);
	run_threads(&data);
	return (0);
}
