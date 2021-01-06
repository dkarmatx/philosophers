/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philos_pool.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hganule@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/06 23:08:50 by hgranule          #+#    #+#             */
/*   Updated: 2021/01/06 23:35:31 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

t_time_ms			g_start_time;
struct s_atomicps	g_philo_status[PHILO_MAX_NUM];

void				philos_launch(struct s_atomicps statuses[],
									pthread_t philo_threads[],
									int philo_num)
{
	int				i;

	i = -1;
	while (++i < philo_num)
	{
		pthread_mutex_init(&statuses[i].deadline.guard, NULL);
		pthread_mutex_init(&statuses[i].is_exited.guard, NULL);
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
		while (++i < philo_num)
		{
			pthread_mutex_lock(&statuses[i].deadline.guard);
			if (statuses[i].deadline.val <= timer_now_ms())
			{
				logger_stop(i, "died");
				run = false;
				pthread_mutex_unlock(&statuses[i].deadline.guard);
				break ;
			}
			pthread_mutex_unlock(&statuses[i].deadline.guard);
			pthread_mutex_lock(&statuses[i].is_exited.guard);
			run |= !statuses[i].is_exited.val;
			pthread_mutex_unlock(&statuses[i].is_exited.guard);
		}
		usleep(PHILO_CHECK_DEATH_DELAY_US);
	}
}

void				philos_destroy(struct s_atomicps statuses[],
									pthread_t philo_threads[],
									int philo_num)
{
	int				i;

	i = -1;
	while (++i < philo_num)
	{
		pthread_mutex_lock(&statuses[i].is_exited.guard);
		statuses[i].is_exited.val = true;
		pthread_mutex_unlock(&statuses[i].is_exited.guard);
	}
	i = -1;
	while (++i < philo_num)
	{
		pthread_join(philo_threads[i], NULL);
	}
	i = -1;
	while (++i < philo_num)
	{
		pthread_mutex_destroy(&statuses[i].deadline.guard);
		pthread_mutex_destroy(&statuses[i].is_exited.guard);
	}
}
