/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   strategies.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llafforg <llafforg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/08 16:18:11 by llafforg          #+#    #+#             */
/*   Updated: 2026/09/17 15:15:38 by llafforg         ###   ########.fr       */
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
	print_dgl(c, d->id);
	return ;
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
