/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_worker.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hganule@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/06 23:32:21 by hgranule          #+#    #+#             */
/*   Updated: 2021/01/07 21:54:24 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool			atomic_bool_load(struct s_atomicbool *atomicbool)
{
	bool			result;

	pthread_mutex_lock(&atomicbool->guard);
	result = atomicbool->val;
	pthread_mutex_unlock(&atomicbool->guard);
	return (result);
}

static void			atomic_time_set(struct s_atomictime *atomictime,
								t_time_ms val)
{
	pthread_mutex_lock(&atomictime->guard);
	atomictime->val = val;
	pthread_mutex_unlock(&atomictime->guard);
}

static void			*fed_up_exit(t_philo_id pid,
								struct s_atomicps *philo)
{
	atomic_time_set(&philo->deadline, timer_now_ms() + 100000000);
	pthread_mutex_lock(&philo->is_exited.guard);
	philo->is_exited.val = true;
	pthread_mutex_unlock(&philo->is_exited.guard);
	logger_msg(pid, "is fed up");
	return (NULL);
}

void				*philo_worker(void *data)
{
	const t_philo_id			pid = (t_philo_id)data;
	struct s_atomicps *const	philo = &g_philo_status[pid];
	struct s_atomicfork			*first;
	struct s_atomicfork			*second;
	int							meal_count;

	philo_choose_forks(pid, &first, &second);
	timer_wait_until_ms(g_start_time);
	if (pid % 2)
		usleep(PHILO_ODD_START_DELAY_US);
	meal_count = 0;
	while (!atomic_bool_load(&philo->is_exited))
	{
		logger_msg(pid, "is thinking");
		philo_take_forks(pid, first, second);
		logger_msg(pid, "is eating");
		atomic_time_set(&philo->deadline, timer_now_ms() + g_conf.ttd + 1);
		timer_wait_until_ms(timer_now_ms() + g_conf.tte);
		philo_put_forks(first, second);
		if (g_conf.meal_count > 0 && ++meal_count >= g_conf.meal_count)
			return (fed_up_exit(pid, philo));
		logger_msg(pid, "is sleeping");
		timer_wait_until_ms(timer_now_ms() + g_conf.tts);
	}
	return (NULL);
}
