/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llafforg <llafforg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 14:16:59 by llafforg          #+#    #+#             */
/*   Updated: 2026/09/08 16:56:55 by llafforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

long	now_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	toggle_end(t_coder *c, char cause)
{
	t_data	*d;

	d = c->datas;
	if (cause == 'b')
		print_log(c, "in\033[1;2m burnout\033[0m");
	if (cause == 'c')
		printf("\033[1;3m Coders win !!!\033[0m\n");
	pthread_mutex_lock(&d->lock);
	d->end = 1;
	pthread_cond_broadcast(&c->dongles_next->available);
	pthread_cond_broadcast(&c->dongles_prev->available);
	pthread_mutex_unlock(&d->lock);
}

