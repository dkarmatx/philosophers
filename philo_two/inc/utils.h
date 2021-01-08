/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hganule@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/08 15:33:11 by hgranule          #+#    #+#             */
/*   Updated: 2021/01/08 15:38:01 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

# include <stdbool.h>
# include <stddef.h>
# include <stdint.h>

int			ft_strlen(const char *str);
void		*ft_memcpy(void *dst, const void *src, size_t n);
int			ft_itoa_buf(unsigned long num, int field, char fill, char *buf);

#endif
