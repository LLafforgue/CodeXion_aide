/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llafforg <llafforg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 15:06:14 by llafforg          #+#    #+#             */
/*   Updated: 2026/09/06 17:05:22 by llafforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	ft_stages(t_coder *c)
{
	pthread_mutex_lock(&c->lock);
	print_dgl(c, 'n');
	compilation(c);
	debugging(c);
}

void	*main_thread(void *arg_coder)
{
	t_coder		*c;
	int			brn;

	c = (t_coder *)arg_coder;
	while (c->nbr_compile < c->datas->nbr_compile && !c->datas->end)
	{
		pthread_mutex_lock(&c->prev->lock);
		while (c->prev->is_compil && !c->datas->end)
		{
			brn = now_ms();
			pthread_cond_wait(&c->prev->in_compil, &c->prev->lock);
			brn = now_ms() - brn;
			if (brn >= c->datas->t_burnout)
			{
				pthread_mutex_unlock(&c->prev->lock);
				if (!c->datas->end)
					toggle_end(c, 'b');
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
