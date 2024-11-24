#pragma once
#include <memory/dlmalloc.h>
#include <memory/gccmemory.h>
#ifndef NULL
#define NULL 0
#endif

template <typename T> class Vector
{
public:
    Vector()
    {
        data = nullptr;
        len = 0;
        capacity = 0;
    }

    ~Vector()
    {
        if (data != nullptr)
        {
            free(data);
        }
    }

    void PushBack(T elem)
    {
        if (len == capacity)
        {
            capacity = capacity ? capacity * 2 : 1;
            data = (T*)realloc(data, sizeof(data[0]) * capacity);
        }
        data[len++] = elem;
    }

    void PushToBack(T elem)
    {
        //put at the bottom of the stack
        if (len == capacity)
        {
            capacity = capacity ? capacity * 2 : 1;
            data = (T*)realloc(data, sizeof(data[0]) * capacity);
        }
        for (size_t i = len; i > 0; i--)
        {
            data[i] = data[i - 1];
        }
        data[0] = elem;
        len++;
    }

    T PopBack()
    {
        //remove the last element
        T elem = data[len - 1];
        len--;
        return elem;
    }
    size_t Length()
    {
        return len;
    }

    T At(size_t index)
    {
        return data[index];
    }

    void Erase(size_t index)
    {
        memcpy(&data[index], &data[index + 1], sizeof(data[0]) * len - index - 1);
        len--;
    }

    void EraseAll()
    {
        len = 0;
        capacity = 0;
        if (data != nullptr) free(data);
        data = nullptr;
    }

    void EraseValue(T val)
    {
        for(size_t i = 0; i < len; i++)
        {
            if (At(i) == val)
            {
                Erase(i);
                break;
            }
        }
    }

    bool IsEmpty()
    {
        return len == 0;
    }

    //Front
    T Front()
    {
        return data[0];
    }

    T* Data()
    {
        return data;
    }

    //allow for direct access to the data using the [] operator
    T& operator[](size_t index)
    {
        return data[index];
    }

    // Iterator support
    T* begin()
    {
        return data;
    }

    T* end()
    {
        return data + len;
    }

private:
    size_t len;
    size_t capacity;
    T* data;
};