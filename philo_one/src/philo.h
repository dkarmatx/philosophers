/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hganule@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/06 22:59:41 by hgranule          #+#    #+#             */
/*   Updated: 2021/01/06 23:50:19 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <pthread.h>
# include <unistd.h>
# include "timer.h"
# include "logger.h"

# define PHILO_WORKERS_START_DELAY_MS 1000
# define PHILO_MAX_NUM 1000
# define PHILO_CHECK_DEATH_DELAY_US 100
# define PHILO_TRY_FORKS_DELAY_US 17

typedef int					t_philo_id;

struct						s_conf
{
	int						philo_num;
	int						meal_count;
	t_time_ms				ttd;
	t_time_ms				tte;
	t_time_ms				tts;
};

struct						s_atomicfork
{
	pthread_mutex_t			guard;
	bool					is_free;
};

struct						s_atomictime
{
	pthread_mutex_t			guard;
	t_time_ms				val;
};

struct						s_atomicbool
{
	pthread_mutex_t			guard;
	bool					val;
};

struct						s_atomicps
{
	struct s_atomictime		deadline;
	struct s_atomicbool		is_exited;
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

extern struct s_conf		g_conf;
extern t_time_ms			g_start_time;

extern struct s_atomicfork	g_forks[PHILO_MAX_NUM];
extern struct s_atomicps	g_philo_status[PHILO_MAX_NUM];

bool						parse_args(int ac, const char *av[],
										struct s_conf *conf);

void						forks_init(struct s_atomicfork forks[],
										int philo_num);
void						forks_destroy(struct s_atomicfork forks[],
										int philo_num);

void						philos_launch(struct s_atomicps statuses[],
										pthread_t philo_threads[],
										int philo_num);
void						philos_checker(struct s_atomicps statuses[],
										int philo_num);
void						philos_destroy(struct s_atomicps statuses[],
										pthread_t philo_threads[],
										int philo_num);

void						*philo_worker(void *data);

void						philo_take_forks_good(int pid,
										struct s_atomicfork *first,
										struct s_atomicfork *second);
void						philo_take_forks_bad(int pid,
										struct s_atomicfork *first,
										struct s_atomicfork *second);
void						philo_take_forks_ugly(int pid,
										struct s_atomicfork *first,
										struct s_atomicfork *second);

void						philo_choose_forks(int pid,
										struct s_atomicfork **first,
										struct s_atomicfork **second);
void						philo_put_forks(int pid,
										struct s_atomicfork *first,
										struct s_atomicfork *second);

#endif
