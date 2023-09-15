#include "shell.h"

/**
 * bfree - freeing a pointer, and Having our address as
 * NULL
 * @ptr: the address of the pointer to be freed
 * Return: 1 if freed, else 0.
 */
int bfree(void **ptr)
{
	if (ptr && *ptr)
	{
		free(*ptr);
		*ptr = NULL;
		return (1);
	}
	return (0);
}
