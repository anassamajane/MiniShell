#ifndef EXEC_H
#define EXEC_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include "../libft/libft.h"

typedef enum s_redir_type
{
	R_INPUT,
	R_OUTPUT,
	R_APPAND,
	R_HEREDOC,
}	t_redir_type;

typedef struct s_redir
{
	t_redir_type	type;
	char	*filename;
	struct s_redir	*next;
}	t_redir;

typedef struct s_cmd
{
	char	**argv;
	t_redir	*redir;
	struct s_cmd	*next;
}	t_cmd;

/* builtins */
int	ft_echo(char **argv);
int	ft_pwd(void);
int	ft_env(char **argv, char **envp);

#endif
