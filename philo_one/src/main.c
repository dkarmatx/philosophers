#include <stdio.h>

#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "timer.h"
#include "logger.h"


volatile t_time_us	g_timer_us;
void				make_zamer() {g_timer_us = timer_now_us();}
void				put_zamer() {printf("%llu us\n", timer_now_us() - g_timer_us);}
void				put_n_make_zamer() {printf("%llu us\n", timer_now_us() - g_timer_us); g_timer_us = timer_now_us();}


# define PHILO_WORKERS_START_DELAY_MS 1000
# define PHILO_MAX_NUM 1000
# define PHILO_CHECK_DEATH_DELAY_US 100
# define PHILO_TRY_FORKS_DELAY_US 17

typedef int			t_philo_id;

struct				s_conf
{
	int				philo_num;
	int				meal_count;
	t_time_ms		ttd;
	t_time_ms		tte;
	t_time_ms		tts;
};

struct				s_atomicfork
{
	pthread_mutex_t	guard;
	bool			is_free;
};

struct				s_atomictime
{
	pthread_mutex_t	guard;
	t_time_ms		val;
};

struct				s_atomicbool
{
	pthread_mutex_t	guard;
	bool			val;
};

struct				s_atomicps
{
	struct s_atomictime deadline;
	struct s_atomicbool	is_exited;
};

enum				e_conf_args
{
	ARG_IDX_PHILO_NUM = 1,
	ARG_IDX_TIME_TO_DIE = 2,
	ARG_IDX_TIME_TO_EAT = 3,
	ARG_IDX_TIME_TO_SLEEP = 4,
	ARG_IDX_MEAL_COUNT = 5,

	CONF_NO_LIMIT_MEAL = 0,
};

bool				parse_args(int ac, const char *av[], struct s_conf *conf)
{
	if (ac <= ARG_IDX_TIME_TO_SLEEP || ac > ARG_IDX_MEAL_COUNT + 1)
		return (false);
	conf->philo_num = atoll(av[ARG_IDX_PHILO_NUM]);
	conf->ttd = atoll(av[ARG_IDX_TIME_TO_DIE]);
	conf->tte = atoll(av[ARG_IDX_TIME_TO_EAT]);
	conf->tts = atoll(av[ARG_IDX_TIME_TO_SLEEP]);
	conf->meal_count = (ac >= 6) ? atoll(av[ARG_IDX_MEAL_COUNT]) : CONF_NO_LIMIT_MEAL;
	return (true);
}

struct s_conf		g_conf;
t_time_ms			g_start_time;

struct s_atomicfork	g_forks[PHILO_MAX_NUM];
struct s_atomicps	g_philo_status[PHILO_MAX_NUM];

void				forks_init(struct s_atomicfork *forks, int philo_num)
{
	int				i;

	i = -1;
	while (++i < philo_num)
	{
		pthread_mutex_init(&forks[i].guard, NULL);
		forks[i].is_free = true;
	}
}

void				forks_destroy(struct s_atomicfork *forks, int philo_num)
{
	int				i;

	i = -1;
	while (++i < philo_num)
		pthread_mutex_destroy(&forks[i].guard);
}

void				philo_take_forks(int pid,
									struct s_atomicfork *first,
									struct s_atomicfork *second)
{

	// ALGO 1
	// bool cont = true;
	// while (true)
	// {
	// 	pthread_mutex_lock(&first->guard);
	// 	if (first->is_free)
	// 	{
	// 		pthread_mutex_lock(&second->guard);
	// 		if (second->is_free)
	// 		{
	// 			logger_msg(pid, "has taken a fork");
	// 			logger_msg(pid, "has taken a fork");
	// 			first->is_free = false;
	// 			second->is_free = false;
	// 			cont = false;
	// 		}
	// 		pthread_mutex_unlock(&second->guard);
	// 	}
	// 	pthread_mutex_unlock(&first->guard);
	// 	if (cont)
	// 		usleep(PHILO_TRY_FORKS_DELAY_US);
	// 	else
	// 		break ;
	// }

	// ALGO 2
	// bool cont1 = true;
	// bool cont2 = true;
	// while (true)
	// {
	// 	if (cont1)
	// 	{
	// 		pthread_mutex_lock(&first->guard);
	// 		if (first->is_free)
	// 		{
	// 			cont1 = false;
	// 			logger_msg(pid, "has taken a fork");
	// 			first->is_free = false;
	// 		}
	// 		pthread_mutex_unlock(&first->guard);
	// 	}
	// 	if (cont2)
	// 	{
	// 		pthread_mutex_lock(&second->guard);
	// 		if (second->is_free)
	// 		{
	// 			cont2 = false;
	// 			logger_msg(pid, "has taken a fork");
	// 			second->is_free = false;
	// 		}
	// 		pthread_mutex_unlock(&second->guard);
	// 	}
	// 	if (cont1 || cont2)
	// 		usleep(PHILO_TRY_FORKS_DELAY_US);
	// 	else
	// 		break ;
	// }

	// ALGO 3
	bool cont;

