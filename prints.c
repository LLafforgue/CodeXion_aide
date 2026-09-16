/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prints.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llafforg <llafforg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/06 11:59:04 by llafforg          #+#    #+#             */
/*   Updated: 2026/09/15 15:49:15 by llafforg         ###   ########.fr       */
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

int	test_a_coder(t_coder *c, int id, char *message)
{
	if (c->id == id)
		return (printf("\033[31;1m--- Test %d :\t%s---\n%s",
				c->id, message, END));
	return (0);
}

void	test_dongle(t_dongle *dongle)
{
	int	one;
	int	two;

	if (dongle->user[0])
		one = dongle->user[0]->id;
	else
		one = 0;
	if (dongle->user[1])
		two = dongle->user[1]->id;
	else
		two = 0;
	printf("---\t\tusers de (%d) : [%d, %d] ---\n", dongle->id, one, two);
}
