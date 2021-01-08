/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hganule@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/23 21:36:33 by hgranule          #+#    #+#             */
/*   Updated: 2021/01/08 16:11:35 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "log_q.h"
#include "logger.h"
#include <unistd.h>

static volatile t_log_q		g_queue;
static volatile t_time_ms	g_log_start_ms;
static volatile bool		g_log_run;
static pthread_t			g_logger_thread;
static t_log_item			g_queue_data[LOG_QUEUE_CAPACITY];

static sem_t				*g_log_q_pusher;
sem_t						*g_atomic;

static void					*logger_worker(void *nullable)
{
	t_log_item		item;

	(void)nullable;
	while (g_log_run || !log_q_isempty((t_log_q*)&g_queue))
	{
		if (!log_q_isempty((t_log_q*)&g_queue))
		{
			item = *log_q_head((t_log_q*)&g_queue);
			logger_write(item.philo_id
						, item.time - g_log_start_ms
						, item.message);
			log_q_pop((t_log_q*)&g_queue);
		}
		else
			usleep(LOGGER_WRITE_DELAY_US);
	}
	return (NULL);
}

void						logger_stop(int philo_idx, const char *msg)
{
	t_log_item	item;

	sem_wait(g_log_q_pusher);
	item.time = timer_now_ms();
	item.philo_id = philo_idx + 1;
	item.message = msg;
	if (g_log_run)
	{
		while (log_q_isfull((t_log_q*)&g_queue))
			usleep(LOGGER_CHECK_FULL_DELAY_US);
		log_q_push((t_log_q*)&g_queue, item);
	}
	g_log_run = false;
	sem_post(g_log_q_pusher);
}

void						logger_destroy(void)
{
	g_log_run = false;
	sem_unlink(LOGGER_SEM_Q_PUSHER_NAME);
	sem_unlink(LOGGER_SEM_ATOMIC_NAME);
	pthread_join(g_logger_thread, NULL);
}

void						logger_init(t_time_ms logger_start_ms)
{
	g_queue.data = g_queue_data;
	g_queue.size = LOG_QUEUE_CAPACITY;
	g_queue.len = 0;
	g_queue.h = 0;
	g_queue.t = 0;
	g_log_run = true;
	g_log_q_pusher = sem_new(LOGGER_SEM_Q_PUSHER_NAME, 1);
	g_atomic = sem_new(LOGGER_SEM_ATOMIC_NAME, 1);
	g_log_start_ms = logger_start_ms;
	pthread_create(&g_logger_thread, NULL, logger_worker, NULL);
}

void						logger_msg(int philo_idx, const char *msg)
{
	t_log_item	item;

	sem_wait(g_log_q_pusher);
	item.time = timer_now_ms();
	item.philo_id = philo_idx + 1;
	item.message = msg;
	if (g_log_run)
	{
		while (log_q_isfull((t_log_q*)&g_queue))
			usleep(LOGGER_CHECK_FULL_DELAY_US);
		log_q_push((t_log_q*)&g_queue, item);
	}
	sem_post(g_log_q_pusher);
}
