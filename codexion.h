/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llafforg <llafforg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/18 15:07:06 by llafforg          #+#    #+#             */
/*   Updated: 2026/09/06 18:26:30 by llafforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODEXION_H
# define CODEXION_H

# include <pthread.h>
# include <stdio.h>
# include <sys/time.h>

# define RESET "\033[0m"

typedef struct s_dongle
{
	int					id;
	int					is_available;
	struct s_coder		*coder_l;
	struct s_coder		*coder_r;
	pthread_mutex_t		lock;
}	t_dongle;

typedef struct s_coder
{
	int					id;
	long				t_burnout;
	int					stage;
	int					nbr_compile;
	pthread_t			thread_id;
	struct s_dongle		*dongles_prev;
	struct s_dongle		*dongles_next;
	struct s_coder		*prev;
	struct s_coder		*next;
	struct s_data		*datas;
	pthread_mutex_t		lock;
	pthread_cond_t		in_compil;
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
	int					dongle_cool;
	int					scheduler;
	long				start_time;
	int					end;
	pthread_t			burnout_watcher;
	pthread_mutex_t		lock;
	t_coder				**coders;
}	t_data;

// utils
void		print_coders(t_coder *head);
void		free_all(t_data *data);
long		now_ms(void);
void		toggle_end(t_coder *c, char cause);

// init
int			init_data(char **argv, t_data **data);
t_coder		*create_coder(t_data *data, int nbr);
t_dongle	*create_dongle(int nbr);
void		init_coders(t_data	*data, t_coder **coders);
int			init_dongles(t_data *data);

// threads
void		thread_init(t_data **datas);

// prints
void		print_dgl(t_coder *c, char n_p);
void		print_log(t_coder *c, char *msg);

// stages
int			compilation(t_coder *c);
int			debugging(t_coder *c);

#endif