	cont = true;
	while (true)
	{
		pthread_mutex_lock(&first->guard);
		if (first->is_free)
		{
			cont = false;
			logger_msg(pid, "has taken a fork");
			first->is_free = false;
		}
		pthread_mutex_unlock(&first->guard);
		if (cont)
			usleep(PHILO_TRY_FORKS_DELAY_US);
		else
			break ;
	}
	cont = true;
	while (true)
	{
		pthread_mutex_lock(&second->guard);
		if (second->is_free)
		{
			cont = false;
			logger_msg(pid, "has taken a fork");
			second->is_free = false;
		}
		pthread_mutex_unlock(&second->guard);
		if (cont)
			usleep(PHILO_TRY_FORKS_DELAY_US);
		else
			break ;
	}
}

void				philo_put_forks(int pid,
									struct s_atomicfork *first,
									struct s_atomicfork *second)
{
	pthread_mutex_lock(&first->guard);
	pthread_mutex_lock(&second->guard);
	first->is_free = true;
	second->is_free = true;
	logger_msg(pid, "is sleeping");
	pthread_mutex_unlock(&second->guard);
	pthread_mutex_unlock(&first->guard);
}

void				philo_choose_forks(int pid,
										struct s_atomicfork **first,
										struct s_atomicfork **second)
{
	if (pid == g_conf.philo_num - 1)
	{
		*first = g_forks + (pid + 1) % g_conf.philo_num;
		*second = g_forks + pid;
	}
	else
	{
		*first = g_forks + pid;
		*second = g_forks + (pid + 1) % g_conf.philo_num;
	}
}

void				*philo_worker(void *data)
{
	const int					pid = (int)data;
	struct s_atomicps *const	philo = &g_philo_status[pid];
	struct s_atomicfork			*first;
	struct s_atomicfork			*second;
	// t_time_ms					deadline;

	philo_choose_forks(pid, &first, &second);
	timer_wait_until_ms(g_start_time, -1);
	// usleep(50 * pid);
	while (true)
	{
		pthread_mutex_lock(&philo->is_exited.guard);
		if (philo->is_exited.val == true)
			return (NULL);
		pthread_mutex_unlock(&philo->is_exited.guard);
		logger_msg(pid, "is thinking");
		philo_take_forks(pid, first, second);
		pthread_mutex_lock(&philo->deadline.guard);
		philo->deadline.val = timer_now_ms() + g_conf.ttd + 1;
		// deadline = philo->deadline.val;
		logger_msg(pid, "is eating");
		pthread_mutex_unlock(&philo->deadline.guard);
		timer_wait_until_ms(timer_now_ms() + g_conf.tte, -1);
		philo_put_forks(pid, first, second);
		timer_wait_until_ms(timer_now_ms() + g_conf.tts, -1);
	}
	return (NULL);
}

void				philos_launch(void)
{
	pthread_t		tid;
	int				i;

	i = -1;
	while (++i < g_conf.philo_num)
	{
		pthread_mutex_init(&g_philo_status[i].deadline.guard, NULL);
		pthread_mutex_init(&g_philo_status[i].is_exited.guard, NULL);
		g_philo_status[i].is_exited.val = false;
		g_philo_status[i].deadline.val = g_start_time + g_conf.ttd + 1;
	}
	i = -1;
	while (++i < g_conf.philo_num)
	{
		pthread_create(&tid, NULL, philo_worker, (void *)(long)i);
		pthread_detach(tid);
	}
}

void				philos_checker(void)
{
	int				i;
	bool			run;

	run = true;
	while (run)
	{
		i = -1;
		while (++i < g_conf.philo_num)
		{
			pthread_mutex_lock(&g_philo_status[i].deadline.guard);
			if (g_philo_status[i].deadline.val <= timer_now_ms())
			{
				logger_stop(i, "died");
				run = false;
				pthread_mutex_unlock(&g_philo_status[i].deadline.guard);
				break ;
			}
			pthread_mutex_unlock(&g_philo_status[i].deadline.guard);
			pthread_mutex_lock(&g_philo_status[i].is_exited.guard);
			run |= !g_philo_status[i].is_exited.val;
			pthread_mutex_unlock(&g_philo_status[i].is_exited.guard);
		}
		usleep(PHILO_CHECK_DEATH_DELAY_US);
	}
	i = -1;
	while (++i)
	{
		pthread_mutex_lock(&g_philo_status[i].is_exited.guard);
		g_philo_status[i].is_exited.val = true;
		pthread_mutex_unlock(&g_philo_status[i].is_exited.guard);
	}
}

int					main(int ac, const char *av[])
{
	if (!parse_args(ac, av, &g_conf))
	{
		write(STDERR_FILENO, "Error\n", 6);
		return (EXIT_FAILURE);
	}
	forks_init(g_forks, g_conf.philo_num);
	g_start_time = timer_now_ms() + PHILO_WORKERS_START_DELAY_MS;
	logger_init(g_start_time);
	philos_launch();
	philos_checker();
	logger_destroy();
	forks_destroy(g_forks, g_conf.philo_num);
	return (EXIT_SUCCESS);
}
