/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strategies.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llafforg <llafforg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/08 16:18:11 by llafforg          #+#    #+#             */
/*   Updated: 2026/09/15 18:58:55 by llafforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <unistd.h>

t_coder	*edf_first_coder(t_coder *first_c, t_dongle *d)
{
	t_coder	*other_c;

	if (!first_c->nbr_compile)
		return (first_c);
	other_c = other_coder(first_c, d);
	pthread_mutex_lock(&other_c->l_burnout);
	if (first_c->t_burnout < other_c->t_burnout)
	{
		pthread_mutex_unlock(&other_c->l_burnout);
		return (first_c);
	}
	pthread_mutex_unlock(&other_c->l_burnout);
	return (other_c);
}

void	strategie_edf(t_coder *c, t_dongle *d)
{
	pthread_mutex_lock(&c->datas->lock);
	if (c->datas->coder_nbr < 2 || d->user[0] == c || c->datas->end)
	{
		pthread_mutex_unlock(&c->datas->lock);
		return ;
	}
	pthread_mutex_unlock(&c->datas->lock);
	pthread_mutex_lock(&d->l_able);
	if (!d->user[0])
	{
		pthread_mutex_unlock(&d->l_able);
		d->user[0] = edf_first_coder(c, d);
		if (d->user[0] == c)
			return ;
	}
	pthread_mutex_unlock(&d->l_able);
	waiting_dongle_available(c, d);
}

void	strategie_fifo(t_coder *c, t_dongle *d)
{
	if (c->datas->coder_nbr < 2 || d->user[0] == c || c->datas->end)
		return ;
	pthread_mutex_lock(&d->lock);
	if (!d->user[0])
	{
		d->user[0] = c;
		pthread_mutex_unlock(&d->lock);
		return ;
	}
	pthread_mutex_unlock(&d->lock);
	waiting_dongle_available(c, d);
}
