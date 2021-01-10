/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_worker.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hganule@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/10 18:13:06 by hgranule          #+#    #+#             */
/*   Updated: 2021/01/10 18:16:36 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>

static t_time_ms	atomic_time_load(struct s_atomictime *atomictime)
{
	t_time_ms		val;

	sem_wait(atomictime->guard);
	val = atomictime->val;
	sem_post(atomictime->guard);
	return (val);
}

static void			atomic_time_set(struct s_atomictime *atomictime,
								t_time_ms val)
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
