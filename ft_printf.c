/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itanisma <itanisma@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 19:30:00 by itanisma          #+#    #+#             */
/*   Updated: 2026/05/09 04:03:17 by itanisma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

static int	parser(char c, va_list args)
{
	if (c == 'd' || c == 'i')
		return (ft_putnbr(va_arg(args, int)));
	else if (c == 'u')
		return (ft_putnbr(va_arg(args, unsigned int)));
	else if (c == 'c')
		return (ft_putchar_fd(va_arg(args, int), 1));
	else if (c == 's')
		return (ft_putstr_fd(va_arg(args, char *), 1));
	else if (c == 'p')
		return (ft_print_pointer(va_arg(args, void *)));
	else if (c == 'x')
		return (ft_puthex(va_arg(args, unsigned int), "0123456789abcdef"));
	else if (c == 'X')
		return (ft_puthex(va_arg(args, unsigned int), "0123456789ABCDEF"));
	else if (c == '%')
		return (ft_putchar_fd('%', 1));
	else
	{
		ft_putchar_fd('%', 1);
		ft_putchar_fd(c, 1);
		return (2);
	}
	return (0);
}

int	ft_printf(const char *format, ...)
{
	va_list	args;
	int		i;
	int		count;

	if (!format)
		return (-1);
	va_start(args, format);
	i = 0;
	count = 0;
	while (format[i])
	{
		if (format[i] == '%')
		{
			if (format[++i] == '\0')
			{
				va_end(args);
				return (-1);
			}
			count += parser(format[i++], args);
		}
		else
			count += ft_putchar_fd(format[i++], 1);
	}
	va_end(args);
	return (count);
}
