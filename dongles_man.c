/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles_man.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llafforg <llafforg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 19:00:18 by llafforg          #+#    #+#             */
/*   Updated: 2026/09/15 18:56:06 by llafforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

static void	ft_take_one(t_coder *c, t_dongle *d)
{
	if (!c->datas->scheduler)
		strategie_fifo(c, d);
	else
		strategie_edf(c, d);
	waiting_dongle_cooldown(c, d);
}

static void	ft_after_ordering(t_coder *c, t_dongle *first, t_dongle *second)
{
	ft_take_one(c, first);
	c->dongles_took++;
	print_dgl(c, first->id);
	if (c->datas->coder_nbr < 2)
		return ;
	ft_take_one(c, second);
	print_dgl(c, second->id);
	c->dongles_took++;
}

void	take_dongles(t_coder *c)
{
	t_dongle	*first;

	if (c->datas->end)
		return ;
	if (c->id % 2 == 0 && !c->nbr_compile)
		first = c->dongles_prev;
	else if (!c->nbr_compile)
		first = c->dongles_next;
	else
		first = find_best_dongles_order(c);
	ft_after_ordering(c, first, other_dongle(c, first));
}

static void	ft_let_one_dongle(t_coder *c, t_dongle *d)
{
	d->t_cooldown = now_ms() + c->datas->t_dongle_cool;
	pthread_mutex_lock(&d->l_able);
	if (d->user[1])
	{
		d->user[0] = d->user[1];
		d->user[1] = NULL;
	}
	else
		d->user[0] = NULL;
	pthread_cond_broadcast(&d->available);
	pthread_mutex_unlock(&d->l_able);
}

void	let_dongles(t_coder *c)
{
	if (c->dongles_took)
	{
		if (c->dongles_prev->user[0] == c)
			ft_let_one_dongle(c, c->dongles_prev);
		if (c->dongles_next->user[0] == c)
			ft_let_one_dongle(c, c->dongles_next);
		pthread_mutex_lock(&c->lock);
		c->dongles_took = 0;
		pthread_mutex_unlock(&c->lock);
	}
}
