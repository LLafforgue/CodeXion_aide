/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llafforg <llafforg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 15:06:14 by llafforg          #+#    #+#             */
/*   Updated: 2026/09/08 15:28:29 by llafforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include "unistd.h"

void	ft_stages(t_coder *c)
{
	take_dongles(c);
	if (!compilation(c))
		let_dongles(c);
	debugging(c);
	refactoring(c);
}

void	*ft_watcher(void *arg)
{
	t_data	*datas;
	t_coder	*curent;
	int		compil_ends;

	datas = (t_data *)arg;
	curent = *datas->coders;
	compil_ends = 0;
	while (1)
	{
		pthread_mutex_lock(&curent->lock);
		if (curent->nbr_compile == datas->nbr_compile)
			compil_ends++;
		if (now_ms() - curent->t_burnout >= datas->t_burnout)
			toggle_end(curent, 'b');
		pthread_mutex_unlock(&curent->lock);
		if (compil_ends >= datas->coder_nbr)
			toggle_end(curent, 'c');
		curent = curent->next;
		usleep(1000);
		pthread_mutex_lock(&datas->lock);
		if (datas->end)
		{
			pthread_mutex_unlock(&datas->lock);
			return (NULL);
		}
		pthread_mutex_unlock(&datas->lock);
	}
}

void	*main_thread(void *arg_coder)
{
	t_coder		*c;

	c = (t_coder *)arg_coder;
	while (1)
	{
		pthread_mutex_lock(&c->datas->lock);
		if (c->datas->end)
		{
			pthread_mutex_unlock(&c->datas->lock);
			return (NULL);
		}
		pthread_mutex_unlock(&c->datas->lock);
		ft_stages(c);
	}
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
