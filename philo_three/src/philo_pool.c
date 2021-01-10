/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_pool.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hganule@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/10 18:17:18 by hgranule          #+#    #+#             */
/*   Updated: 2021/01/10 18:24:29 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <signal.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

#include "philo.h"

void						philo_launch(pid_t philo_pids[], int philo_num)
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
			pthread_create(&tid, NULL,
				philo_async_death_check, (void *)(long)i);
			pthread_detach(tid);
			philo_worker(i);
		}
		else
			philo_pids[i] = pid;
	}
}

static enum e_philo_status	philo_call_me_maybe(pid_t pid)
{
	int		status;

	if (0 == waitpid(pid, &status, WNOHANG))
		return (PHILO_ALIVE);
	if (WIFEXITED(status))
		return (WEXITSTATUS(status) == EXIT_SUCCESS ? PHILO_FEDUP : PHILO_DEAD);
	return (PHILO_UNKNOWN);
}

void						philo_destroy(pid_t philo_pids[], int philo_num)
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

void						philo_wait(pid_t philo_pids[], int philo_num)
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
		run = false;
		while (++id < philo_num)
		{
			if (!philo_exited[id])
			{
				philo_status = philo_call_me_maybe(philo_pids[id]);
				if (philo_status == PHILO_FEDUP)
					philo_exited[id] = true;
				else if (philo_status != PHILO_ALIVE)
					return ;
			}
			run = run || !philo_exited[id];
			usleep(PHILO_CHECK_DEATH_DELAY_US);
		}
	}
}
