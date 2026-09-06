/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llafforg <llafforg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 15:06:14 by llafforg          #+#    #+#             */
/*   Updated: 2026/09/06 15:45:26 by llafforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "codexion.h"

void	ft_stages(t_coder *c)
{
	pthread_mutex_lock(&c->lock);
	print_dgl(c, 'n');
	print_log(c, "compiling");
	c->is_compil = 1;
	c->nbr_compile++;
	pthread_cond_signal(&c->in_compil);
	pthread_mutex_unlock(&c->lock);
	usleep(c->datas->t_compile * 1000);
	pthread_mutex_lock(&c->lock);
	c->is_compil = 0;
	pthread_cond_signal(&c->in_compil);
	pthread_mutex_unlock(&c->lock);
	print_log(c, "debugging");
}

void	*main_thread(void *arg_coder)
{
	t_coder		*c;
	int			brn;

	c = (t_coder *)arg_coder;
	while (c->nbr_compile < c->datas->nbr_compile && !c->datas->end)
	{
		pthread_mutex_lock(&c->prev->lock);
		while (c->prev->is_compil)
		{
			brn = now_ms();
			pthread_cond_wait(&c->prev->in_compil, &c->prev->lock);
			brn = now_ms() - brn;
			if (brn >= c->datas->t_burnout)
			{
				print_log(c, "in\033[1;2m burnout\033[0m");
				pthread_mutex_unlock(&c->prev->lock);
				return (NULL);
			}
		}
		pthread_mutex_unlock(&c->prev->lock);
		ft_stages(c);
	}
	return (NULL);
}

void	thread_init(t_data **datas)
{
	int			i;
	t_coder		*curent_c;

	i = 0;
	curent_c = *((*datas)->coders);
	while (i != (*datas)->coder_nbr && !(*datas)->end)
	{
		pthread_create(&curent_c->thread_id, NULL, main_thread, curent_c);
		i++;
		curent_c = curent_c->next;
	}
	i = 0;
	curent_c = *((*datas)->coders);
	while (i != (*datas)->coder_nbr)
	{
		i++;
		pthread_join(curent_c->thread_id, NULL);
		curent_c = curent_c->next;
	}
}
