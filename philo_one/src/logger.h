/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <wp3d3p@yandex.ru>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/23 21:31:03 by hgranule          #+#    #+#             */
/*   Updated: 2020/12/24 01:02:27 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOGGER_H
# define LOGGER_H

# include "timer.h"

# define LOGGER_TIME_FIELD 5
# define LOGGER_MSG_BUFF 256

void		logger_destroy();
void		logger_init();
void		logger_stop();
void		logger_msg(int philo_idx, const char *msg);
void		logger_write(int philo_id, t_time_ms stamp, const char *msg);

#endif
