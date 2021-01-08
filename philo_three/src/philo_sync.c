/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_sync.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hganule@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/06 23:21:37 by hgranule          #+#    #+#             */
/*   Updated: 2021/01/08 17:09:02 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void				philo_put_forks(sem_t *forks)
{
	sem_post(forks);
}

void				philo_take_forks(t_philo_id pid, sem_t *forks)
{
	sem_wait(forks);
	logger_msg(pid, "has taken a fork");
	logger_msg(pid, "has taken a fork");
}
