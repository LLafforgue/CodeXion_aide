/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_free.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llafforg <llafforg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/08 14:50:57 by llafforg          #+#    #+#             */
/*   Updated: 2026/09/08 15:13:02 by llafforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdlib.h>

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
		pthread_cond_destroy(&current->dongles_prev->available);
		free(current->dongles_prev);
		pthread_mutex_destroy(&current->lock);
		free(current);
		current = next;
		n--;
	}
	pthread_mutex_destroy(&data->lock);
	free(data);
}

t_coder	*free_coders(t_coder *coder)
{
	int		nbr;
	t_coder	*temp;

	nbr = 1;
	temp = coder;
	while (coder)
	{
		if (nbr++ == coder->datas->coder_nbr)
			return (temp);
		coder = coder->next;
	}
	while (temp)
	{
		coder = temp;
		pthread_mutex_destroy(&coder->dongles_prev->lock);
		pthread_cond_destroy(&coder->dongles_prev->available);
		free(coder->dongles_prev);
		pthread_mutex_destroy(&coder->lock);
		temp = temp->next;
		free(coder);
	}
	return (NULL);
}
