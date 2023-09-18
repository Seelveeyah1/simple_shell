#include "shell.h"

/**
 * _strlen -  length of a string that returns
 * @s: string length to check
 * Return: an integer length of string
 */
int _strlen(char *s)
{
	int i = 0;

	if (!s)
		return (0);

	while (*s++)
		i++;
	return (i);
}

/**
 * _strcmp - performs a comparison of two strings.
 * @s1: the first string
 * @s2: the second string
 * Return: negative if s1 < s2, positive if s1 > s2, else 0 if s1 == s2
 */
int _strcmp(char *s1, char *s2)
{
	while (*s1 && *s2)
	{
		if (*s1 != *s2)
		return (*s1 - *s2);
		s1++;
		s2++;
	}
	if (*s1 == *s2)
		return (0);
	else
		return (*s1 < *s2 ? -1 : 1);
}

/**
 * starts_with - checking  if needle starts with haystack
 * @haystack: the string to search
 * @needle: the substring to find
 * Return: address of the next character of haystack or NULL
 */

char *starts_with(const char *haystack, const char *needle)
{
	while (*needle)
		if (*needle++ != *haystack++)
		{
			return (NULL);
		}
	return ((char *)haystack);
}

/**
 * _strcat - two strings to concatenate
 * @dest: the destination buffer
 * @src: the source buffer
 * Return: pointing to the destination buffer
 */

char *_strcat(char *dest, char *src)
{
	char *ret = dest;

	while (*dest)
		dest++;
	while (*src)
		*dest++ = *src++;
	*dest = *src;
	return (ret);
}
