/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hganule@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/06 23:28:10 by hgranule          #+#    #+#             */
/*   Updated: 2021/01/08 22:04:39 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <signal.h>

#include "logger.h"
#include "philo.h"

enum e_philo_status
{
	PHILO_ALIVE = 0,
	PHILO_DEAD = 1,
	PHILO_UNKNOWN = 2,
	PHILO_FEDUP = 3,
};

struct s_atomictime	g_deadline;
t_time_ms			g_start_time;

t_time_ms			atomic_time_load(struct s_atomictime *atomictime)
{
	t_time_ms		val;

	sem_wait(atomictime->guard);
	val = atomictime->val;
	sem_post(atomictime->guard);
	return (val);
}

void				atomic_time_set(struct s_atomictime *atomictime, t_time_ms val)
{
	sem_wait(atomictime->guard);
	atomictime->val = val;
	sem_post(atomictime->guard);
}

t_no_return			philo_async_death_check(void *data)
{
	const t_philo_id		pid = (t_philo_id)(long)data;

	while (true)
	{
		if (timer_now_ms() >= atomic_time_load(&g_deadline))
			break ;
		usleep(PHILO_CHECK_DEATH_DELAY_US);
	}
	logger_stop(pid, "died");
	sem_close(g_deadline.guard);
	exit(EXIT_FAILURE);
	return (NULL);
}

static t_no_return	fed_up_exit(t_philo_id pid)
{
	atomic_time_set(&g_deadline, timer_now_ms() + 100000000);
	sem_close(g_deadline.guard);
	logger_msg(pid, "is fed up");
	exit(EXIT_SUCCESS);
	return (NULL);
}

t_no_return			philo_worker(t_philo_id pid)
{
	int							meal_count;

	timer_wait_until_ms(g_start_time);
	meal_count = 0;
	while (true)
	{
		logger_msg(pid, "is thinking");
		philo_take_forks(pid, g_forks);
		logger_msg(pid, "is eating");
		atomic_time_set(&g_deadline, timer_now_ms() + g_conf.ttd + 1);
		timer_wait_until_ms(timer_now_ms() + g_conf.tte);
		philo_put_forks(g_forks);
		if (g_conf.meal_count > 0 && ++meal_count >= g_conf.meal_count)
			return (fed_up_exit(pid));
		logger_msg(pid, "is sleeping");
		timer_wait_until_ms(timer_now_ms() + g_conf.tts);
	}
	return (NULL);
}

static void			sem_name_set(const char *base,
									t_philo_id pid, char *buf)
{
	const int	base_len = ft_strlen(base);

	memset(buf, 0, PHILO_SEM_NAME_BUF_SZ);
	ft_memcpy(buf, base, base_len);
	ft_itoa_buf((unsigned)pid, 4, '0', buf + base_len);
}

void				philo_launch(pid_t philo_pids[], int philo_num)
{
	t_philo_id		i;
	pthread_t		tid;
	pid_t			pid;
	char			sem_name_buf[PHILO_SEM_NAME_BUF_SZ];

	i = -1;
	while (++i < philo_num)
	{
		pid = fork();
		if (pid == 0)
		{
			g_deadline.val = g_start_time + g_conf.ttd + 1;
			sem_name_set(PHILO_SEM_DEADLINE_NAME, i, sem_name_buf);
			g_deadline.guard = sem_new(sem_name_buf, 1);
			pthread_create(&tid, NULL, philo_async_death_check, (void *)(long)i);
			pthread_detach(tid);
			philo_worker(i);
		}
		else
			philo_pids[i] = pid;
	}
}

enum e_philo_status	philo_call_me_maybe(pid_t pid)
{
	int		status;

	if (0 == waitpid(pid, &status, WNOHANG | WUNTRACED))
		return (PHILO_ALIVE);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status) == EXIT_SUCCESS ? PHILO_FEDUP : PHILO_DEAD);
	return (PHILO_UNKNOWN);
}

void				philo_destroy(pid_t philo_pids[], int philo_num)
{
	t_philo_id		id;
	char			sem_name_buf[PHILO_SEM_NAME_BUF_SZ];

	id = -1;
	while (++id < philo_num)
	{
		sem_name_set(PHILO_SEM_DEADLINE_NAME, id, sem_name_buf);
		sem_unlink(sem_name_buf);
		kill(philo_pids[id], SIGKILL);
	}
}

void				philo_wait(pid_t philo_pids[], int philo_num)
{
	bool				run;
	t_philo_id			id;
	bool				philo_exited[PHILO_MAX_NUM];
	enum e_philo_status	philo_status;

	run = true;
	memset(philo_exited, false, sizeof(philo_exited));
	while (run)
	{
		id = -1;
		while (++id < philo_num)
		{
			if (!philo_exited[id])
			{
				philo_status = philo_call_me_maybe(philo_pids[id]);
				if (philo_status == PHILO_ALIVE)
					continue ;
				else if (philo_status == PHILO_FEDUP)
					philo_exited[id] = true;
				else
					break ;
			}
			// TODO THEY ARE NOT STOPPING AFTER THEY FED UP OR DIED
			run = run || !philo_exited[id];
		}
	}
	
}

// TODO GLOBAL CLEANUP

int					main(int ac, const char *av[])
{
	pid_t			philo_pids[PHILO_MAX_NUM];

	if (!parse_args(ac, av, &g_conf))
		return (EXIT_FAILURE);
	forks_init(&g_forks, g_conf.philo_num);
	g_start_time = timer_now_ms() + PHILO_WORKERS_START_DELAY_MS;
	logger_init(g_start_time);
	philo_launch(philo_pids, g_conf.philo_num);
	philo_wait(philo_pids, g_conf.philo_num);
	philo_destroy(philo_pids, g_conf.philo_num);
	logger_destroy();
	forks_destroy(&g_forks, g_conf.philo_num);
	return (EXIT_SUCCESS);
}
