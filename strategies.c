/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strategies.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llafforg <llafforg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/08 16:18:11 by llafforg          #+#    #+#             */
/*   Updated: 2026/09/08 16:53:36 by llafforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <unistd.h>


t_coder	*other_coder(t_coder *c, t_dongle *d)
{
	if (d->coder_l == c)
		return (d->coder_r);
	return (d->coder_l);
}

void	strategie_fifo(t_coder *c, t_dongle *d)
{
	if (c->id % 2 && !c->nbr_compile)
		usleep(1000);
	pthread_mutex_lock(&d->lock);
	if (!d->user[0])
	{
		d->user[0] = c;
		d->user[1] = other_coder(c, d);
	}
	while (d->user[0] != c)
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
	if (other_coder(c, d) != d->user[0])
		d->user[0] = other_coder(c, d);
	d->user[1] = c;
	pthread_cond_signal(&d->available);
	pthread_mutex_unlock(&d->lock);
	d = c->dongles_next;
	pthread_mutex_lock(&d->lock);
	d->t_cooldown = now_ms() + c->datas->t_dongle_cool;
	d->is_available = 1;
	if (other_coder(c, d) != d->user[0])
		d->user[0] = other_coder(c, d);
	d->user[1] = c;
	pthread_cond_signal(&d->available);
	pthread_mutex_unlock(&d->lock);
}
