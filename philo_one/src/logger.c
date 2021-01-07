/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hganule@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/23 21:36:33 by hgranule          #+#    #+#             */
/*   Updated: 2021/01/07 17:52:48 by hgranule         ###   ########.fr       */
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
static pthread_mutex_t		g_log_q_pusher;
pthread_mutex_t				g_atomic;

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

	pthread_mutex_lock(&g_log_q_pusher);
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
	pthread_mutex_unlock(&g_log_q_pusher);
}

void						logger_destroy(void)
{
	g_log_run = false;
	pthread_mutex_destroy(&g_log_q_pusher);
	pthread_mutex_destroy(&g_atomic);
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
	pthread_mutex_init(&g_log_q_pusher, NULL);
	pthread_mutex_init(&g_atomic, NULL);
	g_log_start_ms = logger_start_ms;
	pthread_create(&g_logger_thread, NULL, logger_worker, NULL);
}

void						logger_msg(int philo_idx, const char *msg)
{
	t_log_item	item;

	pthread_mutex_lock(&g_log_q_pusher);
	item.time = timer_now_ms();
	item.philo_id = philo_idx + 1;
	item.message = msg;
	if (g_log_run)
	{
		while (log_q_isfull((t_log_q*)&g_queue))
			usleep(LOGGER_CHECK_FULL_DELAY_US);
		log_q_push((t_log_q*)&g_queue, item);
	}
	pthread_mutex_unlock(&g_log_q_pusher);
}
