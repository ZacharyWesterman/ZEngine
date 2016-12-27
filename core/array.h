#pragma once
#ifndef ARRAY_H_INCLUDED
#define ARRAY_H_INCLUDED

#ifndef uint
    #define uint uint32_t
#endif // uint

#include <vector>
#include <iostream>
using namespace std;

namespace core
{
    ///Custom array class
    //This array is meant for storing objects with a moderate-to-large size.
    //It uses a vector of pointers rather than objects, to speed up resizing.
    //Thus, this class is not effective for small objects like chars.
    //(char pointer size > char size)
    template <typename T>
    class array
    {
    private:
        std::vector<T> array_data;

        bool is_sorted;

    public:
        array() {is_sorted = false;}
        array(bool sorted) {is_sorted = sorted;}
        array(const array&);
        ~array() {}

        void clear();
        void append(const T&);

        bool insert(const T&, uint);
        void insert(const T&);

        bool remove(uint);

        uint size() const;

        T& at(const uint);
        const T& at(const uint) const;
        T& operator[](const uint);
        const T& operator[](const uint) const;

        int find(const T&) const;

        const array& operator=(const array& other)
        {
            clear();

            is_sorted = other.is_sorted;

            for (uint i=0; i<other.array_data.size(); i++)
                array_data.push_back(other.array_data.at(i));

            return *this;
        }

        bool sorted() const { return is_sorted; }

        bool is_valid(uint position) const
        {
            return (position < array_data.size());
        }
    };

    ///Copy constructor
    template <typename T>
    array<T>::array(const array<T>& other)
    {
        is_sorted = other.is_sorted;

        for (uint i=0; i<other.array_data.size(); i++)
            array_data.push_back(other.array_data.at(i));
    }


    ///Function to empty the array
    template <typename T>
    void array<T>::clear()
    {
        array_data.clear();
    }


    ///Function to append an object to the end of the array
    template <typename T>
    void array<T>::append(const T& object)
    {
        array_data.push_back(object);
    }


    ///Function to insert an object to the given index in the array
    ///Do not use this if the array is meant to be auto-sorted!
    //places the given object in that index(if valid), returning false if invalid index
    template <typename T>
    bool array<T>::insert(const T& object, uint index)
    {
        //if invalid index, return false
        if (index >= (int)array_data.size())
            return false;

        //if curr < prev, not sorted
        if ((index > 0) && (array_data.at(index-1) > object))
            is_sorted = false;

        //or if curr > next, not sorted
        if ((index < array_data.size()-1) && (array_data.at(index+1) < object))
            is_sorted = false;


        array_data.insert(array_data.begin() + index, object);

        return true;
    }

    ///Function to insert the given object into the array
    ///Use this if the array is meant to be auto-sorted!
    ///If the array is currently sorted, sort this object into the array.
    ///Otherwise, insert at the beginning.
    //places the given object in that index(if valid), returning false if invalid index
    template <typename T>
    void array<T>::insert(const T& object)
    {
        if (is_sorted)
        {
            bool done = false;
            uint index = 0;

            while(!done && (index < array_data.size()))
            {
                if (array_data.at(index) > object)
                {
                    array_data.insert(array_data.begin() + index, object);
                    done = true;
                }

                index++;
            }

            if (!done)
                array_data.push_back(object);
        }
        else
        {
            array_data.insert(array_data.begin(), object);
        }
    }


    ///Function to remove an object from the array
    //removes an object from the given index, returning false if invalid index
    template <typename T>
    bool array<T>::remove(uint index)
    {
        if ((index < 0) || (index >= array_data.size()))
            return false;

        array_data.erase(array_data.begin() + index);

        return true;
    }


    ///Return the number of objects in the array
    template <typename T>
    uint array<T>::size() const
    {
        return array_data.size();
    }

    ///Functions to get an object from the array, given an index
    //throw an exception if given an invalid index
    template <typename T>
    T& array<T>::at(uint index)
    {
        if (index >= array_data.size())
        {
            throw std::bad_alloc();
        }
        else
        {
            return array_data.at(index);
        }
    }

    template <typename T>
    const T& array<T>::at(uint index) const
    {
        if (index >= array_data.size())
        {
            throw std::bad_alloc();
        }
        else
        {
            return array_data.at(index);
        }
    }


    template <typename T>
    T& array<T>::operator[](uint index)
    {
        if (index >= array_data.size())
        {
            throw std::bad_alloc();
        }
        else
        {
            return array_data.at(index);
        }
    }

    template <typename T>
    const T& array<T>::operator[](uint index) const
    {
        if (index >= array_data.size())
        {
            throw std::bad_alloc();
        }
        else
        {
            return array_data.at(index);
        }
    }

    ///Function to check if a given object is in the array
    //returns -1 if the object was not found,
    //returns the object's index if it was found
    template <typename T>
    int array<T>::find(const T& object) const
    {
        if (false)//is_sorted)
        {
            uint left = 0;
            uint right = array_data.size()-1;

            while (left != right)
            {
                uint center = (left + right) / 2;

                if (array_data.at(center) < object)
                {
                    left = center + 1;
                }
                else if (array_data.at(center) > object)
                {
                    right = center - 1;
                }
                else
                {
                    return center;
                }
            }

            if (array_data.at(left) == object)
                return left;
        }
        else
        {
            for (uint i=0; i<array_data.size(); i++)
                if (array_data.at(i) == object)
                    return i;
        }

        return -1;
    }


}

#endif // ARRAY_H_INCLUDED
