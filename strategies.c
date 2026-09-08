/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strategies.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llafforg <llafforg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/08 16:18:11 by llafforg          #+#    #+#             */
/*   Updated: 2026/09/08 21:52:12 by llafforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <unistd.h>

static t_coder	*ft_edf_first_coder(t_coder	*first_c, t_dongle *d)
{
	t_coder	*other_c;

	if (!first_c->nbr_compile)
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

void	strategie_edf(t_coder *c, t_dongle *d)
{
	if ((1 - c->id % 2) && !c->nbr_compile)
		usleep(1000);
	pthread_mutex_lock(&d->lock);
	pthread_mutex_lock(&c->datas->lock);
	if (c->datas->coder_nbr < 2 || c->datas->end)
	{
		pthread_mutex_unlock(&d->lock);
		pthread_mutex_unlock(&c->datas->lock);
		return ;
	}
	pthread_mutex_unlock(&c->datas->lock);
	if (!d->user[0])
	{
		d->user[0] = ft_edf_first_coder(c, d);
		d->user[1] = other_coder(ft_edf_first_coder(c, d), d);
	}
	while ((d->user[0] != c) && !c->datas->end)
		pthread_cond_wait(&d->available, &d->lock);
	pthread_mutex_unlock(&d->lock);
}

void	strategie_fifo(t_coder *c, t_dongle *d)
{
	pthread_mutex_lock(&d->lock);
	if (c->datas->coder_nbr < 2)
	{
		pthread_mutex_unlock(&d->lock);
		return ;
	}
	if (!d->user[0])
	{
		d->user[0] = c;
		d->user[1] = other_coder(c, d);
	}
	while (d->user[0] != c && !c->datas->end)
		pthread_cond_wait(&d->available, &d->lock);
	pthread_mutex_unlock(&d->lock);
}

static void	ft_let_one_dongle(t_coder *c, t_dongle *d)
{
	pthread_mutex_lock(&d->lock);
	d->is_available = 1;
	d->t_cooldown = now_ms() + c->datas->t_dongle_cool;
	if (other_coder(c, d) != d->user[0])
		d->user[0] = other_coder(c, d);
	d->user[1] = c;
	pthread_cond_broadcast(&d->available);
	pthread_mutex_unlock(&d->lock);
}

void	let_dongles(t_coder *c)
{
	if (c->dongles_took)
	{
		ft_let_one_dongle(c, c->dongles_prev);
		ft_let_one_dongle(c, c->dongles_next);
		pthread_mutex_lock(&c->lock);
		c->dongles_took = 0;
		pthread_mutex_unlock(&c->lock);
	}
}
