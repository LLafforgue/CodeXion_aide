/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llafforg <llafforg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 15:39:56 by llafforg          #+#    #+#             */
/*   Updated: 2026/08/18 16:31:10 by llafforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <codexion.h>
#include <stdio.h>
#include <stdlib.h>

int	arg_nbr_error(int argc)
{
	if (argc != 9)
	{
		printf("\033[33;1mError: Invalid number of arguments (8).\033[0m\n");
		printf("Usage: ./codexion");
		printf(" <coder_nbr> <t_burnout> <t_compile>");
		printf(" <t_debug> <t_refactor> <nbr_compile>");
		printf(" <dongle_cool> <scheduler>\n");
		printf("Try again !\n");
		return (1);
	}
	return (0);
}

int	arg_value_error(char **argv)
{
	int	i;

	i = 1;
	while (i < 9)
	{
		if (atoi(argv[i]) <= 0)
		{
			printf("\033[31;1mError: Invalid argument value.\033[0m\n");
			printf("All arguments must be positive integers.\n");
			printf("Try again !\n");
			return (1);
		}
		i++;
	}
	return (0);
}

t_data	*init_data(char **argv)
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (!data)
	{
		printf("\033[31;1mError: Memory allocation failed.\033[0m\n");
		return (NULL);
	}
	data->coder_nbr = atoi(argv[1]);
	data->t_burnout = atoi(argv[2]);
	data->t_compile = atoi(argv[3]);
	data->t_debug = atoi(argv[4]);
	data->t_refactor = atoi(argv[5]);
	data->nbr_compile = atoi(argv[6]);
	data->dongle_cool = atoi(argv[7]);
	data->scheduler = atoi(argv[8]);
	data->coders = NULL;
	data->dongles = NULL;
	return (data);
}

void	init_coders(t_data	**data, t_coder **coders)
{
	int		nbr;
	t_coder	*temp;
	t_coder	*new_coder;

	nbr = 0;
	while (nbr < (*data)->coder_nbr)
	{
		if (nbr == 0)
		{
			*coders = create_coder(*data, nbr);
			temp = *coders;
		}
		else
		{
			new_coder = create_coder(*data, nbr);
			new_coder->prev = *coders;
			(*coders)->next = new_coder;
			*coders = new_coder;
		}
		nbr++;
	}
	*coders = temp;
}

t_coder	*create_coder(t_data *data, int nbr)
{
	t_coder	*coder;

	coder = malloc(sizeof(t_coder));
	if (!coder)
		return (coder = NULL);
	coder->id = nbr;
	coder->nbr_compile = 0;
	coder->stage = 0;
	coder->t_burnout = data->t_burnout;
	coder->dongles = malloc(sizeof(t_dongle) * 2);
	if (! coder->dongles)
		return (NULL);
	return (coder);
}

int	main(int argc, char **argv)
{
	t_data		*data;
	t_coder		*coders;
	t_dongle	*dongle;

	coders = NULL;
	if (arg_nbr_error(argc) || arg_value_error(argv))
		return (1);
	data = init_data(argv);
	init_coders(&data, &coders);
	return (0);
}
