/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   waiting.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llafforg <llafforg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 18:34:02 by llafforg          #+#    #+#             */
/*   Updated: 2026/09/15 18:57:18 by llafforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <unistd.h>

void	waiting_dongle_cooldown(t_coder *c, t_dongle *d)
{
	long	waiting;

	pthread_mutex_lock(&d->l_able);
	waiting = d->t_cooldown;
	pthread_mutex_unlock(&d->l_able);
	while (!c->datas->end)
	{
		if (waiting <= now_ms())
			return ;
	}
}

void	waiting_dongle_available(t_coder *c, t_dongle *d)
{
	pthread_mutex_lock(&d->lock);
	d->user[1] = c;
	pthread_mutex_unlock(&d->lock);
	pthread_mutex_lock(&d->l_able);
	while (d->user[0] == other_coder(c, d) && !c->datas->end)
		pthread_cond_wait(&d->available, &d->l_able);
	d->user[1] = NULL;
	pthread_mutex_unlock(&d->l_able);
}
