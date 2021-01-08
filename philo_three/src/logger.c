/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hganule@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/23 21:36:33 by hgranule          #+#    #+#             */
/*   Updated: 2021/01/08 17:51:12 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "logger.h"
#include <unistd.h>

t_time_ms					g_log_start_ms;
sem_t						*g_log_q_pusher;

void						logger_stop(int philo_idx, const char *msg)
{
	sem_wait(g_log_q_pusher);
	logger_write(philo_idx + 1, timer_now_ms() - g_log_start_ms, msg);
}

void						logger_destroy(void)
{
	sem_unlink(LOGGER_SEM_Q_PUSHER_NAME);
}

void						logger_init(t_time_ms logger_start_ms)
{
	g_log_q_pusher = sem_new(LOGGER_SEM_Q_PUSHER_NAME, 1);
	g_log_start_ms = logger_start_ms;
}

void						logger_msg(int philo_idx, const char *msg)
{

	sem_wait(g_log_q_pusher);
	logger_write(philo_idx + 1, timer_now_ms() - g_log_start_ms, msg);
	sem_post(g_log_q_pusher);
}
