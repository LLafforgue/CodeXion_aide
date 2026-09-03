/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llafforg <llafforg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/18 15:07:06 by llafforg          #+#    #+#             */
/*   Updated: 2026/09/03 17:49:01 by llafforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdio.h>

typedef struct s_dongle
{
	int					id;
	int					is_available;
	struct s_coder		*coder_l;
	struct s_coder		*coder_r;
}	t_dongle;

typedef struct s_coder
{
	int					id;
	int					t_burnout;
	int					stage;
	int					nbr_compile;
	// pthread_t			thread_id;
	struct s_dongle		*dongles_prev;
	struct s_dongle		*dongles_next;
	struct s_coder		*prev;
	struct s_coder		*next;
}	t_coder;

typedef struct s_data
{
	int			coder_nbr;
	int			t_burnout;
	int			t_compile;
	int			t_debug;
	int			t_refactor;
	int			nbr_compile;
	int			dongle_cool;
	int			scheduler;
	long		current_time;
	t_coder		**coders;
}	t_data;

// utils.c
void		print_coders(t_coder *head);
void		free_all(t_data *data);

// init.c
int			init_data(char **argv, t_data **data);
t_coder		*create_coder(t_data *data, int nbr);
t_dongle	*create_dongle(int nbr);
void		init_coders(t_data	*data, t_coder **coders);
int			init_dongles(t_data *data);

// simulations
p
#endif