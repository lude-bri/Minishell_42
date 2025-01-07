/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   200_tokenization.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mde-agui <mde-agui@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 12:49:13 by luigi             #+#    #+#             */
/*   Updated: 2025/01/07 16:45:25 by mde-agui         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*copy_word(const char *input, int start, int end)
{
	int		i;
	int		len;
	char	*word;

	len = end - start;
	word = malloc(sizeof(char) * (len + 1));
	if (!word)
		return (NULL);
	i = 0;
	while (i < len)
	{
		word[i] = input[start + i];
		i++;
	}
	word[len] = '\0';
	return (word);
}

int	count_words(const char *input)
{
	int	i;
	int	counter;

	i = 0;
	counter = 0;
	while (input[i])
	{
		while (input[i] && is_whitespace(input[i]))
			i++;
		if (input[i] == '\'' || input[i] == '"')
		{
			i = quote_helper(input, i);
			counter++;
		}
		else if (special_char_helper(input, &i))
			counter++;
		else if (regular_char_helper(input, &i))
			counter++;
	}
	return (counter);
}

void	process_tokens(const char *input, t_tkn_op *sp, t_msh *msh)
{
	while (input[sp->i])
	{
		while (input[sp->i] && is_whitespace(input[sp->i]))
			sp->i++;
		if (input[sp->i] == '\'' || input[sp->i] == '"')
			handle_quotes(input, sp, msh);
		else if (input[sp->i] == '$')
			handle_expand(input, sp, msh);
		else if (input[sp->i] == '|')
			handle_pipes(input, sp);
		else if (input[sp->i] == '<')
			handle_input_redir(input, sp, msh);
		else if (input[sp->i] == '>')
			handle_output_redir(input, sp, msh);
		else if (input[sp->i] && !is_whitespace(input[sp->i]))
			handle_words(input, sp, msh);
		if (sp->j > 0 && !sp->argv[sp->j - 1])
		{
			printf("Error: Memory allocation failed for token %d\n", sp->j - 1);
			while (--sp->j >= 0)
				free(sp->argv[sp->j]);
			free(sp->argv);
			return ;
		}
	}
}

void	process_input(const char *input, t_tkn_op *sp, t_msh *msh)
{
	skip_whitespace(&input);
	process_tokens(input, sp, msh);
}

/* void	process_input(const char *input, t_tkn_op *sp, t_msh *msh)
{
	while (*input && is_whitespace(*input))
		input++;
	while (input[sp->i])
	{
		while (input[sp->i] && is_whitespace(input[sp->i]))
			sp->i++;
		if (input[sp->i] == '\'' || input[sp->i] == '"')
			handle_quotes(input, sp, msh);
		else if (input[sp->i] == '$')
			handle_expand(input, sp, msh);
		else if (input[sp->i] == '|')
			handle_pipes(input, sp);
		else if (input[sp->i] == '<')
			handle_input_redir(input, sp, msh);
		else if (input[sp->i] == '>')
			handle_output_redir(input, sp, msh);
		else if (input[sp->i] && !is_whitespace(input[sp->i]))
			handle_words(input, sp, msh);
		if (sp->j > 0 && !sp->argv[sp->j - 1])
		{
			printf("Error: Memory allocation failed for token %d\n", sp->j - 1);
			while (--sp->j >= 0)
				free(sp->argv[sp->j]);
			free(sp->argv);
			return ;
		}
	}
} */

char	**split_input(const char *input, t_msh *msh)
{
	t_tkn_op	sp;

	sp.i = 0;
	sp.j = 0;
	msh->len = take_len(input);
	sp.argv = (char **)malloc(sizeof(char *) * (msh->len + 1));
	if (!sp.argv)
		return (NULL);
	process_input(input, &sp, msh);
	sp.argv[sp.j] = NULL;
	return (sp.argv);
}
