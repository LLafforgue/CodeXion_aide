/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llafforg <llafforg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 15:06:14 by llafforg          #+#    #+#             */
/*   Updated: 2026/09/06 18:32:18 by llafforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	ft_stages(t_coder *c)
{
	print_dgl(c, 'n');
	compilation(c);
	debugging(c);
}

void	*ft_watcher(void *arg)
{
	t_data	*datas;
	t_coder	*curent;
	long	time_past;

	datas = (t_data *)arg;
	curent = *datas->coders;
	while (!datas->end)
	{
		pthread_mutex_lock(&curent->lock);
		time_past = now_ms() - curent->t_burnout;
		pthread_mutex_unlock(&curent->lock);
		if (time_past >= datas->t_burnout)
			toggle_end(curent, 'b');
		curent = curent->next;
	}
	pthread_mutex_unlock(&datas->lock);
	return (NULL);
}

void	*main_thread(void *arg_coder)
{
	t_coder		*c;

	c = (t_coder *)arg_coder;
	while (c->nbr_compile < c->datas->nbr_compile && !c->datas->end)
	{
		pthread_mutex_lock(&c->prev->lock);
		while (c->prev->is_compil && !c->datas->end)
			pthread_cond_wait(&c->prev->in_compil, &c->prev->lock);
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
	pthread_create(&(*datas)->burnout_watcher, NULL, ft_watcher, (*datas));
	i = 0;
	pthread_join((*datas)->burnout_watcher, NULL);
	while (i != (*datas)->coder_nbr)
	{
		i++;
		pthread_join(curent_c->thread_id, NULL);
		curent_c = curent_c->next;
	}
}
