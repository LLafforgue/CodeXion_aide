/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llafforg <llafforg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 14:16:59 by llafforg          #+#    #+#             */
/*   Updated: 2026/09/06 17:04:11 by llafforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdlib.h>

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

void	free_all(t_data *data)
{
	t_coder	*current;
	t_coder	*next;
	int		n;

	if (!data)
		return ;
	current = *(data->coders);
	n = data->coder_nbr;
	while (n > 0)
	{
		next = current->next;
		pthread_mutex_destroy(&current->dongles_prev->lock);
		free(current->dongles_prev);
		pthread_cond_destroy(&current->in_compil);
		pthread_mutex_destroy(&current->lock);
		free(current);
		current = next;
		n--;
	}
	pthread_mutex_destroy(&data->lock);
	free(data);
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
		print_log(c, "in\033[1;3m the winner\033[0m");
	pthread_mutex_lock(&d->lock);
	d->end = 1;
	pthread_cond_broadcast(&c->in_compil);
	pthread_mutex_unlock(&d->lock);
}
