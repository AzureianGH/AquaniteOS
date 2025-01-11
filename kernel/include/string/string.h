#pragma once

#include <stdint.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C"
{
#endif
int strcmp(const char* str1, const char* str2);
//strtok
char *strtok(char *str, const char *delim);

//strdup
char* strdup(const char* str);

//strlen
size_t strlen(const char* str);

//strncmp
int strncmp(const char* str1, const char* str2, size_t n);

//strncpy
char* strncpy(char* dest, const char* src, size_t n);

//stpcpy
char* stpcpy(char* dest, const char* src);

//strstr
char* strstr(const char* haystack, const char* needle);

//strcasecmp
int strcasecmp(const char* str1, const char* str2);

//strrchr
char* strrchr(const char* str, int c);

//strchr
char* strchr(const char* str, int c);

//strncasecmp
int strncasecmp(const char* str1, const char* str2, size_t n);

//atoi
int atoi(const char* str);

//isspace
int isspace(int c);

//tolower
int tolower(int c);

//toupper
int toupper(int c);

//sscanf
int sscanf(const char* str, const char* format, ...);

//atof
double atof(const char* str);

#ifdef __cplusplus
}
#endif