/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_worker.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hganule@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/06 23:32:21 by hgranule          #+#    #+#             */
/*   Updated: 2021/01/08 15:46:18 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static bool			atomic_bool_load(struct s_atomicbool *atomicbool)
{
	bool			result;

	sem_wait(atomicbool->guard);
	result = atomicbool->val;
	sem_post(atomicbool->guard);
	return (result);
}

static void			atomic_time_set(struct s_atomictime *atomictime,
								t_time_ms val)
{
	sem_wait(atomictime->guard);
	atomictime->val = val;
	sem_post(atomictime->guard);
}

static void			*fed_up_exit(t_philo_id pid,
								struct s_atomicps *philo)
{
	atomic_time_set(&philo->deadline, timer_now_ms() + 100000000);
	sem_wait(philo->is_exited.guard);
	philo->is_exited.val = true;
	sem_post(philo->is_exited.guard);
	logger_msg(pid, "is fed up");
	return (NULL);
}

void				*philo_worker(void *data)
{
	const t_philo_id			pid = (t_philo_id)data;
	struct s_atomicps *const	philo = &g_philo_status[pid];
	int							meal_count;

	timer_wait_until_ms(g_start_time);
	meal_count = 0;
	while (!atomic_bool_load(&philo->is_exited))
	{
		logger_msg(pid, "is thinking");
		philo_take_forks(pid, g_forks);
		logger_msg(pid, "is eating");
		atomic_time_set(&philo->deadline, timer_now_ms() + g_conf.ttd + 1);
		timer_wait_until_ms(timer_now_ms() + g_conf.tte);
		philo_put_forks(g_forks);
		if (g_conf.meal_count > 0 && ++meal_count >= g_conf.meal_count)
			return (fed_up_exit(pid, philo));
		logger_msg(pid, "is sleeping");
		timer_wait_until_ms(timer_now_ms() + g_conf.tts);
	}
	return (NULL);
}
