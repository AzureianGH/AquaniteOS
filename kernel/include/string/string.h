#pragma once

#include <stdint.h>
#include <stddef.h>

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