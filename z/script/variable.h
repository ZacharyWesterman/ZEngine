#pragma once
#ifndef VARIABLE_H_INCLUDED
#define VARIABLE_H_INCLUDED

#include "../core/string.h"
#include "../core/array.h"

#include "script_errors.h"
#include "is_valid_script_name.h"


namespace script
{
    namespace VARIABLE
    {
        enum TYPE
        {
            NONE = 0,
            SINGLE,
            ARRAY
        };
    }

    //template for script variables
    //a variable has a single name, and an array of values.
    //these values can be accessed by name[index].
    //any script variable that is not an array
    //will be an array with a length of 1.
    template <typename CHAR>
    class variable
    {
    private:
        core::string<CHAR> var_name;
        core::array< core::string<CHAR> > var_values;

        bool name_is_valid;
        VARIABLE::TYPE var_type;

    public:
        variable() {name_is_valid = false; var_type = VARIABLE::NONE;}
        variable(const core::string<CHAR>& name)
        {
            var_name = name;
            name_is_valid = is_valid_script_name(var_name);

            var_type = VARIABLE::NONE;
        }

        ~variable() {}


        const variable& operator=(const variable& other)
        {
            var_name = other.var_name;
            var_values = other.var_values;

            var_type = other.var_type;
            name_is_valid = other.name_is_valid;

            return *this;
        }

        const variable& operator=(const core::string<CHAR>& name)
        {
            var_values.clear();
            var_type = VARIABLE::NONE;

            var_name = name;
            name_is_valid = is_valid_script_name(var_name);

            return *this;
        }


        bool rename(const core::string<CHAR>& name)
        {
            var_type = VARIABLE::NONE;

            var_name = name;
            name_is_valid = is_valid_script_name(var_name);

            return name_is_valid;
        }

        error_flag set_array_size(const int new_size)
        {
            var_values.clear();

            if (new_size > 0)
            {
                var_type = VARIABLE::ARRAY;

                for (int i=0; i<new_size; i++)
                {
                    core::string<CHAR> empty_str;
                    var_values.add(empty_str);
                }

                return error::NONE;
            }
            else
            {
                var_type = VARIABLE::NONE;
                return error::OUT_OF_BOUNDS;
            }
        }

        error_flag set_array_values(const core::array< core::string<CHAR> >& vals)
        {
            var_type = VARIABLE::ARRAY;

            var_values = vals;

            return error::NONE;
        }

        error_flag set_array_value(const int index, const core::string<CHAR>& val)
        {
            if (var_type != VARIABLE::ARRAY)
                return error::WRONG_VAR_TYPE;

            if ((int)var_values.size() > index)
            {
                var_values.at(index) = val;

                return error::NONE;
            }
            else
            {
                return error::OUT_OF_BOUNDS;
            }
        }

        error_flag set_to_single_value(const core::string<CHAR>& val)
        {
            var_values.clear();

            var_values.add(val);

            var_type = VARIABLE::SINGLE;

            return error::NONE;
        }


        error_flag get_single_value(core::string<CHAR>& output) const
        {
            if (var_type != VARIABLE::SINGLE)
                return error::WRONG_VAR_TYPE;

            output = var_values.at(0);

            return error::NONE;
        }

        error_flag get_array_value(const int index, core::string<CHAR>& output) const
        {
            if (var_type != VARIABLE::ARRAY)
                return error::WRONG_VAR_TYPE;

            if ((index < 0) || (index >= (int)var_values.size()))
                return error::OUT_OF_BOUNDS;

            output = var_values.at(index);
            return error::NONE;
        }


        int get_size() const {return var_values.size();}

        const core::string<CHAR>& get_name() const {return var_name;}

        bool is_valid() const {return name_is_valid;}
        VARIABLE::TYPE get_type() const {return var_type;}
    };
}

#endif // VARIABLE_H_INCLUDED
