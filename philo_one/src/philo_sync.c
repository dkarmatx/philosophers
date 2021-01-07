/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_sync.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hganule@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/06 23:21:37 by hgranule          #+#    #+#             */
/*   Updated: 2021/01/07 21:50:13 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void				philo_put_forks(struct s_atomicfork *first,
									struct s_atomicfork *second)
{
	pthread_mutex_unlock(&second->guard);
	pthread_mutex_unlock(&first->guard);
}

void				philo_choose_forks(t_philo_id pid,
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

void				philo_take_forks(t_philo_id pid,
									struct s_atomicfork *first,
									struct s_atomicfork *second)
{
	pthread_mutex_lock(&first->guard);
	logger_msg(pid, "has taken a fork");
	pthread_mutex_lock(&second->guard);
	logger_msg(pid, "has taken a fork");
}
