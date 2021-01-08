/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger_write.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hganule@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/23 23:46:47 by hgranule          #+#    #+#             */
/*   Updated: 2021/01/08 15:37:07 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "timer.h"
#include "logger.h"
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>

void		logger_write(int philo_id, t_time_ms stamp, const char *msg)
{
	const int	msg_len = ft_strlen(msg);
	char		buff[LOGGER_MSG_BUFF];
	int			i;

	i = 0;
	buff[i++] = '[';
	buff[i++] = ' ';
	i += ft_itoa_buf(stamp, LOGGER_TIME_FIELD, ' ', buff + i);
	buff[i++] = ' ';
	buff[i++] = 'm';
	buff[i++] = 's';
	buff[i++] = ' ';
	buff[i++] = ']';
	buff[i++] = ' ';
	buff[i++] = ' ';
	i += ft_itoa_buf(philo_id, 0, ' ', buff + i);
	buff[i++] = ' ';
	ft_memcpy(buff + i, msg, msg_len);
	i += msg_len;
	buff[i++] = '\n';
	write(STDOUT_FILENO, buff, i);
}
