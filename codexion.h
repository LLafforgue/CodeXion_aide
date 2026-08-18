/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   codexion.h                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: llafforg <llafforg@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/08/18 15:07:06 by llafforg          #+#    #+#             */
/*   Updated: 2026/08/18 16:30:13 by llafforg         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

typedef struct s_dongle
{
	int	id;
	int	is_available;
}	t_dongle;

typedef struct s_coder
{
	int			id;
	int			t_burnout;
	t_dongle	*dongles;
	int			stage;
	int			nbr_compile;
	t_coder		*prev;
	t_coder		*next;
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
	t_coder		**coders;
	t_dongle	**dongles;
}	t_data;