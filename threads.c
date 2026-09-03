/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llafforg <llafforg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 15:06:14 by llafforg          #+#    #+#             */
/*   Updated: 2026/09/03 17:09:57 by llafforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "codexion.h"

// voit	ft_codexion(t_data **datas)
// {
// 	pthread_t	*thds;

// 	thds = malloc((*datas)->coder_nbr * sizeof(pthread_t));
// }

int	print_valid(void *arg)
{
	int	id;

	id = *(int *)arg;
	printf("thread du coder \033[32m%d\033[0m cree");
	return (0);
}

void	ft_thread_init(t_data **datas)
{
	pthread_t	thds[100];
	int			ids[100];
	int			i;
	t_coder		*curent_c;

	i = 0;
	curent_c = (*datas)->coders;
	while (i != (*datas)->coder_nbr)
	{
		
		pthread_create()
	}
}