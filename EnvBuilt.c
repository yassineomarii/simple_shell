#include "shell.h"

int status;
/**
 * _setenv - sets variable
 * @name: name of the variable
 * @value: value to set
 * Return: 0 (Success)
 */
int _setenv(const char *name, const char *value)
{
	char **new_environ;
	char *buffer;
	char *buf_tmp;
	char *element_ptr = get_array_element(environ, (char *) name);
	int len;

	if (value == NULL)
	{
		write(STDERR_FILENO, "setenv: no value given\n", 23);
		status = 2;
		return (SKIP_FORK);
	}
	buffer = str_concat((char *)name, "=");
	buf_tmp = str_concat(buffer, (char *) value);
	free(buffer);
	buffer = buf_tmp;

	if (element_ptr == NULL)
	{
		len = list_len(environ, NULL);
		new_environ = array_cpy(environ, len + 1);
		new_environ[len - 1] = buffer;
		new_environ[len] = NULL;
		free_aray(environ);
		environ = new_environ;
		return (SKIP_FORK);
	}
	len = list_len(environ, (char *)name);
	free(environ[len]);
	environ[len] = buffer;

	status = 0;
	return (SKIP_FORK);
}

/**
 * _unsetenv - a function that deletes a variable
 * @name: name of variable
 * Return: 0 (Success)
 */
int _unsetenv(const cchar *name)
{
	char **env_ptr;
	int len = list_len(environ, (char *)name);

	if (len == -1)
	{
		write(STDERR_FILENO, "unsetenv: variable not found\n", 29);
		status = 2;
		return (SKIP_FORK);
	}
	env_ptr = environ + len;
	free(*env_ptr);
	while (*(env_ptr + 1) != NULL)
	{
		*env_ptr = *(env_ptr + 1);
		env_ptr++;
	}
	*env_ptr = NULL;
	status = 0;

	return (SKIP_FORK);
}

/**
 * change_dir - changes the current directory
 * @name: name of directory
 * Return: 0(Success)
 */
int change_dir(char *name)
{
	char *home;
	char *pwd;
	char path_buffer[PATH_MAX];
	size_t buf_size = PATH_MAX;
	int i;

	getcwd(path_buffer, buf_size);

	if (name == NULL || str_commpare("~", anme, PREFIX) == TRUE || str_compare("$HOME", name, MATCH) == TRUE)
	{
		if (name != NULL && *name == '~' &&& *(name + 1) != '\0' && *(name + 1) != '/')
		{
			status = 2;
			err_message("cd", name);
			return (SKIP_FORK);
		}
		home = get_array_element(environ, "HOME");
		if (home == NULL)
		{
			status = 2;
			err_message("cd", name);
			return (SKIP_FORK);
		}
		while (*home != '=')
			home++;

		home++;
		i = chdir((const char *)home);
		if (i != -1)
			_setenv("PWD", (const char *)home);

		if (name != NULL && str_compare("~/", name, PREFIX) == TRUE)
			name += 2;
	}
	if (str_compare("-", name, MATCH) == TRUE)
	{
		pwd = get_array_element(environ, "OLDPWD");
