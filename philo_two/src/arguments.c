/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arguments.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: hgranule <hganule@42.fr>                   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/06 23:14:29 by hgranule          #+#    #+#             */
/*   Updated: 2021/01/08 15:33:59 by hgranule         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"
#include <stdlib.h>

struct s_conf		g_conf;

static bool			parse_fail(const char *msg)
{
	const char	*prefix = "Error: ";

	write(STDERR_FILENO, prefix, ft_strlen(prefix));
	write(STDERR_FILENO, msg, ft_strlen(msg));
	write(STDERR_FILENO, "\n", 1);
	return (false);
}

static bool			ft_is_digit(int sym)
{
	return (sym >= '0' && sym <= '9');
}

static unsigned		parse_uint(const char *src, bool *fail)
{
	unsigned		uint_num;
	int				i;
	char			sym;

	uint_num = 0;
	i = -1;
	while ((sym = src[++i]))
	{
		if (!ft_is_digit(sym))
			*fail = true;
		uint_num *= 10;
		uint_num += (unsigned)(sym - '0');
	}
	return (uint_num);
}

bool				parse_args(int ac, const char *av[], struct s_conf *conf)
{
	bool	failure;

	failure = false;
	if (ac <= ARG_IDX_TIME_TO_SLEEP || ac > ARG_IDX_MEAL_COUNT + 1)
		return (parse_fail("Wrong number of arguments"));
	conf->philo_num = parse_uint(av[ARG_IDX_PHILO_NUM], &failure);
	conf->ttd = parse_uint(av[ARG_IDX_TIME_TO_DIE], &failure);
	conf->tte = parse_uint(av[ARG_IDX_TIME_TO_EAT], &failure);
	conf->tts = parse_uint(av[ARG_IDX_TIME_TO_SLEEP], &failure);
	conf->meal_count = (ac >= 6)
		? parse_uint(av[ARG_IDX_MEAL_COUNT], &failure)
		: CONF_NO_LIMIT_MEAL;
	if (failure)
		return (parse_fail("Argument isn't a unsigned number"));
	if (conf->ttd < 60 || conf->tte < 60 || conf->tts < 60)
		return (parse_fail("Delay must be more then 60 miliseconds"));
	if (conf->philo_num < 2 || conf->philo_num > 200)
		return (parse_fail("Number of philosophers must at interval: [2:200]"));
	return (true);
}
