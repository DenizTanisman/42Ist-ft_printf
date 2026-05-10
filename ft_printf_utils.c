/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: itanisma <itanisma@student.42istanbul.c    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/03/31 19:30:00 by itanisma          #+#    #+#             */
/*   Updated: 2026/04/04 13:25:27 by itanisma         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int	ft_putchar_fd(char c, int fd)
{
	write(fd, &c, 1);
	return (1);
}

int	ft_putstr_fd(char *s, int fd)
{
	int	count;

	if (s == NULL)
		s = "(null)";
	count = 0;
	while (*s)
	{
		count += ft_putchar_fd(*s, fd);
		s++;
	}
	return (count);
}

int	ft_putnbr(long n)
{
	int	count;

	count = 0;
	if (n < 0)
	{
		count += ft_putchar_fd('-', 1);
		n = -n;
	}
	if (n >= 10)
		count += ft_putnbr(n / 10);
	count += ft_putchar_fd((n % 10) + '0', 1);
	return (count);
}

int	ft_puthex(unsigned long n, char *base)
{
	int	count;

	count = 0;
	if (n >= 16)
		count += ft_puthex(n / 16, base);
	count += ft_putchar_fd(base[n % 16], 1);
	return (count);
}

int	ft_print_pointer(void *ptr)
{
	int	count;

	if (ptr == NULL)
		return (ft_putstr_fd("(nil)", 1));
	count = ft_putstr_fd("0x", 1);
	count += ft_puthex((unsigned long)ptr, "0123456789abcdef");
	return (count);
}
