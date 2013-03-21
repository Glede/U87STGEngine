#pragma once

#include <iostream>
#include "U87Util.h"

namespace u87
{
    /** implements this interface for serialzation and deserialization */
    class Serializable
    {
    public:
        virtual bool initFromFile(const char* file) = 0;

        virtual void saveToFile(const char* file) = 0;
    };

    template <typename Type>
    void serializaData(const Type* data, std::ostream& out)
    {
        int length = sizeof(*data);
        UCHAR* p = (UCHAR*)data;
        for(int i = 0; i < length; i++)
            out.put(p[i]);
    }

    template <typename Type>
    void deserializaData(Type* data, std::istream& in)
    {
        int length = sizeof(*data);
        UCHAR* p = (UCHAR*)data;
        for(int i = 0; i < length; i++)
            p[i] = in.get();
    }
}