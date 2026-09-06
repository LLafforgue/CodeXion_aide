/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stages.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llafforg <llafforg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/06 16:40:30 by llafforg          #+#    #+#             */
/*   Updated: 2026/09/06 18:08:55 by llafforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "codexion.h"
#include <unistd.h>

int	compilation(t_coder *c)
{
	if (c->datas->end)
		return (0);
	pthread_mutex_lock(&c->lock);
	print_log(c, "compiling");
	c->is_compil = 1;
	c->nbr_compile++;
	pthread_cond_signal(&c->in_compil);
	pthread_mutex_unlock(&c->lock);
	usleep(c->datas->t_compile * 1000);
	pthread_mutex_lock(&c->lock);
	c->t_burnout = now_ms();
	c->is_compil = 0;
	pthread_cond_signal(&c->in_compil);
	pthread_mutex_unlock(&c->lock);
	return (1);
}

int	debugging(t_coder *c)
{
	if (c->datas->end)
		return (0);
	print_log(c, "debugging");
	return (1);
}
