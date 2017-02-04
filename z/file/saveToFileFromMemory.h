#pragma once
#ifndef SAVETOFILEFROMMEMORY_H_INCLUDED
#define SAVETOFILEFROMMEMORY_H_INCLUDED

#include "../core/string.h"
#include <stdio.h>

namespace file
{
    bool saveStringToFile(const core::string<char>& fileName, const core::string<wchar_t>& fileData)
    {
                std::wofstream file;
                file.open(fileName.str());

                if (file.good())
                {
                    for (int i=0; i<fileData.length(); i++)
                    {
                        file.put(fileData[i]);
                    }

                    return true;
                }
                else
                {
                    return false;
                }
    }


    template <typename T>
    bool saveBinaryFile(const core::string<char>& fileName, const T& object)
    {
        FILE* file = NULL;

        file = fopen(fileName.str(), "wb");

        if (file)
        {
            fwrite(&object, sizeof(T), 1, file);

            fclose(file);
            return true;
        }
        else
        {
            return false;
        }
    }
}

#endif // SAVETOFILEFROMMEMORY_H_INCLUDED
