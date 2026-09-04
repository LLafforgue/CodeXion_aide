/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llafforg <llafforg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 15:06:14 by llafforg          #+#    #+#             */
/*   Updated: 2026/09/03 19:38:07 by llafforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "codexion.h"

// voit	ft_codexion(t_data **datas)
// {
// 	pthread_t	*thds;

// 	thds = malloc((*datas)->coder_nbr * sizeof(pthread_t));
// }

int	try_take(t_dongle *d)
{
	int	got;

	pthread_mutex_lock(&d->lock);
	if (d->is_available)
	{
		d->is_available = 0;
		got = 1;
		// usleep(500000);
	}
	else
		got = 0;
	pthread_mutex_unlock(&d->lock);
	return (got);
}

void	give_back(t_dongle *d)
{
	pthread_mutex_lock(&d->lock);
	d->is_available = 1;
	usleep(1000000);
	pthread_mutex_unlock(&d->lock);
}

void	*print_valid(void *arg_coder)
{
	t_coder	*c;
	int		id;
	// long	t;

	c = (t_coder *)arg_coder;
	id = c->id;
	if (try_take(c->dongles_prev))
	{
		if (try_take(c->dongles_next))
			printf("coder %d a pris les dongles %d et %d\n",
				id, c->dongles_prev->id, c->dongles_next->id);
		else
		{
			give_back(c->dongles_prev);
			printf("\033[31mcoder %d : dongle %d indisponible\033[0m\n",
				id, c->dongles_next->id);
		}
	}
	else
		printf("\033[31mcoder %d : dongle %d indisponible\033[0m\n",
			id, c->dongles_prev->id);
	// t = now_ms() - c->datas->start_time;
	// printf("at \033[32m%ld ms\033[0m cree\n", t);
	return (NULL);
}

void	ft_thread_init(t_data **datas)
{
	int			i;
	t_coder		*curent_c;

	i = 0;
	curent_c = *((*datas)->coders);
	while (i != (*datas)->coder_nbr)
	{
		pthread_create(&curent_c->thread_id, NULL, print_valid, curent_c);
		i++;
		curent_c = curent_c->next;
	}
	i = 0;

	while (i != (*datas)->coder_nbr)
	{
		i++;
		pthread_join(curent_c->thread_id, NULL);
		curent_c = curent_c->next;
	}
}
