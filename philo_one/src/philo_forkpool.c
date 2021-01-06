/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_forkpool.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hganule@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/06 23:10:17 by hgranule          #+#    #+#             */
/*   Updated: 2021/01/06 23:12:22 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

struct s_atomicfork	g_forks[PHILO_MAX_NUM];

void				forks_init(struct s_atomicfork forks[], int philo_num)
{
	int				i;

	i = -1;
	while (++i < philo_num)
	{
		pthread_mutex_init(&forks[i].guard, NULL);
		forks[i].is_free = true;
	}
}

void				forks_destroy(struct s_atomicfork forks[], int philo_num)
{
	int				i;

	i = -1;
	while (++i < philo_num)
		pthread_mutex_destroy(&forks[i].guard);
}
