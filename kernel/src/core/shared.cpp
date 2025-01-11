#include <shared.h>

int create_flags(int first, ...)
{
    va_list args;
    va_start(args, first);
    int flags = first;
    int flag = va_arg(args, int);
    while (flag != 0)
    {
        flags |= flag;
        flag = va_arg(args, int);
    }
    va_end(args);
    return flags;
}

bool has_flag(int flags, int flag)
{
    return (flags & flag) == flag;
}