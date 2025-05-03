/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   201_tkns_handlers.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/21 23:11:56 by mde-agui          #+#    #+#             */
/*   Updated: 2025/01/06 19:23:25 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/**
 * @brief Handles input redirection tokens in the input string.
 *
 * Detects whether it's a single ('<') or double ('<<') input redirection
 * and delegates to the appropriate handler.
 *
 * @param input The raw input string.
 * @param sp Pointer to the token operation structure (tracking state and storage).
 * @param msh Pointer to the main shell state structure.
 */
void	handle_input_redir(const char *input, t_tkn_op *sp, t_msh *msh)
{
	if (input[sp->i] == '<' && input[sp->i + 1] == '<')
		handle_double_in_redir(input, sp);
	else if (input[sp->i] == '<')
		handle_single_in_redir(input, sp, msh);
}

/**
 * @brief Handles output redirection tokens in the input string.
 *
 * Differentiates between single ('>') and double ('>>') output redirections
 * and calls the appropriate handler function.
 *
 * @param input The raw input string.
 * @param sp Pointer to the token operation structure.
 * @param msh Pointer to the main shell state structure.
 */
void	handle_output_redir(const char *input, t_tkn_op *sp, t_msh *msh)
{
	if (input[sp->i] == '>' && input[sp->i + 1] == '>')
		handle_double_out_redir(input, sp);
	else if (input[sp->i] == '>')
		handle_single_out_redir(input, sp, msh);
}

/**
 * @brief Handles pipe ('|') tokens, with consideration for surrounding quotes.
 *
 * Adds the pipe token to the token array, handling the case where the pipe
 * is enclosed in quotes (e.g., `"|"`) as a literal string.
 *
 * @param input The raw input string.
 * @param sp Pointer to the token operation structure.
 */
void	handle_pipes(const char *input, t_tkn_op *sp)
{
	if (sp->i > 0 && (input[sp->i - 1] == '"' || input[sp->i + 1] == '"'))
		sp->argv[(sp->j)++] = ft_strdup("\"|\"");
	else
		sp->argv[(sp->j)++] = ft_strdup("|");
	(sp->i)++;
}

/**
 * @brief Handles environment variable expansion (e.g., `$USER`).
 *
 * Determines whether the `$` symbol should be expanded or treated literally.
 * If expandable, retrieves the variable value and adds it to the token array.
 *
 * @param input The raw input string.
 * @param sp Pointer to the token operation structure.
 * @param msh Pointer to the main shell state structure (for accessing env).
 */
void	handle_expand(const char *input, t_tkn_op *sp, t_msh *msh)
{
	char	*expanded;
	bool	is_first_word;

	is_first_word = (sp->j == 0);
	if (input[sp->i + 1] == '\0' || input[sp->i + 1] == '\"')
	{
		sp->argv[(sp->j)++] = ft_strdup("$");
		(sp->i)++;
	}
	else
	{
		expanded = expand_var(input, &sp->i, msh);
		if (expanded)
		{
			if (!(is_first_word && expanded[0] == '\0'))
				sp->argv[(sp->j)++] = ft_strdup(expanded);
			free(expanded);
		}
	}
}

/**
 * @brief Handles quoted strings in the input, both single and double quotes.
 *
 * Delegates to appropriate quote handlers and adds the resulting string to the token array.
 * Handles empty quotes and edge cases where quotes appear at the start of the input.
 *
 * @param input The raw input string.
 * @param sp Pointer to the token operation structure.
 * @param msh Pointer to the main shell state structure.
 */
void	handle_quotes(const char *input, t_tkn_op *sp, t_msh *msh)
{
	char	*word;

	if (input[sp->i] == '\'')
		sp->argv[sp->j++] = handle_single_quotes(input, &sp->i);
	else if (input[sp->i] == '"')
	{
		sp->start = sp->i;
		if (input[++sp->i] == '"')
		{
			sp->argv[sp->j++] = ft_strdup("");
			sp->i++;
		}
		else
		{
			word = handle_double_quotes(input, &sp->i, msh);
			if (word && word[0] != '\0')
				sp->argv[sp->j++] = word;
			else
			{
				free(word);
				if (sp->start == 0)
					sp->argv[sp->j++] = ft_strdup("");
			}
		}
	}
}
