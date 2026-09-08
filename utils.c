/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llafforg <llafforg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 14:16:59 by llafforg          #+#    #+#             */
/*   Updated: 2026/09/08 14:52:34 by llafforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	print_coders(t_coder *head)
{
	t_coder	*cur;

	if (!head)
		return ;
	cur = head;
	printf("(d)->\t\033[1m[cdr]\033[0m\t->(d)\n");
	while (cur != head->prev)
	{
		printf("(%d)->\t", cur->dongles_prev->id);
		printf("\033[1m[%d]\033[0m\t->(%d)", cur->id, cur->dongles_next->id);
		if (cur->next != head)
			printf("\n");
		cur = cur->next;
	}
	printf("(%d)->\t", cur->dongles_prev->id);
	printf("\033[1m[%d]\033[0m\t->(%d)", cur->id, cur->dongles_next->id);
	printf("\n");
}

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

void	let_dongles(t_coder *c)
{
	t_dongle	*d;

	d = c->dongles_prev;
	pthread_mutex_lock(&d->lock);
	d->is_available = 1;
	d->t_cooldown = now_ms() + c->datas->t_dongle_cool;
	d->user = NULL;
	pthread_cond_signal(&d->available);
	pthread_mutex_unlock(&d->lock);
	d = c->dongles_next;
	pthread_mutex_lock(&d->lock);
	d->t_cooldown = now_ms() + c->datas->t_dongle_cool;
	d->is_available = 1;
	d->user = NULL;
	pthread_cond_signal(&d->available);
	pthread_mutex_unlock(&d->lock);
}
