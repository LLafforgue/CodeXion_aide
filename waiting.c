/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   waiting.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llafforg <llafforg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/11 18:34:02 by llafforg          #+#    #+#             */
/*   Updated: 2026/09/17 15:45:30 by llafforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <unistd.h>

void	waiting_dongle_cooldown(t_coder *c, t_dongle *d)
{
	long	waiting;

	waiting = d->t_cooldown;
	while (!c->datas->end)
	{
		if (waiting <= now_ms())
			return ;
	}
}

void	waiting_dongle_available(t_coder *c, t_dongle *d)
{
	while (d->user[0] == other_coder(c, d) && !c->datas->end)
		pthread_cond_wait(&d->available, &d->lock);
	pthread_mutex_unlock(&d->lock);
}
