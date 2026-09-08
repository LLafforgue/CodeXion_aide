/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strategies.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llafforg <llafforg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/08 16:18:11 by llafforg          #+#    #+#             */
/*   Updated: 2026/09/08 18:52:12 by llafforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <unistd.h>

t_coder	*ft_fifo_edf_first_coder(t_coder	*first_c, t_dongle *d)
{
	t_coder	*other_c;

	if (first_c->datas->scheduler)
		return (first_c);
	other_c = other_coder(first_c, d);
	pthread_mutex_lock(&first_c->lock);
	pthread_mutex_lock(&other_c->lock);
	if (first_c->t_burnout < other_c->t_burnout)
	{
		pthread_mutex_unlock(&first_c->lock);
		pthread_mutex_unlock(&other_c->lock);
		return (first_c);
	}
	pthread_mutex_unlock(&first_c->lock);
	pthread_mutex_unlock(&other_c->lock);
	return (other_c);
}

void	strategie(t_coder *c, t_dongle *d)
{
	if (c->id % 2 && !c->nbr_compile)
		usleep(1000);
	pthread_mutex_lock(&d->lock);
	if (!d->user[0])
	{
		d->user[0] = ft_fifo_edf_first_coder(c, d);
		d->user[1] = other_coder(c, d);
	}
	while ((d->user[0] != c) & !c->datas->end)
		pthread_cond_wait(&d->available, &d->lock);
	pthread_mutex_unlock(&d->lock);
}

void	let_dongles(t_coder *c)
{
	t_dongle	*d;

	d = c->dongles_prev;
	pthread_mutex_lock(&d->lock);
	d->is_available = 1;
	d->t_cooldown = now_ms() + c->datas->t_dongle_cool;
	if ((other_coder(c, d) != d->user[0]) && c->datas->scheduler)
		d->user[0] = other_coder(c, d);
	else
		d->user[0] = ft_fifo_edf_first_coder(c, d);
	d->user[1] = other_coder(ft_fifo_edf_first_coder(c, d), d);
	pthread_cond_signal(&d->available);
	pthread_mutex_unlock(&d->lock);
	d = c->dongles_next;
	pthread_mutex_lock(&d->lock);
	d->t_cooldown = now_ms() + c->datas->t_dongle_cool;
	d->is_available = 1;
	if ((other_coder(c, d) != d->user[0]) && c->datas->scheduler)
		d->user[0] = other_coder(c, d);
	else
		d->user[0] = ft_fifo_edf_first_coder(c, d);
	d->user[1] = other_coder(ft_fifo_edf_first_coder(c, d), d);
	pthread_cond_signal(&d->available);
	pthread_mutex_unlock(&d->lock);
}
