/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexer.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: welee <welee@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 16:55:45 by welee             #+#    #+#             */
/*   Updated: 2024/09/24 16:58:31 by welee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* lexer.c */

#include "token.h"
#include "libft.h"
// #include <stdlib.h>
// #include <string.h>
// #include <ctype.h>

static int	is_special_char(char c)
{
	return (c == '|' || c == '<' || c == '>');
}

static t_token	*create_token(t_token_type type, char *value)
{
	t_token	*new_token;

	new_token = malloc(sizeof(t_token));
	if (!new_token)
		return (NULL);
	new_token->type = type;
	new_token->value = value;
	new_token->next = NULL;
	return (new_token);
}

static t_token_type	get_token_type(char *str)
{
	if (ft_strcmp(str, "|") == 0)
		return (TOKEN_PIPE);
	else if (ft_strcmp(str, "<") == 0)
		return (TOKEN_REDIRECT_IN);
	else if (ft_strcmp(str, ">") == 0)
		return (TOKEN_REDIRECT_OUT);
	else if (ft_strcmp(str, ">>") == 0)
		return (TOKEN_REDIRECT_APPEND);
	else if (ft_strcmp(str, "<<") == 0)
		return (TOKEN_HEREDOC);
	return (TOKEN_ARG);
}

static char	*extract_token(char **input)
{
	char	*start;
	char	*token;

	while (ft_isspace(**input))
		(*input)++;
	if (is_special_char(**input))
	{
		if ((*input)[0] == (*input)[1])
			token = ft_strndup(*input, 2);
		else
			token = ft_strndup(*input, 1);
		*input += ft_strlen(token);
		return (token);
	}
	start = *input;
	while (**input && !ft_isspace(**input) && !is_special_char(**input))
		(*input)++;
	token = ft_strndup(start, *input - start);
	return (token);
}

t_token	*lexer(char *input)
{
	t_token			*head;
	t_token			*current;
	char			*token_str;
	t_token_type	type;

	head = NULL;
	while (*input)
	{
		token_str = extract_token(&input);
		type = get_token_type(token_str);
		if (!head)
			head = create_token(type, token_str);
		else
		{
			current = head;
			while (current->next)
				current = current->next;
			current->next = create_token(type, token_str);
		}
	}
	return (head);
}
