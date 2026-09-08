/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llafforg <llafforg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 15:06:14 by llafforg          #+#    #+#             */
/*   Updated: 2026/09/08 21:07:47 by llafforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <unistd.h>

static void	ft_check_end(t_coder *c, int *compil_ends)
{
	t_data	*datas;

	datas = c->datas;
	pthread_mutex_lock(&c->lock);
	if (c->nbr_compile >= datas->nbr_compile && !c->max_reach)
	{
		(*compil_ends)++;
		c->max_reach = 1;
	}
	if ((now_ms() - c->t_burnout >= datas->t_burnout
			&& datas->t_burnout))
		toggle_end(c, 'b');
	pthread_mutex_unlock(&c->lock);
	if (*compil_ends >= datas->coder_nbr)
		toggle_end(c, 'c');
}

static void	*ft_watcher(void *arg)
{
	t_data	*datas;
	t_coder	*curent;
	int		compil_ends;

	datas = (t_data *)arg;
	curent = *datas->coders;
	compil_ends = 0;
	while (1)
	{
		ft_check_end(curent, &compil_ends);
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

void	*ft_coder_thread(void *arg_coder)
{
	t_coder		*c;

	while (1)
	{
		c = (t_coder *)arg_coder;
		pthread_mutex_lock(&c->datas->lock);
		if (c->datas->end)
		{
			pthread_mutex_unlock(&c->datas->lock);
			return (NULL);
		}
		pthread_mutex_unlock(&c->datas->lock);
		take_dongles(c);
		if (compilation(c) && c->datas->coder_nbr > 1)
		{
			debugging(c);
			refactoring(c);
		}
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
		pthread_create(&curent_c->thread_id, NULL, ft_coder_thread, curent_c);
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
