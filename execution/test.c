#include "exec.h"

char	*redir_to_str(t_redir_type type)
{
	if (type == R_INPUT)
		return ("<");
	if (type == R_OUTPUT)
		return (">");
	if (type == R_APPAND)
		return (">>");
	if (type == R_HEREDOC)
		return ("<<");
	return ("?");
}

void	print_cmds(t_cmd *cmd)
{
	int i = 1;
    	while (cmd)
    	{
        	printf("=== Command %d ===\n", i++);
        	for (int j = 0; cmd->argv && cmd->argv[j]; j++)
            		printf("argv[%d]: %s\n", j, cmd->argv[j]);

        	t_redir *r = cmd->redir;
        	while (r)
        	{
         	   printf("Redirection: type %s, file '%s'\n", redir_to_str(r->type), r->filename);
            	   r = r->next;
        	}
        cmd = cmd->next;
    }
}

t_cmd	*create_cmd_list(void)
{
	// ls -la | grep mini > file.txt

	t_cmd	*cmd1 = malloc(sizeof(t_cmd));
	cmd1->argv = ft_split("ls -la", ' ');
	cmd1->redir = NULL;
	cmd1->next = NULL;

	t_cmd	*cmd2 = malloc(sizeof(t_cmd));
	cmd2->argv = ft_split("grep min", ' ');

	t_redir	*redir = malloc(sizeof(t_redir));
	redir->type = R_OUTPUT;
	redir->filename = ft_strdup("file.txt");
	redir->next = NULL;

	cmd2->redir = redir;
	cmd2->next = NULL;

	cmd1->next = cmd2;
	return (cmd1);
}

int	main()
{
	t_cmd	*cmd_list = create_cmd_list();
	print_cmds(cmd_list);
	return (0);
}
