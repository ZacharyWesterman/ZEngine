/**
 * File:            sorted_array.h
 * Namespace:       z::core
 * Description:     An extension of the array class which
 *                  attempts to keep all data sorted.
 *                  Note that this is not finished yet, and
 *                  it does not work.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   3 Feb. 2017
**/


#pragma once
#ifndef SORTED_ARRAY_H_INCLUDED
#define SORTED_ARRAY_H_INCLUDED

#include "./array.h"

#include <iostream>

namespace z
{
    namespace core
    {
        template <typename T>
        class sorted_array : public array<T>
        {
        private:
            //private

        public:
            virtual int add(const T&);

            virtual int find(const T&) const;
        };



        template <typename T>
        int sorted_array<T>::add(const T& object)
        {
            if (this->array_data.size() == 0)
            {
                this->array_data.push_back(object);
                return 0;
            }
            else
            {
                int left = 0;
                int right = this->array_data.size()-1;


                while (left < right)
                {
                    int center = (left + right) / 2;


                    if (this->array_data.at(center) > object)
                    {
                        right = center;
                    }
                    else if (this->array_data.at(center) < object)
                    {
                        left = center + 1;
                    }
                    else
                    {
                        this->insert(object, center);
                        return center;
                    }


                    //std::cout << left << ":" << right << std::endl;
                }


                if (this->array_data.at(left) < object)
                    this->insert(object, left+1);
                else
                    this->insert(object, left);

                return left;
            }
        }


        template <typename T>
        int sorted_array<T>::find(const T& object) const
        {
            int left = 0;
            int right = this->array_data.size()-1;

            while (left != right)
            {
                int center = (left + right) / 2;

                if ((this->array_data.at(center)) < object)
                {
                    left = center + 1;
                }
                else if (this->array_data.at(center) > object)
                {
                    right = center - 1;
                }
                else
                {
                    return center;
                }
            }

            if (this->array_data.at(left) == object)
                return left;
            else
                return -1;
        }
    }
}

#endif // SORTED_ARRAY_H_INCLUDED
