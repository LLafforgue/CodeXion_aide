/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dongles_man.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llafforg <llafforg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 19:00:18 by llafforg          #+#    #+#             */
/*   Updated: 2026/09/17 16:42:08 by llafforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

// static void	ft_take_one(t_coder *c, t_dongle *d)
// {
// 	if (!c->datas->scheduler)
// 		strategie_fifo(c, d);
// 	else
// 		strategie_edf(c, d);
// 	waiting_dongle_cooldown(c, d);
// }

static void	ft_after_ordering_fifo(t_coder *c, t_dongle *first, t_dongle *second)
{
	while (!c->datas->end && c->dongles_took < 2)
	{
		pthread_mutex_lock(&first->lock);
		if (!first->user[0])
		{
			pthread_mutex_unlock(&first->lock);
			pthread_mutex_lock(&first->keep);
			first->user[0] = c;
			pthread_mutex_lock(&c->lock);
			c->dongles_took++;
			pthread_mutex_unlock(&c->lock);
			waiting_dongle_cooldown(c, first);
			print_dgl(c, first->id);
		}
		else if (first->user[0] != c && first->user[0]->dongles_took != 2)
		{
			pthread_mutex_unlock(&first->lock);
			first->user[0] = c;
			pthread_mutex_lock(&c->lock);
			c->dongles_took++;
			pthread_mutex_unlock(&c->lock);
			pthread_mutex_lock(&other_coder(c, first)->lock);
			print_log(other_coder(c, first), "leaving a dongle");
			c->dongles_took--;
			pthread_mutex_unlock(&other_coder(c, first)->lock);
			waiting_dongle_cooldown(c, first);
			print_dgl(c, first->id);
		}
		else
			waiting_dongle_available(c, first);
		if (c->datas->coder_nbr < 2)
			break ;
		pthread_mutex_lock(&second->lock);
		if (!second->user[0])
		{
			pthread_mutex_unlock(&second->lock);
			pthread_mutex_lock(&second->keep);
			second->user[0] = c;
			pthread_mutex_lock(&c->lock);
			c->dongles_took++;
			pthread_mutex_unlock(&c->lock);
			waiting_dongle_cooldown(c, second);
			print_dgl(c, second->id);
		}
		else if (second->user[0] != c && second->user[0]->dongles_took != 2)
		{
			pthread_mutex_unlock(&second->lock);
			second->user[0] = c;
			pthread_mutex_lock(&c->lock);
			c->dongles_took++;
			pthread_mutex_unlock(&c->lock);
			pthread_mutex_lock(&other_coder(c, second)->lock);
			print_log(other_coder(c, second), "leaving a dongle");
			c->dongles_took--;
			pthread_mutex_unlock(&other_coder(c, second)->lock);
			waiting_dongle_cooldown(c, second);
			print_dgl(c, second->id);
		}
		else
			waiting_dongle_available(c, second);
	}
}

static void	ft_after_ordering_edf(t_coder *c, t_dongle *first, t_dongle *second)
{
	while (!c->datas->end && c->dongles_took < 2)
	{
		pthread_mutex_lock(&first->lock);
		if (!first->user[0])
		{
			pthread_mutex_unlock(&first->lock);
			pthread_mutex_lock(&first->keep);
			if (c == edf_first_coder(c, first))
			{
				first->user[0] = c;
				pthread_mutex_lock(&c->lock);
				c->dongles_took++;
				pthread_mutex_unlock(&c->lock);
				waiting_dongle_cooldown(c, first);
				print_dgl(c, first->id);
			}
			else
			{
				pthread_mutex_unlock(&first->keep);
				if (first->user[0] == c)
					c->dongles_took--;
				print_log(c, "dont take the dongle");
			}
		}
		else
			waiting_dongle_available(c, first);
		if (c->datas->coder_nbr < 2)
			break ;
		pthread_mutex_lock(&second->lock);
		if (!second->user[0])
		{
			pthread_mutex_unlock(&second->lock);
			if (c == edf_first_coder(c, second))
			{
				second->user[0] = c;
				pthread_mutex_lock(&second->keep);
				pthread_mutex_lock(&c->lock);
				c->dongles_took++;
				pthread_mutex_unlock(&c->lock);
				waiting_dongle_cooldown(c, second);
				print_dgl(c, second->id);
			}
			else
			{
				pthread_mutex_unlock(&second->keep);
				if (second->user[0] == c)
					c->dongles_took--;
				print_log(c, "\033[1mdont take the dongle");
			}
		}
		else
			waiting_dongle_available(c, second);
	}
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
	if (!c->datas->scheduler)
		ft_after_ordering_fifo(c, first, other_dongle(c, first));
	else
		ft_after_ordering_edf(c, first, other_dongle(c, first));
}

static void	ft_let_one_dongle(t_coder *c, t_dongle *d)
{
	d->user[0] = NULL;
	c->dongles_took--;
	pthread_cond_broadcast(&d->available);
	pthread_mutex_unlock(&d->keep);
}

void	let_dongles(t_coder *c)
{
	if (c->dongles_took)
	{
		if (c->dongles_prev->user[0] == c)
			ft_let_one_dongle(c, c->dongles_prev);
		if (c->dongles_next->user[0] == c)
			ft_let_one_dongle(c, c->dongles_next);
		c->dongles_took = 0;
	}
}
