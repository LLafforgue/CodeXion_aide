/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prints.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llafforg <llafforg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/06 11:59:04 by llafforg          #+#    #+#             */
/*   Updated: 2026/09/08 22:18:43 by llafforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	print_dgl(t_coder *c, int id_d)
{
	long	time;
	int		clr;

	clr = c->id % 6 + 1;
	time = now_ms() - c->datas->start_time;
	pthread_mutex_lock(&c->datas->lock);
	if (!c->datas->end)
		printf("[%ld ms] -\t\033[3%dm%d%s has taken a dongle (id %d).\n",
			time, clr, c->id, END, id_d);
	pthread_mutex_unlock(&c->datas->lock);
}

void	print_log(t_coder *c, char *msg)
{
	long	time;
	int		clr;

	time = now_ms() - c->datas->start_time;
	clr = c->id % 6 + 1;
	printf("[%ld ms] -\t\033[3%dm%d%s is %s.\n",
		time, clr, c->id, END, msg);
}
