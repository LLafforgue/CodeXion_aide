/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llafforg <llafforg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 15:39:56 by llafforg          #+#    #+#             */
/*   Updated: 2026/08/20 15:04:25 by llafforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
	while (i < 8)
	{
		if (atoi(argv[i]) <= 0)
		{
			printf("\033[33;1mError: Invalid argument value.\033[0m\n");
			printf("The 7 first arguments must be positive integers.\n");
			printf("Try again !\n");
			return (1);
		}
		i++;
	}
	if (strcmp("fifo", argv[8]) && strcmp("edf", argv[8]))
	{
		printf("\033[33;1mError: Invalid argument value.\033[0m\n");
		printf("Scheduler must be 'fifo' or 'edf'.\n");
		printf("Try again !\n");
		return (1);
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_data		*data;
	t_coder		*coders;

	coders = NULL;
	data = NULL;
	if (arg_nbr_error(argc) || arg_value_error(argv))
		return (1);
	if (init_data(argv, &data))
		init_coders(data, &coders);
	if (coders == NULL)
	{
		printf("\033[33;1mAllocations failled!!!\033[0m");
		return (1);
	}
	data->coders = &coders;
	if (!init_dongles(data))
		return (1);
	print_coders(coders);
	free_all(data);
	return (0);
}
