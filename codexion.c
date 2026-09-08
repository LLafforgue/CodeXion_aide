/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llafforg <llafforg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/17 15:39:56 by llafforg          #+#    #+#             */
/*   Updated: 2026/09/08 15:24:12 by llafforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <stdlib.h>
#include <string.h>

int	arg_nbr_error(int argc, char **argv)
{
	int		i;
	char	*args[7];

	if (argc == 9)
		return (0);
	printf("%s Invalid number of arguments (8).%s\n", ERR, END);
	printf("Usage: ./codexion");
	args[0] = "<coder_nbr>";
	args[1] = "<t_burnout>";
	args[2] = "<t_compile>";
	args[3] = "<t_debug>";
	args[4] = "<t_refactor>";
	args[5] = "<nbr_compile>";
	args[6] = "<dongle_cool>";
	i = 0;
	while (argv[++i])
		printf(" %s", argv[i]);
	while (i < 8)
		printf(" \033[1m%s%s", args[i++ - 1], END);
	return (printf(" \033[1m<scheduler>%s\nTry again !\n", END));
}

int	arg_value_error(char **argv)
{
	int	i;

	i = 0;
	while (i++ < 8)
	{
		if ((i == 1 && atoi(argv[i]) <= 0) || (i == 6 && atoi(argv[i]) == 0))
		{
			printf("%s At least one coder and compilation !!!%s\n", ERR, END);
			return (printf("Try again !\n"));
		}
		if (i > 1 && atoi(argv[i]) < 0)
		{
			printf("%s Invalid argument value.%s\n", ERR, END);
			printf("The 7 first arguments must be positive integers.\n");
			return (printf("Try again !\n"));
		}
	}
	if (strcmp("fifo", argv[8]) && strcmp("edf", argv[8]))
	{
		printf("%s Invalid argument value.%s\n", ERR, END);
		printf("Scheduler must be 'fifo' or 'edf'.\n");
		return (printf("Try again !\n"));
	}
	return (0);
}

int	main(int argc, char **argv)
{
	t_data		*data;
	t_coder		*coders;

	coders = NULL;
	data = NULL;
	if (arg_nbr_error(argc, argv) || arg_value_error(argv))
		return (1);
	if (init_data(argv, &data))
		coders = init_coders(data, coders);
	if (coders == NULL)
	{
		printf("\033[33;1mAllocations failled!!!\033[0m");
		return (1);
	}
	data->coders = &coders;
	if (!init_dongles(data))
		return (1);
	thread_init(&data);
	free_all(data);
	return (0);
}
