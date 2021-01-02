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
# define PHILO_CHECK_DEATH_DELAY 1000

typedef int			t_philo_id;

struct				s_conf
{
	int				philo_num;
	int				meal_count;
	t_time_ms		ttd;
	t_time_ms		tte;
	t_time_ms		tts;
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

pthread_mutex_t		g_forks[PHILO_MAX_NUM];
volatile t_time_ms	g_deadlines[PHILO_MAX_NUM] = {0};
volatile bool		g_exited[PHILO_MAX_NUM] = {0};

void				forks_init(pthread_mutex_t *forks, int philo_num)
{
	int				i;

	i = -1;
	while (++i < philo_num)
		pthread_mutex_init(forks + i, NULL);
}

void				forks_destroy(pthread_mutex_t *forks, int philo_num)
{
	int				i;

	i = -1;
	while (++i < philo_num)
		pthread_mutex_destroy(forks + i);
}

void				philo_take_forks(int pid)
{
	if (pid == g_conf.philo_num - 1)
	{
		pthread_mutex_lock(&g_forks[0]);
		logger_msg(pid, "has taken a lfork");
		pthread_mutex_lock(&g_forks[pid]);
		logger_msg(pid, "has taken a rfork");
	}
	else
	{
		pthread_mutex_lock(&g_forks[pid]);
		logger_msg(pid, "has taken a rfork");
		pthread_mutex_lock(&g_forks[pid + 1]);
		logger_msg(pid, "has taken a lfork");
	}
}

void				philo_put_forks(int pid)
{
	pthread_mutex_unlock(&g_forks[pid]);
	pthread_mutex_unlock(&g_forks[(pid + 1) % g_conf.philo_num]);
}

void				*philo_worker(void *data)
{
	const int		pid = (int)data;

	timer_wait_until_ms(g_start_time, -1);
	while (!g_exited[pid])
	{
		logger_msg(pid, "is thinking");
		philo_take_forks(pid);
		g_deadlines[pid] = timer_now_ms() + g_conf.ttd;
		logger_msg(pid, "is eating");
		timer_wait_until_ms(timer_now_ms() + g_conf.tte, -1);
		philo_put_forks(pid);
		logger_msg(pid, "is sleeping");
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
		g_deadlines[i] = g_start_time + g_conf.ttd;
	i = -1;
	while (++i < g_conf.philo_num)
		g_exited[i] = false;
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
			if (g_deadlines[i] <= timer_now_ms())
			{
				logger_msg(i, "died");
				logger_stop();
				run = false;
				break ;
			}
			run |= !g_exited[i];
		}
		usleep(50);
	}
	i = -1;
	while (++i)
		g_exited[i] = true;
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
