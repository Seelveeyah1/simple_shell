#include "shell.h"

/**
 * hsh - Main shell loop for executing commands.
 * @info: Pointer to the information structure.
 * @av: Argument vector from main().
 *
 * This function is the main shell loop responsible for handling user commands.
 * It repeatedly clears the information structure, displays a prompt if running
 * interactively, reads user input, and processes the input by either finding
 * built-in commands or external commands. It also manages memory and exits the
 * shell when necessary.
 *
 * Return: 0 on success, 1 on error, or an error code.
 */
int hsh(info_t *info, char **av)
{
	ssize_t r = 0;
	int builtin_ret = 0;

	while (r != -1 && builtin_ret != -2)
	{
		clear_info(info);

		/* Display the shell prompt if running interactively */
		if (interactive(info))
			_puts("$ ");

		_eputchar(BUF_FLUSH);

		/* Read user input */
		r = get_input(info);

		if (r != -1)
		{
			set_info(info, av);
			builtin_ret = find_builtin(info);

/* If not a built-in command, find and execute external command */
			if (builtin_ret == -1)
				find_cmd(info);
		}
		else if (interactive(info))
			_putchar('\n');

		/* Free allocated memory */
		free_info(info, 0);
	}

	/* Write shell history */
	write_history(info);

	/* Free memory and exit the shell */
	free_info(info, 1);

/* Handle non-interactive mode and specific exit conditions */
	if (!interactive(info) && info->status)
		exit(info->status);

	if (builtin_ret == -2)
	{
		/* Handle specific error conditions */
		if (info->err_num == -1)
			exit(info->status);

		exit(info->err_num);
	}

	return (builtin_ret);
}

/**
 * find_builtin - finds a builtin command
 * @info: the parameter & return information structure
 *
 * Return: -1 if builtin is not found, 0 if builtin is
 * executed successfully, 1 if builtin is found but not
 * successful, -2 if builtin signals exit()
 */
int find_builtin(info_t *info)
{
	int i, built_in_ret = -1;
	builtin_table builtintbl[] = {
		{"exit", _myexit},
		{"env", _myenv},
		{"help", _myhelp},
		{"history", _myhistory},
		{"setenv", _mysetenv},
		{"unsetenv", _myunsetenv},
		{"cd", _mycd},
		{"alias", _myalias},
		{NULL, NULL}
	};

	for (i = 0; builtintbl[i].type; i++)
		if (_strcmp(info->argv[0], builtintbl[i].type) == 0)
	{
		info->line_count++;
		built_in_ret = builtintbl[i].func(info);
		break;
	
	}
	return (built_in_ret);
}

/**
 * find_cmd - finding a command in the PATH
 * @info: the parameter & return information structure
 * Return: void
 */
void find_cmd(info_t *info)
{
	char *path = NULL;
	int i, k;

	info->path = info->argv[0];
	if (info->linecount_flag == 1)
	{
		info->line_count++;
		info->linecount_flag = 0;
	}
	for (i = 0, k = 0; info->arg[i]; i++)
		if (!is_delim(info->arg[i], " \t\n"))
			k++;
	if (!k)
		return;

	path = find_path(info, _getenv(info, "PATH="), info->argv[0]);
	if (path)
	{
		info->path = path;
		fork_cmd(info);
	}
	else
	{
	if ((interactive(info) || _getenv(info, "PATH=") ||
	info->argv[0][0] == '/') && is_cmd(info, info->argv[0]))
		fork_cmd(info);
	else if (*(info->arg) != '\n')
	{
		info->status = 127;
		print_error(info, "not found\n");
	}
	}
}

/**
 * fork_cmd - to fork an executive thread to run command
 * @info: the parameter & return information structure
 *
 * Return: void
 */
void fork_cmd(info_t *info)
{
	pid_t child_pid;

	child_pid = fork();

	if (child_pid == -1)
	{
		/* TODO: PUT THE ERROR FUNCTION */
		perror("Error:");
		return;
	}
	if (child_pid == 0)
	{
		if (execve(info->path, info->argv, get_environ(info)) == -1)
		{
			free_info(info, 1);
			if (errno == EACCES)
				exit(126);
			exit(1);
		}
		/* TODO: PUT THE ERROR FUNCTION */
	}
	else
	{
		wait(&(info->status));
	if (WIFEXITED(info->status))
	{
		info->status = WEXITSTATUS(info->status);
		if (info->status == 126)
		print_error(info, "Permission denied\n");
	}
	}
}
