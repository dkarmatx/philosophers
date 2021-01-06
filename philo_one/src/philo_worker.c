/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_worker.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hganule@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/06 23:32:21 by hgranule          #+#    #+#             */
/*   Updated: 2021/01/06 23:51:18 by hgranule         ###   ########.fr       */
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

void				*philo_worker(void *data)
{
	const int					pid = (int)data;
	struct s_atomicps *const	philo = &g_philo_status[pid];
	struct s_atomicfork			*first;
	struct s_atomicfork			*second;
	t_time_ms					deadline;

	philo_choose_forks(pid, &first, &second);
	timer_wait_until_ms(g_start_time, -1);
	// WTF !? WE NEED TO FIX IT
	if (pid % 2)
		usleep(500);
	while (!atomic_bool_load(&philo->is_exited))
	{
		logger_msg(pid, "is thinking");
		philo_take_forks_ugly(pid, first, second);
		logger_msg(pid, "is eating");
		deadline = timer_now_ms() + g_conf.ttd + 1;
		atomic_time_set(&philo->deadline, deadline);
		timer_wait_until_ms(timer_now_ms() + g_conf.tte, -1);
		philo_put_forks(pid, first, second);
		timer_wait_until_ms(timer_now_ms() + g_conf.tts, -1);
	}
	return (NULL);
}
