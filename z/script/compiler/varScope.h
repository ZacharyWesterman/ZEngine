#pragma once
#ifndef VARSCOPE_H_INCLUDED
#define VARSCOPE_H_INCLUDED

#include "signatures.h"

namespace z
{
namespace script
{
    namespace compiler
    {
        /**
         * \brief A struct for keeping track of defined
         * variables and in what scopes they are defined.
         */
        struct varScope
        {
            ///The scope one level outside of this one.
            varScope* parent;

            ///A list of the variables in this scope.
            core::array< varSignature > vars;

            ///A list of the scopes beneath this one.
            core::array< varScope > children;


            varScope(varScope* _parent = NULL);

            ///Compare this scope to another.
            inline bool operator==(const varScope& other) const
            { return false; }

            bool addVar(const varSignature&);
            bool assignVar(const varSignature&);

            bool exists(const varSignature&);

            symID getVarUniqueID(const varSignature&);
            void* getVarType(const varSignature&);

            varSignature getVariable(const varSignature&);
        };

        /**
         * \brief Default constructor.
         *
         * \param _parent an optional pointer to
         * the scope one level outside of this one.
         */
        varScope::varScope(varScope* _parent)
        {
            parent = _parent;
        }

        /**
         * \brief Add a variable to this scope.
         *
         * \param _var the signature of the variable
         * to add to the scope.
         *
         * \return \b False if the
         * given signature already exists.
         * \b True otherwise.
         */
        bool varScope::addVar(const varSignature& _var)
        {
            if (vars.find(_var) > -1)
                return false;

            vars.add(_var);
            return true;
        }

        /**
         * \brief Add a variable to this scope if it
         * does not exist in any previous scope.
         *
         * \param _var the signature of the variable
         * to add to the scope.
         *
         * \return \b True if the variable did not exist
         * previously. \b False otherwise.
         */
        bool varScope::assignVar(const varSignature& _var)
        {
            if (!exists(_var))
            {
                vars.add(_var);
                return true;
            }
            else
                return false;
        }

        /**
         * \brief Check if a variable has been declared
         * in this scope or any of its parents.
         *
         * \param _var the signature of the variable to
         * look for.
         *
         * \return \b True if the variable has been
         * declared. \b False otherwise.
         */
        bool varScope::exists(const varSignature& _var)
        {
            varScope* _scope = this;

            while (_scope->vars.find(_var) < 0)
            {
                if (_scope->parent)
                    _scope = _scope->parent;
                else
                    return false;
            }

            return true;
        }

        /**
         * \brief Get the unique identifier associated with
         * the given variable.
         *
         * Searches this scope, then its parents for a
         * variable whose name matches the given signature's.
         *
         * \param _var the signature of the variable to
         * look for.
         *
         * \return The unique identifying value of the variable,
         * if it exists. \b 0 if it does not exist.
         */
        symID varScope::getVarUniqueID(const varSignature& _var)
        {
            varScope* _scope = this;

            int index = _scope->vars.find(_var);

            while (index < 0)
            {
                if (_scope->parent)
                    _scope = _scope->parent;
                else
                    return 0;

                index = _scope->vars.find(_var);
            }

            return _scope->vars[index].uniqueID;
        }

        /**
         * \brief Get the type associated with
         * the name of the given variable.
         *
         * Searches this scope, then its parents for a
         * variable whose name matches the given signature's.
         *
         * \param _var the signature of the variable to
         * look for.
         *
         * \return The type identifier of the variable,
         * if it exists. \b NULL if it does not exist.
         */
        void* varScope::getVarType(const varSignature& _var)
        {
            varScope* _scope = this;

            int index = _scope->vars.find(_var);

            while (index < 0)
            {
                if (_scope->parent)
                    _scope = _scope->parent;
                else
                    return NULL;

                index = _scope->vars.find(_var);
            }

            return _scope->vars[index].type;
        }

        /**
         * \brief Get the full signature of the variable with
         * the given name.
         *
         * Searches this scope, then its parents for a
         * variable whose name matches the given signature's.
         *
         * \param _var the signature of the variable to
         * look for.
         *
         * \return The full signature of the variable,
         * if it exists, and an empty signature if it does not.
         */
        varSignature varScope::getVariable(const varSignature& _var)
        {
            varScope* _scope = this;

            int index = _scope->vars.find(_var);

            while (index < 0)
            {
                if (_scope->parent)
                    _scope = _scope->parent;
                else
                    return varSignature(NULL);

                index = _scope->vars.find(_var);
            }

            return _scope->vars[index];
        }


    }
}
}

#endif // VARSCOPE_H_INCLUDED
