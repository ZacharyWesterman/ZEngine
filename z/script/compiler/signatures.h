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
         *
         * \see funcSignature
         * \see typeSignature
         */
        struct varSignature
        {
            void* ID; ///<Identifier for variable name.
            void* type;///<Identifier for variable type.

            symID uniqueID;///<Unique value to identify with this variable.


            inline bool operator==(const varSignature&) const;

            varSignature(void* _ID,
                         symID _uniqueID = 0,
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
                                   symID _uniqueID,
                                   void* _type)
        {
            ID = _ID;
            type = _type;
            uniqueID = _uniqueID;
        }


        /**
         * \brief Function signature for semantic analyzer.
         *
         * \see varSignature
         * \see typeSignature
         */
        struct funcSignature
        {
            void* ID;///<Identifier for function name.
            void* returnType;///<Identifier for function return type.

            symID uniqueID;///<Unique value to identify with this function.

            ///Identifier for the current type that the function is in.
            void* inType;

            ///List of identifiers for each parameter's type.
            core::array< void* > paramTypes;

            ///List of unique ID values for each parameter.
            core::array< symID > params;

            bool declared;

            bool operator==(const funcSignature& other) const;

            funcSignature(void* _ID, void* _returnType,
                          symID _uniqueID, void* _inType,
                          bool is_declared);

            const funcSignature& operator=(const funcSignature& other);
        };

        /**
         * \brief Full constructor.
         *
         * \param _ID the identifier for the function name.
         * \param _returnType the identifier for the function return type.
         * \param _uniqueID a unique value to identify the function.
         * \param _inType an identifier for the type this function is in.
         * \b NULL implies that this function does not belong to a type.
         */
        funcSignature::funcSignature(void* _ID, void* _returnType,
                                     symID _uniqueID, void* _inType,
                                     bool is_declared = false)
        {
            ID = _ID;
            returnType = _returnType;
            uniqueID = _uniqueID;
            inType = _inType;
            declared = is_declared;
        }

        /**
         * \brief Compare this signature to another function signature.
         *
         * This function is used to search for other functions
         * with the same name and parameter types in the same
         * scope (type declaration).
         */
        bool funcSignature::operator==(const funcSignature& other) const
        {
            return (ID == other.ID) &&
                   (inType == other.inType) &&
                   (paramTypes == other.paramTypes);
        }

        /**
         * \brief Function signature assignment operator.
         */
        const funcSignature& funcSignature::operator=
            (const funcSignature& other)
        {
            ID = other.ID;
            returnType = other.returnType;
            uniqueID = other.uniqueID;
            inType = other.inType;
            paramTypes = other.paramTypes;

            return *this;
        }


        /**
         * \brief Type signature for semantic analyzer.
         *
         * \see varSignature
         * \see funcSignature
         */
        struct typeSignature
        {
            void* type;///<Identifier for the type name.
            varScope* scope;///<Scope to keep track of all type members.

            ///List of the unique identifiers of this type's member variables.
            core::array<symID> vars;

            ///List of the unique identifiers of this type's member functions.
            core::array<symID> funcs;


            inline bool operator==(const typeSignature& other) const;

            typeSignature(void* _type, varScope* _scope);
        };

        /**
         * \brief Full constructor.
         *
         * \param _type the identifier for the type name.
         * \param _scope pointer to the scope where the type
         * was first declared.
         */
        typeSignature::typeSignature(void* _type, varScope* _scope)
        {
            type = _type;
            scope = _scope;
        }

        /**
         * \brief Compare this signature to another type signature.
         *
         * This function is used to search for other types
         * with the same name.
         */
        inline bool typeSignature::operator==(const typeSignature& other) const
        {
            return (type == other.type);
        }
    }
}
}

#endif // SIGNATURES_H_INCLUDED
