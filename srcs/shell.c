/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shell.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: welee <welee@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 14:58:18 by welee             #+#    #+#             */
/*   Updated: 2024/09/24 18:22:02 by welee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

/* shell.c */

#include "minishell.h"
#include "token.h"
#include "command.h"

void	handle_sigint(int sig)
{
	(void)sig;
	write(1, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	handle_sigquit(int sig)
{
	(void)sig;
	/* Do nothing */
}

void	setup_signal_handlers(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_handler = &handle_sigint;
	sigemptyset(&sa_int.sa_mask);
	sa_int.sa_flags = SA_RESTART;
	sigaction(SIGINT, &sa_int, NULL);

	sa_quit.sa_handler = &handle_sigquit;
	sigemptyset(&sa_quit.sa_mask);
	sa_quit.sa_flags = 0;
	sigaction(SIGQUIT, &sa_quit, NULL);
}

void	run_command(t_command *cmd)
{
	int	i;

	if (cmd->cmd)
		printf("Running command: %s\n", cmd->cmd);
	if (cmd->args)
	{
		i = 0;
		while (cmd->args[i])
		{
			printf("Arg %d: %s\n", i, cmd->args[i]);
			i++;
		}
	}
	if (cmd->input_file)
		printf("Input file: %s\n", cmd->input_file);
	if (cmd->output_file)
		printf("Output file: %s\n", cmd->output_file);
	if (cmd->append)
		printf("Appending to file\n");
	if (cmd->heredoc)
		printf("Using heredoc\n");
}

void	start_shell(void)
{
	char		*input;
	char		*prompt;
	t_token		*tokens;
	t_command	*cmd;

	prompt = "minishell$ ";
	setup_signal_handlers();
	while (1)
	{
		input = readline(prompt);
		if (!input)
		{
			printf("exit\n");
			break ;
		}
		if (*input)
		{
			add_history(input);
			tokens = lexer(input);
			cmd = parser(tokens);
			run_command(cmd);
		}
		free(input);
	}
}
