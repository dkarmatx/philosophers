/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   logger_write.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <wp3d3p@yandex.ru>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/23 23:46:47 by hgranule          #+#    #+#             */
/*   Updated: 2020/12/24 01:06:40 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "timer.h"
#include "logger.h"
#include <stdint.h>
#include <stddef.h>
#include <unistd.h>

static int	ft_count_digits(unsigned long num)
{
	int	count;

	count = 0;
	if (num == 0)
		return (1);
	while (num != 0)
	{
		++count;
		num /= 10;
	}
	return (count);
}

static int	ft_itoa_buf(unsigned long num, int field_len, char *buf)
{
	int		digits_num;
	int		len;
	int		i;

	digits_num = ft_count_digits(num);
	len = field_len > digits_num ? field_len : digits_num;
	i = len;
	buf[i] = '\0';
	while (--i >= 0)
	{
		if (digits_num > 0)
		{
			buf[i] = (char)(num % 10) + '0';
			num /= 10;
		}
		else
			buf[i] = ' ';
		--digits_num;
	}
	return (len);
}

static int	ft_strlen(const char *str)
{
	const char	*beg = str;

	while (*str)
		str++;
	return ((int)(str - beg));
}

static void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	const uint8_t	*src_ptr;
	uint8_t			*dst_ptr;

	dst_ptr = dst;
	src_ptr = src;
	while (n--)
		*dst_ptr++ = *src_ptr++;
	return (dst);
}

void		logger_write(int philo_id, t_time_ms stamp, const char *msg)
{
	const int	msg_len = ft_strlen(msg);
	char		buff[LOGGER_MSG_BUFF];
	int			i;

	i = 0;
	buff[i++] = '[';
	buff[i++] = ' ';
	i += ft_itoa_buf(stamp, LOGGER_TIME_FIELD, buff + i);
	buff[i++] = ' ';
	buff[i++] = 'm';
	buff[i++] = 's';
	buff[i++] = ' ';
	buff[i++] = ']';
	buff[i++] = ' ';
	buff[i++] = ' ';
	i += ft_itoa_buf(philo_id, 0, buff + i);
	buff[i++] = ' ';
	ft_memcpy(buff + i, msg, msg_len);
	i += msg_len;
	buff[i++] = '\n';
	write(STDOUT_FILENO, buff, i);
}
