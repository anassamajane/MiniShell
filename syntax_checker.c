#include "lexer.h"

bool	is_redirection(t_token_type type)
{
	return (type == T_REDIR_IN || type == T_REDIR_OUT || type == T_APPEND || type == T_HEREDOC);
}

bool	check_pipe_syntax(t_list *tokens)
{
	t_token	*curr;
	t_token	*next;

	curr = (t_token *)tokens->content;
	if (curr->type == T_PIPE) // pipe at the begening ==> | ls 
		return (false);
	while (tokens && tokens->next)
	{
		curr = (t_token *)tokens->content;
		next = (t_token *)tokens->next->content;
		if (curr->type == T_PIPE && next->type == T_PIPE) // two pipe ==> cat file || ls
			return (false);
		tokens = tokens->next;
	}
	curr = (t_token *)tokens->content;
	if (curr->type == T_PIPE) // pipe at the end ==> ls -la | grep hello |
		return (false);
	return (true);
}

bool	check_redirection_syntax(t_list *tokens)
{
	t_token	*curr;
	t_token	*next;

	while (tokens && tokens->next)
	{
		curr = (t_token *)tokens->content;
		next = (t_token *)tokens->next->content;
		if (is_redirection(curr->type) && next->type != T_WORD) // redir not followed by a word
			return (false); // examples : (echo > <<)  (echo > |)
		tokens = tokens->next;
	}
	if (tokens)
	{
		curr = (t_token *)tokens->content;
		if (is_redirection(curr->type)) // redir at the end ==> (echo >>)
			return (false);
	}
	return (true);
}

bool	check_invalid_sequences(t_list *tokens)
{
	t_token	*curr;
	t_token	*next;

	while (tokens && tokens->next)
	{
		curr = (t_token *)tokens->content;
		next = (t_token *)tokens->next->content;
		if (is_redirection(curr->type) && is_redirection(next->type)) // redir followed by a redir
			return (false); // example : (><) (>> <)
		//if (curr->type == T_PIPE && is_redirection(next->type)) // pipe followed by a redir
		//	return (false); // example : (| >>) (| <)
		tokens = tokens->next;
	}
	return (true);
}

/*bool	check_heredoc_delimiter(t_list *tokens)
{
	t_token	*curr;
	t_token	*next;

	while (tokens && tokens->next)
	{
		curr = (t_token *)tokens->content;
		next = (t_token *)tokens->next->content;
		if (curr->type == T_HEREDOC && next->type == T_WORD)
		{
			if (next->value[0] == '\0' && !next->was_quoted)
				return (false);
		}
		tokens = tokens->next;
	}
	return (true);
}*/

bool	syntax_is_valid(t_list *tokens)
{
	if (!tokens) // no tokens = valide
		return (true);
	if (!check_pipe_syntax(tokens))
		return (false);
	if (!check_redirection_syntax(tokens))
		return (false);
	if (!check_invalid_sequences(tokens))
		return (false);
	/*if (!check_heredoc_delimiter(tokens))
		return (false);*/
	return (true);
}
