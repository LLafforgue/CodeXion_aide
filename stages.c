/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stages.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llafforg <llafforg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/06 16:40:30 by llafforg          #+#    #+#             */
/*   Updated: 2026/09/06 20:30:55 by llafforg         ###   ########.fr       */
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
	c->t_burnout = now_ms();
	pthread_cond_signal(&c->in_compil);
	pthread_mutex_unlock(&c->lock);
	usleep(c->datas->t_compile * 1000);
	pthread_mutex_lock(&c->lock);
	c->is_compil = 0;
	pthread_cond_signal(&c->in_compil);
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

void	take_dongles(t_coder *c)
{
	if (c->datas->end)
		return ;
	pthread_mutex_lock(&c->dongles_prev->lock);
	while (!c->dongles_prev->is_available && !c->datas->end)
		pthread_cond_wait(&c->dongles_prev->available, &c->dongles_prev->lock);
	c->dongles_prev->is_available = 0;
	c->dongles_prev->prev_user = c;
	pthread_mutex_unlock(&c->dongles_prev->lock);
	print_dgl(c, 'p');
	pthread_mutex_lock(&c->dongles_next->lock);
	while (!c->dongles_next->is_available && !c->datas->end)
		pthread_cond_wait(&c->dongles_next->available, &c->dongles_next->lock);
	c->dongles_next->is_available = 0;
	c->dongles_next->prev_user = c;
	pthread_mutex_unlock(&c->dongles_next->lock);
	print_dgl(c, 'n');
}
