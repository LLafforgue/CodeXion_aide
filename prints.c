/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prints.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llafforg <llafforg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/06 11:59:04 by llafforg          #+#    #+#             */
/*   Updated: 2026/09/06 20:34:29 by llafforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"

void	print_dgl(t_coder *c, char n_p)
{
	int		id_d;
	long	time;
	int		clr;

	clr = c->id % 6 + 1;
	if (n_p == 'p')
		id_d = c->dongles_prev->id;
	else
		id_d = c->dongles_next->id;
	time = now_ms() - c->datas->start_time;
	if (!c->datas->end)
		printf("[%ld ms] -\t\033[3%dm%d%s has taken a dongle (id %d).\n",
			time, clr, c->id, RESET, id_d);
}

void	print_log(t_coder *c, char *msg)
{
	long	time;
	int		clr;

	time = now_ms() - c->datas->start_time;
	clr = c->id % 6 + 1;
	printf("[%ld ms] -\t\033[3%dm%d%s is %s.\n",
		time, clr, c->id, RESET, msg);
}
