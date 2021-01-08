/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hganule@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/23 21:31:03 by hgranule          #+#    #+#             */
/*   Updated: 2021/01/08 15:55:53 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGER_H
# define LOGGER_H

# include "sems.h"
# include "utils.h"
# include "timer.h"

# define LOGGER_TIME_FIELD 5
# define LOGGER_MSG_BUFF 256
# define LOGGER_CHECK_FULL_DELAY_US 100
# define LOGGER_WRITE_DELAY_US 100

# define LOGGER_SEM_Q_PUSHER_NAME "philo_two_log_sem"
# define LOGGER_SEM_ATOMIC_NAME "philo_two_log_atomic_sem"

void		logger_destroy();
void		logger_init();
void		logger_stop();
void		logger_msg(int philo_idx, const char *msg);
void		logger_write(int philo_id, t_time_ms stamp, const char *msg);

#endif
