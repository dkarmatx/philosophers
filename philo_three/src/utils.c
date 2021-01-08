/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hganule@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/08 15:32:43 by hgranule          #+#    #+#             */
/*   Updated: 2021/01/08 15:36:40 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

int			ft_strlen(const char *str_beg)
{
	const char	*str_end = str_beg;

	while (*str_end != '\0')
		++str_end;
	return (int)(str_end - str_beg);
}

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

int			ft_itoa_buf(unsigned long num, int field_len,
						char filler, char *buf)
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
			buf[i] = filler;
		--digits_num;
	}
	return (len);
}

void		*ft_memcpy(void *dst, const void *src, size_t n)
{
	const uint8_t	*src_ptr;
	uint8_t			*dst_ptr;

	dst_ptr = dst;
	src_ptr = src;
	while (n--)
		*dst_ptr++ = *src_ptr++;
	return (dst);
}
