/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llafforg <llafforg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 14:16:59 by llafforg          #+#    #+#             */
/*   Updated: 2026/09/16 15:31:35 by llafforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../codexion.h"

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
	pthread_mutex_unlock(&d->lock);
}

t_coder	*other_coder(t_coder *c, t_dongle *d)
{
	if (d->coder_l == c)
		return (d->coder_r);
	return (d->coder_l);
}

t_dongle	*other_dongle(t_coder *c, t_dongle *d)
{
	if (d == c->dongles_next)
		return (c->dongles_prev);
	return (c->dongles_next);
}

t_dongle	*find_best_dongles_order(t_coder *c)
{
	t_coder		*next_c;
	t_coder		*prev_c;
	t_dongle	*first;

	pthread_mutex_lock(&c->dongles_prev->lock);
	prev_c = c->dongles_prev->user[0];
	pthread_mutex_unlock(&c->dongles_prev->lock);
	pthread_mutex_lock(&c->dongles_next->lock);
	next_c = c->dongles_next->user[0];
	pthread_mutex_unlock(&c->dongles_next->lock);
	if (!prev_c)
		return (c->dongles_prev);
	if (!next_c)
		return (c->dongles_next);
	pthread_mutex_lock(&prev_c->lock);
	if (prev_c->dongles_took < next_c->dongles_took)
		first = c->dongles_next;
	else if (prev_c->dongles_took > next_c->dongles_took)
		first = c->dongles_prev;
	else if (prev_c->t_burnout < next_c->t_burnout && c->datas->scheduler)
		first = c->dongles_prev;
	else
		first = c->dongles_next;
	pthread_mutex_unlock(&prev_c->lock);
	return (first);
}
