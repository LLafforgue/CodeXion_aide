/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stages.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llafforg <llafforg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/06 16:40:30 by llafforg          #+#    #+#             */
/*   Updated: 2026/09/08 16:56:00 by llafforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <unistd.h>

int	compilation(t_coder *c)
{
	if (c->datas->end)
		return (0);
	pthread_mutex_lock(&c->lock);
	print_log(c, "compiling");
	c->is_compil = 1;
	c->nbr_compile++;
	c->t_burnout = now_ms();
	pthread_mutex_unlock(&c->lock);
	usleep(c->datas->t_compile * 1000);
	pthread_mutex_lock(&c->lock);
	c->is_compil = 0;
	pthread_mutex_unlock(&c->lock);
	let_dongles(c);
	return (1);
}

int	debugging(t_coder *c)
{
	if (c->datas->end)
		return (0);
	print_log(c, "debugging");
	usleep(c->datas->t_debug * 1000);
	return (1);
}

void	ft_take_one(t_coder *c, t_dongle *d)
{
	struct timespec	ts;
	long			target;

	if (c->datas->scheduler)
		strategie_fifo(c, d);
	else
		strategie_fifo(c, d);
	pthread_mutex_lock(&d->lock);
	while (!c->datas->end)
	{
		if (d->is_available && now_ms() >= d->t_cooldown && c == d->user[0])
			break ;
		if (!d->is_available)
			pthread_cond_wait(&d->available, &d->lock);
		else
		{
			clock_gettime(CLOCK_REALTIME, &ts);
			target = d->t_cooldown;
			ts.tv_sec = target / 1000;
			ts.tv_nsec = (target % 1000) * 1000000;
			pthread_cond_timedwait(&d->available, &d->lock, &ts);
		}
	}
	d->is_available = 0;
	pthread_mutex_unlock(&d->lock);
	print_dgl(c, d->id);
}

void	take_dongles(t_coder *c)
{
	if (c->datas->end)
		return ;
	if (c->dongles_prev->id < c->dongles_next->id)
	{
		ft_take_one(c, c->dongles_prev);
		ft_take_one(c, c->dongles_next);
	}
	else
	{
		ft_take_one(c, c->dongles_next);
		ft_take_one(c, c->dongles_prev);
	}
}

void	refactoring(t_coder *c)
{
	if (c->datas->end)
		return ;
	print_log(c, "refactoring");
	usleep(c->datas->t_refactor * 1000);
}


