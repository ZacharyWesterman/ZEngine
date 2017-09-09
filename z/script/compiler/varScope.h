/**
 * File:            varScope.h
 * Namespace:       z::script
 * Description:     Structs for keeping track of
 *                  defined variables and in what
 *                  scopes they are defined.
 *
 *
 * Author:          Zachary Westerman
 * Email:           zacharywesterman@yahoo.com
 * Last modified:   26 Aug. 2017
**/

#pragma once
#ifndef VARSCOPE_H_INCLUDED
#define VARSCOPE_H_INCLUDED

namespace z
{
    namespace script
    {
        typedef unsigned long symID;

        struct varSignature
        {
            void* ID;
            void* type;

            symID uniqueID;

            inline bool operator==(const varSignature& other) const
            { return (ID == other.ID); }

            varSignature(void* _ID, unsigned long _uniqueID = 0, void* _type = NULL)
            {
                ID = _ID;
                type = _type;

                uniqueID = _uniqueID;
            }
        };

        struct varScope
        {
            varScope* parent;

            core::array< varSignature > vars;

            core::array< varScope > children;

            varScope(varScope* _parent = NULL)
            { parent = _parent; }

            inline bool operator==(const varScope& other) const
            { return false; }

            errorFlag addVar(const varSignature&);
            bool assignVar(const varSignature&);

            bool exists(const varSignature&);

            symID getVarUniqueID(const varSignature&);
            void* getVarType(const varSignature&);

            varSignature getVariable(const varSignature&);
        };

        errorFlag varScope::addVar(const varSignature& _var)
        {
            if (vars.find(_var) > -1)
                return error::VARIABLE_REDECLARED;

            vars.add(_var);
            return error::NONE;
        }

        //returns false if variable was previously defined, true otherwise.
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

#endif // VARSCOPE_H_INCLUDED
