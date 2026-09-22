/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stages.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llafforg <llafforg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/06 16:40:30 by llafforg          #+#    #+#             */
/*   Updated: 2026/09/17 16:16:39 by llafforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <unistd.h>

int	compilation(t_coder *c)
{
	if (c->datas->end)
	{
		let_dongles(c);
		return (0);
	}
	pthread_mutex_lock(&c->l_burnout);
	c->t_burnout = now_ms();
	pthread_mutex_unlock(&c->l_burnout);
	print_log(c, "compiling");
	usleep(c->datas->t_compile * 1000);
	pthread_mutex_lock(&c->lock);
	c->nbr_compile++;
	pthread_mutex_unlock(&c->lock);
	let_dongles(c);
	return (1);
}

int	debugging(t_coder *c)
{
	if (c->datas->end)
		return (0);
	print_log(c, "debugging");
	usleep(c->datas->t_debug * 1000);
	return (1);
}

void	refactoring(t_coder *c)
{
	if (c->datas->end)
		return ;
	print_log(c, "refactoring");
	usleep(c->datas->t_refactor * 1000);
}
