/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llafforg <llafforg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 15:03:36 by llafforg          #+#    #+#             */
/*   Updated: 2026/09/03 18:24:34 by llafforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


int	init_data(char **argv, t_data **data)
{

	(*data) = malloc(sizeof(t_data));
	if (!(*data))
		return (0);
	(*data)->coder_nbr = atoi(argv[1]);
	(*data)->t_burnout = atoi(argv[2]);
	(*data)->t_compile = atoi(argv[3]);
	(*data)->t_debug = atoi(argv[4]);
	(*data)->t_refactor = atoi(argv[5]);
	(*data)->nbr_compile = atoi(argv[6]);
	(*data)->dongle_cool = atoi(argv[7]);
	if (strcmp("fifo", argv[8]))
		(*data)->scheduler = 1;
	else
		(*data)->scheduler = 2;
	(*data)->coders = NULL;
	(*data)->start_time = now_ms();
	return (1);
}

t_coder	*create_coder(t_data *data, int nbr)
{
	t_coder	*coder;

	coder = malloc(sizeof(t_coder));
	if (!coder)
		return (NULL);
	coder->dongles_prev = NULL;
	coder->dongles_next = NULL;
	coder->id = nbr;
	coder->nbr_compile = 0;
	coder->stage = 0;
	coder->t_burnout = data->t_burnout;
	coder->next = NULL;
	coder->prev = NULL;
	coder->datas = data;
	return (coder);
}

t_dongle	*create_dongle(int nbr)
{
	t_dongle	*dongle;

	dongle = malloc(sizeof(t_dongle));
	if (!dongle)
		return (NULL);
	dongle->id = nbr;
	dongle->is_available = 0;
	dongle->coder_r = NULL;
	dongle->coder_l = NULL;

	return (dongle);
}

void	init_coders(t_data	*data, t_coder **coders)
{
	int		nbr;
	t_coder	*temp;
	t_coder	*new_coder;

	nbr = 0;
	while (nbr < data->coder_nbr)
	{
		if (nbr == 0)
		{
			*coders = create_coder(data, nbr);
			temp = *coders;
		}
		else
		{
			new_coder = create_coder(data, nbr);
			new_coder->prev = *coders;
			(*coders)->next = new_coder;
			*coders = new_coder;
		}
		nbr++;
	}
	temp->prev = *coders;
	(*coders)->next = temp;
	*coders = temp;
}

int	init_dongles(t_data *data)
{
	int			nbr;
	t_dongle	*new_dongle;
	t_coder		*current_c;

	nbr = 0;
	current_c = *(data->coders);
	while (nbr < data->coder_nbr)
	{
		new_dongle = create_dongle(nbr);
		if (!new_dongle)
		{
			printf("\033[33,1mAllocation error [dongle]\033[0m");
			return (0);
		}
		new_dongle->coder_l = current_c;
		current_c->dongles_next = new_dongle;

		new_dongle->coder_r = current_c->next;
		current_c->next->dongles_prev = new_dongle;
		nbr++;
		current_c = current_c->next;
	}
	return (1);
}
