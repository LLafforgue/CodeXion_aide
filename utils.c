/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llafforg <llafforg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/20 14:16:59 by llafforg          #+#    #+#             */
/*   Updated: 2026/09/03 18:18:02 by llafforg         ###   ########.fr       */
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
		free(current->dongles_prev);
		free(current);
		current = next;
		n--;
	}
	free(data);
}

long	now_ms(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}
