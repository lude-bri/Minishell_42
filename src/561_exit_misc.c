/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   561_exit_misc.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/23 01:38:29 by mde-agui          #+#    #+#             */
/*   Updated: 2024/12/23 01:41:15 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Checks if a string is a valid number (integer format).
 *
 * Accepts optional '+' or '-' signs at the beginning.
 *
 * @param str The input string to validate.
 * @return 1 if it's a valid number; 0 otherwise.
 */
int	is_num(const char *str)
{
	int	i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
			return (0);
		i++;
	}
	return (1);
}

/**
 * @brief Checks if the string contains at least one alphabetical character.
 *
 * Used to detect invalid arguments like `"exit abc"` which are not numeric.
 *
 * @param str The string to check.
 * @return 1 if it contains letters; 0 otherwise.
 */
int	is_letter(const char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (ft_isalpha(str[i]))
			return (1);
		i++;
	}
	return (0);
}

/**
 * @brief Checks if the argument is a standalone `+` or `-` sign.
 *
 * Useful for validating edge cases such as `exit +` or `exit -`.
 *
 * @param argv The argument to check.
 * @return 1 if it is '+' or '-'; 0 otherwise.
 */
int	is_sign(char *argv)
{
	return ((ft_strncmp(argv, "+", 1) == 0) || (ft_strncmp(argv, "-", 1) == 0));
}
