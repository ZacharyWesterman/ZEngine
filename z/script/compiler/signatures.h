#pragma once
#ifndef SIGNATURES_H_INCLUDED
#define SIGNATURES_H_INCLUDED


#include <z/core/array.h>

namespace z
{
namespace script
{
    namespace compiler
    {
        typedef unsigned long symID;
        class varScope;

        /**
         * \brief Variable signature for semantic analyzer.
         */
        struct varSignature
        {
            void* ID; ///<Identifier for variable name.
            void* type;///<Identifier for variable type.

            symID uniqueID;///<Unique value to identify with this variable.


            inline bool operator==(const varSignature&) const;

            varSignature(void* _ID,
                         unsigned long _uniqueID = 0,
                         void* _type = NULL);
        };

        /**
         * \brief Compare this signature to another in the scope.
         *
         * This function is used to search for other variables
         * with the same name in a given scope.
         */
        inline bool varSignature::operator==(const varSignature& other) const
        {
            return (ID == other.ID);
        }

        /**
         * \brief Full constructor.
         *
         * \param _ID the identifier for the variable name.
         * \param _uniqueID an optional, unique value to
         * identify the variable.
         * \param _type an optional identifier for the
         * variable type. If not given or \b NULL, the
         * variable is assumed to be non-typed.
         */
        varSignature::varSignature(void* _ID,
                                   unsigned long _uniqueID,
                                   void* _type)
        {
            ID = _ID;
            type = _type;
            uniqueID = _uniqueID;
        }


        struct funcSignature
        {
            void* ID;
            void* returnType;

            symID uniqueID;

            void* inType;

            core::array< void* > paramTypes;
            core::array< symID > params;

            inline bool operator==(const funcSignature& other) const
            { return (ID == other.ID) &&
                     (inType == other.inType) &&
                     (paramTypes == other.paramTypes); }

            funcSignature(void* _ID, void* _returnType,
                          symID _uniqueID, void* _inType)
            {
                ID = _ID;
                returnType = _returnType;
                uniqueID = _uniqueID;
                inType = _inType;
            }

            const funcSignature& operator=(const funcSignature& other)
            {
                ID = other.ID;
                returnType = other.returnType;
                uniqueID = other.uniqueID;
                inType = other.inType;
                paramTypes = other.paramTypes;

                return *this;
            }
        };

        struct typeSignature
        {
            void* type;
            varScope* scope;

            inline bool operator==(const typeSignature& other) const
            { return (type == other.type); }

            typeSignature(void* _type, varScope* _scope)
            {
                type = _type;
                scope = _scope;
            }

            core::array<symID> vars;
            core::array<symID> funcs;
        };
    }
}
}

#endif // SIGNATURES_H_INCLUDED
