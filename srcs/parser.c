/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: welee <welee@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 17:00:15 by welee             #+#    #+#             */
/*   Updated: 2024/09/24 18:55:39 by welee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* parser.c */

#include "token.h"
#include "command.h"
#include "libft.h"
#include <stdlib.h>
// #include <string.h>

// Create a new command structure
static t_command	*create_command(char *cmd)
{
	t_command	*new_cmd;

	new_cmd = malloc(sizeof(t_command));
	if (!new_cmd)
		return (NULL);
	new_cmd->cmd = ft_strdup(cmd);
	new_cmd->args = malloc(sizeof(char *) * 2);
	new_cmd->args[0] = ft_strdup(cmd);
	new_cmd->args[1] = NULL;
	new_cmd->input_file = NULL;
	new_cmd->output_file = NULL;
	new_cmd->append = 0;
	new_cmd->heredoc = 0;
	new_cmd->next = NULL;
	return (new_cmd);
}

// Add an argument to the command structure
static void	add_argument(t_command *cmd, char *arg)
{
	int	i;

	i = 0;
	while (cmd->args[i])
		i++;
	cmd->args = ft_realloc(cmd->args, sizeof(char *) * (i + 2));
	cmd->args[i] = ft_strdup(arg);
	cmd->args[i + 1] = NULL;
}

// Set redirection details (input/output) in the command structure
static void	set_redirection(t_command *cmd, t_token *token)
{
	if (token->type == TOKEN_REDIRECT_IN)
		cmd->input_file = ft_strdup(token->next->value);
	else if (token->type == TOKEN_REDIRECT_OUT)
		cmd->output_file = ft_strdup(token->next->value);
	else if (token->type == TOKEN_REDIRECT_APPEND)
	{
		cmd->output_file = ft_strdup(token->next->value);
		cmd->append = 1;
	}
	else if (token->type == TOKEN_HEREDOC)
	{
		cmd->input_file = ft_strdup(token->next->value);
		cmd->heredoc = 1;
	}
}

static void	handle_command_and_args(t_command **head, t_command **current, t_token *token)
{
	if (!*head)
	{
		*head = create_command(token->value);
		*current = *head;
	}
	else if (token->type == TOKEN_ARG)
	{
		add_argument(*current, token->value);
	}
}

t_command	*parser(t_token *tokens)
{
	t_command	*head;
	t_command	*current;
	t_token		*token;

	token = tokens;
	head = NULL;
	current = NULL;
	while (token)
	{
		if (token->type == TOKEN_COMMAND || token->type == TOKEN_ARG)
			handle_command_and_args(&head, &current, token);
		else if (token->type == TOKEN_PIPE)
		{
			current->next = create_command(token->next->value);
			current = current->next;
		}
		else if (token->type == TOKEN_REDIRECT_IN
			|| token->type == TOKEN_REDIRECT_OUT
			|| token->type == TOKEN_REDIRECT_APPEND
			|| token->type == TOKEN_HEREDOC)
			set_redirection(current, token);
		token = token->next;
	}
	return (head);
}
