/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llafforg <llafforg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/02 15:06:14 by llafforg          #+#    #+#             */
/*   Updated: 2026/09/03 18:54:34 by llafforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include "codexion.h"

// voit	ft_codexion(t_data **datas)
// {
// 	pthread_t	*thds;

// 	thds = malloc((*datas)->coder_nbr * sizeof(pthread_t));
// }

void	*print_valid(void *arg_coder)
{
	t_coder	*coder;
	int		id;
	long	t;

	coder = (t_coder *)arg_coder;
	id = coder->id;
	usleep(1000000);
	t = now_ms() - coder->datas->start_time;
	printf("thread du coder \033[3%dm%d\033[0m cree ", (id % 5) + 1, id);
	printf("at \033[32m%ld ms\033[0m cree\n", t);
	return (NULL);
}

void	ft_thread_init(t_data **datas)
{
	int			i;
	t_coder		*curent_c;

	i = 0;
	curent_c = *((*datas)->coders);
	while (i != (*datas)->coder_nbr)
	{
		pthread_create(&curent_c->thread_id, NULL, print_valid, curent_c);
		i++;
		curent_c = curent_c->next;
	}
	i = 0;
	printf("[current coder: %d]\n", curent_c->id);
	while (i != (*datas)->coder_nbr)
	{
		i++;
		pthread_join(curent_c->thread_id, NULL);
		curent_c = curent_c->next;
	}
}
