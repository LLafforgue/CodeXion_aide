/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llafforg <llafforg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/18 15:07:06 by llafforg          #+#    #+#             */
/*   Updated: 2026/09/08 20:27:52 by llafforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdio.h>
# include <sys/time.h>

# define END "\033[0m"
# define ERR "\033[33;1mError:"

typedef struct s_dongle
{
	int					id;
	int					is_available;
	long				t_cooldown;
	struct s_coder		*coder_l;
	struct s_coder		*coder_r;
	struct s_coder		*user[2];
	pthread_mutex_t		lock;
	pthread_cond_t		available;
}	t_dongle;

typedef struct s_coder
{
	int					id;
	long				t_burnout;
	int					stage;
	int					nbr_compile;
	int					max_reach;
	int					dongles_took;
	pthread_t			thread_id;
	struct s_dongle		*dongles_prev;
	struct s_dongle		*dongles_next;
	struct s_coder		*prev;
	struct s_coder		*next;
	struct s_data		*datas;
	pthread_mutex_t		lock;
	int					is_compil;
}	t_coder;

typedef struct s_data
{
	int					coder_nbr;
	int					t_burnout;
	int					t_compile;
	int					t_debug;
	int					t_refactor;
	int					nbr_compile;
	int					scheduler;
	long				t_dongle_cool;
	long				start_time;
	int					end;
	pthread_t			burnout_watcher;
	pthread_mutex_t		lock;
	t_coder				**coders;
}	t_data;

// utils
long		now_ms(void);
void		toggle_end(t_coder *c, char cause);
void		let_dongles(t_coder *c);
t_coder		*other_coder(t_coder *c, t_dongle *d);

// utils_free
void		free_all(t_data *data);
t_coder		*free_coders(t_coder *coder);

// init
int			init_data(char **argv, t_data **data);
t_coder		*create_coder(t_data *data, int nbr);
t_coder		*init_coders(t_data	*data, t_coder *coders);
int			init_dongles(t_data *data);

// threads
void		thread_init(t_data **datas);

// prints
void		print_dgl(t_coder *c, int id_d);
void		print_log(t_coder *c, char *msg);

// stages
int			compilation(t_coder *c);
void		take_dongles(t_coder *c);
int			debugging(t_coder *c);
void		refactoring(t_coder *c);

// strategies
void		strategie(t_coder *c, t_dongle *d);

#endif
