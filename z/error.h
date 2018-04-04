/**
 * File:            script_error.h
 * Namespace:       z::script::error
 * Description:     The z::script::errorFlag type is defined here.
 *                  Additionally, all error flags are defined here.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   27 Aug. 2017
**/

#pragma once
#ifndef SCRIPT_ERROR_H_INCLUDED
#define SCRIPT_ERROR_H_INCLUDED


#include <z/core/string.h>

namespace z
{
    namespace script
    {
        class error
        {
        private:
            int err_line;
            int err_column;

            core::string<char> err_file;
            core::string<char> err_msg;

        public:
            error()
            {
                err_line = -1;
                err_column = -1;
            }

            error(const core::string<char>& msg,
                  const core::string<char>& file,
                  int line = -1, int column = -1)
            {
                err_line = line;
                err_column = column;

                err_msg = msg;

                err_file = file;
            }

            error(const core::string<char>& msg,
                  int line = -1, int column = -1)
            {
                err_line = line;
                err_column = column;

                err_msg = msg;
            }

            error(const error& other)
            {
                err_line = other.err_line;
                err_column = other.err_column;

                err_msg = other.err_msg;

                err_file = other.err_file;
            }

            error(error&& other)
            {
                err_line = other.err_line;
                err_column = other.err_column;

                err_msg = other.err_msg;

                err_file = other.err_file;
            }

            const error& operator=(const error& other)
            {
                err_line = other.err_line;
                err_column = other.err_column;

                err_msg = other.err_msg;

                err_file = other.err_file;

                return *this;
            }

            inline bool operator==(const error& other) const
            {
                return err_msg == other.err_msg;
            }


            const core::string<char> fullMessage() const
            {
                core::string<char> fullmsg;

                if ((err_line >= 0) &&
                    (err_column >= 0))
                {
                    fullmsg = err_line;
                    fullmsg += ", ";
                    fullmsg += err_column;

                    if (err_file.length())
                        fullmsg += " ";
                }

                if (err_file.length())
                {
                    fullmsg += "in '";
                    fullmsg += err_file;
                    fullmsg += "'";
                }

                fullmsg += ": ";
                fullmsg += err_msg;

                return fullmsg;
            }


            inline void setPosition(int line, int column)
            {
                err_line = line;
                err_column = column;
            }

            inline void setMessage(const core::string<char>& msg)
            {
                err_msg = msg;
            }

            inline void setFile(const core::string<char>& file)
            {
                err_file = file;
            }


            inline int line() const
            {
                return err_line;
            }

            inline int column() const
            {
                return err_column;
            }

            inline const core::string<char>& message() const
            {
                return err_msg;
            }

            inline const core::string<char>& file() const
            {
                return err_file;
            }

            inline bool none()
            {
                return (err_msg.length() == 0);
            }
        };

    }
}

#endif // SCRIPT_ERROR_H_INCLUDED
