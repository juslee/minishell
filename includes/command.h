/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: welee <welee@student.42singapore.sg>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/24 16:51:47 by welee             #+#    #+#             */
/*   Updated: 2024/09/24 18:23:35 by welee            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef COMMAND_H
# define COMMAND_H

# include "token.h"

typedef struct s_command
{
	char				*cmd;
	char				**args;
	char				*input_file;
	char				*output_file;
	int					append;
	int					heredoc;
	struct s_command	*next;
}	t_command;

t_command	*parser(t_token *tokens);

#endif
