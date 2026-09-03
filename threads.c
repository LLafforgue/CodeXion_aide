/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llafforg <llafforg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 15:06:14 by llafforg          #+#    #+#             */
/*   Updated: 2026/09/03 17:48:50 by llafforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include "codexion.h"

// voit	ft_codexion(t_data **datas)
// {
// 	pthread_t	*thds;

// 	thds = malloc((*datas)->coder_nbr * sizeof(pthread_t));
// }

void	*print_valid(void *arg)
{
	int	id;

	id = *(int *)arg;
	printf("thread du coder \033[32m%d\033[0m cree\n", id);
	return (NULL);
}

void	ft_thread_init(t_data **datas)
{
	pthread_t	thds[100];
	int			i;
	t_coder		*curent_c;

	i = 0;
	curent_c = *((*datas)->coders);
	while (i != (*datas)->coder_nbr)
	{
		pthread_create(&thds[i], NULL, print_valid, &(curent_c->id));
		i++;
		curent_c = curent_c->next;
	}
	i = 0;
	while (i != (*datas)->coder_nbr)
		pthread_join(thds[i++], NULL);
}
