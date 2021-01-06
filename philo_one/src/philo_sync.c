/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_sync.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hganule@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/06 23:21:37 by hgranule          #+#    #+#             */
/*   Updated: 2021/01/06 23:49:53 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void				philo_put_forks(int pid,
									struct s_atomicfork *first,
									struct s_atomicfork *second)
{
	pthread_mutex_lock(&first->guard);
	pthread_mutex_lock(&second->guard);
	first->is_free = true;
	second->is_free = true;
	logger_msg(pid, "is sleeping");
	pthread_mutex_unlock(&second->guard);
	pthread_mutex_unlock(&first->guard);
}

void				philo_choose_forks(int pid,
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

// SOME VARIANT NEED YOU TO CHOOSE

void				philo_take_forks_good(int pid,
									struct s_atomicfork *first,
									struct s_atomicfork *second)
{
	bool cont;

	cont = true;
	while (true)
	{
		pthread_mutex_lock(&first->guard);
		if (first->is_free)
		{
			cont = false;
			logger_msg(pid, "has taken a fork");
			first->is_free = false;
		}
		pthread_mutex_unlock(&first->guard);
		if (cont)
			usleep(PHILO_TRY_FORKS_DELAY_US);
		else
			break ;
	}
	cont = true;
	while (true)
	{
		pthread_mutex_lock(&second->guard);
		if (second->is_free)
		{
			cont = false;
			logger_msg(pid, "has taken a fork");
			second->is_free = false;
		}
		pthread_mutex_unlock(&second->guard);
		if (cont)
			usleep(PHILO_TRY_FORKS_DELAY_US);
		else
			break ;
	}
}

void				philo_take_forks_bad(int pid,
									struct s_atomicfork *first,
									struct s_atomicfork *second)
{
	bool cont1;
	bool cont2;

	cont1 = true;
	cont2 = true;
	while (true)
	{
		if (cont1)
		{
			pthread_mutex_lock(&first->guard);
			if (first->is_free)
			{
				cont1 = false;
				logger_msg(pid, "has taken a fork");
				first->is_free = false;
			}
			pthread_mutex_unlock(&first->guard);
		}
		if (cont2)
		{
			pthread_mutex_lock(&second->guard);
			if (second->is_free)
			{
				cont2 = false;
				logger_msg(pid, "has taken a fork");
				second->is_free = false;
			}
			pthread_mutex_unlock(&second->guard);
		}
		if (cont1 || cont2)
			usleep(PHILO_TRY_FORKS_DELAY_US);
		else
			break ;
	}
}

void				philo_take_forks_ugly(int pid,
									struct s_atomicfork *first,
									struct s_atomicfork *second)
{
	bool cont;

	cont = true;
	while (true)
	{
		pthread_mutex_lock(&first->guard);
		if (first->is_free)
		{
			pthread_mutex_lock(&second->guard);
			if (second->is_free)
			{
				logger_msg(pid, "has taken a fork");
				logger_msg(pid, "has taken a fork");
				first->is_free = false;
				second->is_free = false;
				cont = false;
			}
			pthread_mutex_unlock(&second->guard);
		}
		pthread_mutex_unlock(&first->guard);
		if (cont)
			usleep(PHILO_TRY_FORKS_DELAY_US);
		else
			break ;
	}
}
