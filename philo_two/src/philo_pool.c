/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_pool.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hganule@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/06 23:08:50 by hgranule          #+#    #+#             */
/*   Updated: 2021/01/08 16:12:28 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <string.h>

t_time_ms			g_start_time;
struct s_atomicps	g_philo_status[PHILO_MAX_NUM];

static void			sem_name_set(const char *base,
									t_philo_id pid, char *buf)
{
	const int	base_len = ft_strlen(base);

	memset(buf, 0, PHILO_SEM_NAME_BUF_SZ);
	ft_memcpy(buf, base, base_len);
	ft_itoa_buf((unsigned)pid, 4, '0', buf + base_len);
}

void				philos_launch(struct s_atomicps statuses[],
									pthread_t philo_threads[],
									int philo_num)
{
	int				i;
	char			sem_name_buffer[PHILO_SEM_NAME_BUF_SZ];

	i = -1;
	while (++i < philo_num)
	{
		sem_name_set(PHILO_SEM_EXITED_NAME, i, sem_name_buffer);
		statuses[i].is_exited.guard = sem_new(sem_name_buffer, 1);
		sem_name_set(PHILO_SEM_DEADLINE_NAME, i, sem_name_buffer);
		statuses[i].deadline.guard = sem_new(sem_name_buffer, 1);
		statuses[i].is_exited.val = false;
		statuses[i].deadline.val = g_start_time + g_conf.ttd + 1;
	}
	i = -1;
	while (++i < philo_num)
		pthread_create(&philo_threads[i], NULL, philo_worker, (void *)(long)i);
}

void				philos_checker(struct s_atomicps statuses[],
									int philo_num)
{
	int				i;
	bool			run;

	run = true;
	while (run)
	{
		i = -1;
		run = false;
		while (++i < philo_num)
		{
			sem_wait(statuses[i].deadline.guard);
			if (statuses[i].deadline.val <= timer_now_ms())
			{
				logger_stop(i, "died");
				run = false;
				sem_post(statuses[i].deadline.guard);
				break ;
			}
			sem_post(statuses[i].deadline.guard);
			sem_wait(statuses[i].is_exited.guard);
			run = run || !statuses[i].is_exited.val;
			sem_post(statuses[i].is_exited.guard);
		}
		usleep(PHILO_CHECK_DEATH_DELAY_US);
	}
}

void				philos_destroy(struct s_atomicps statuses[],
									pthread_t philo_threads[],
									int philo_num)
{
	int				i;
	char			sem_name_buffer[PHILO_SEM_NAME_BUF_SZ];

	i = -1;
	while (++i < philo_num)
	{
		sem_wait(statuses[i].is_exited.guard);
		statuses[i].is_exited.val = true;
		sem_post(statuses[i].is_exited.guard);
	}
	i = -1;
	while (++i < philo_num)
	{
		pthread_join(philo_threads[i], NULL);
	}
	i = -1;
	while (++i < philo_num)
	{
		sem_name_set(PHILO_SEM_EXITED_NAME, i, sem_name_buffer);
		sem_unlink(sem_name_buffer);
		sem_name_set(PHILO_SEM_DEADLINE_NAME, i, sem_name_buffer);
		sem_unlink(sem_name_buffer);
	}
}
