#pragma once
#ifndef LOADFILETOMEMORY_H_INCLUDED
#define LOADFILETOMEMORY_H_INCLUDED

#include "../core/string.h"
#include <fstream>
#include <stdio.h>

namespace file
{

    int getFileChrSizeW(const core::string<char>& fileName)
    {
        int size = 0;

        std::wifstream file;
        file.open(fileName.str());

        if (file)
        {
            file.seekg(0, std::ios_base::end);

            size = file.tellg();

            file.close();
        }
        else
        {
            return -1;
        }

        return size;
    }

    int getFileChrSizeC(const core::string<char>& fileName)
    {
        int size = 0;

        std::ifstream file;
        file.open(fileName.str());

        if (file)
        {
            file.seekg(0, std::ios_base::end);

            size = file.tellg();

            file.close();
        }
        else
        {
            return -1;
        }

        return size;
    }



    bool loadFileToMemory(const core::string<char>& fileName, core::string<char>& fileData)
    {
            int size = getFileChrSizeC(fileName);

            if (size < 0)
            {
                return false;
            }
            else
            {
                std::ifstream file;
                file.open(fileName.str());

                if (file)
                {
                    char tfileData[size+1];

                    for (int i=0; i<size; i++)
                    {
                        if (file.eof())
                        {
                            tfileData[i-1] = null;
                            break;
                        }

                        tfileData[i] = file.get();
                    }

                    tfileData[size] = null;

                    fileData = tfileData;
                }
                else
                {
                    return false;
                }
            }

        return true;

    }


    bool loadFileToMemory(const core::string<char>& fileName, core::string<wchar_t>& fileData)
    {
            int size = getFileChrSizeW(fileName);

            if (size < 0)
            {
                return false;
            }
            else
            {
                std::wifstream file;
                file.open(fileName.str());

                if (file)
                {
                    wchar_t tfileData[size+1];

                    for (int i=0; i<size; i++)
                    {
                        if (file.eof())
                        {
                            tfileData[i-1] = null;
                            break;
                        }

                        tfileData[i] = file.get();
                    }

                    tfileData[size] = null;

                    fileData = tfileData;
                }
                else
                {
                    return false;
                }
            }


        return true;

    }


    template <typename T>
    bool loadBinaryFile(const core::string<char>& fileName, T& object)
    {
        FILE* file = NULL;

        file = fopen(fileName.str(), "rb");

        if (file)
        {
            fread(&object, sizeof(object), 1, file);

            fclose(file);
            return true;
        }
        else
        {
            return false;
        }
    }
}

#endif // LOADFILETOMEMORY_H_INCLUDED
