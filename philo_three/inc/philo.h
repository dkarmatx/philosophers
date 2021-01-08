/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hganule@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/06 22:59:41 by hgranule          #+#    #+#             */
/*   Updated: 2021/01/08 21:05:58 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>

# include "sems.h"
# include "utils.h"
# include "timer.h"
# include "logger.h"

# define PHILO_SEM_FORKS_NAME "philo_two_sem_forks"
# define PHILO_SEM_DEADLINE_NAME "philo_two_sem_deadline"
# define PHILO_SEM_EXITED_NAME "philo_two_sem_exited"
# define PHILO_SEM_NAME_BUF_SZ 256

# define PHILO_WORKERS_START_DELAY_MS 1400
# define PHILO_MAX_NUM 200
# define PHILO_CHECK_DEATH_DELAY_US 100


typedef int					t_philo_id;

struct						s_conf
{
	int						philo_num;
	int						meal_count;
	t_time_ms				ttd;
	t_time_ms				tte;
	t_time_ms				tts;
};

enum						e_conf_args
{
	ARG_IDX_PHILO_NUM = 1,
	ARG_IDX_TIME_TO_DIE = 2,
	ARG_IDX_TIME_TO_EAT = 3,
	ARG_IDX_TIME_TO_SLEEP = 4,
	ARG_IDX_MEAL_COUNT = 5,

	CONF_NO_LIMIT_MEAL = 0,
};

typedef void				*t_no_return;

extern struct s_conf		g_conf;
extern t_time_ms			g_start_time;

struct						s_atomictime
{
	sem_t					*guard;
	t_time_ms				val;
};

extern struct s_atomictime	g_deadline;
extern sem_t				*g_forks;

bool						parse_args(int ac, const char *av[],
										struct s_conf *conf);

void						forks_init(sem_t **forks, int philo_num);
void						forks_destroy(sem_t **forks, int philo_num);

void						philo_take_forks(t_philo_id pid, sem_t *forks);

void						philo_put_forks(sem_t *forks);

#endif
