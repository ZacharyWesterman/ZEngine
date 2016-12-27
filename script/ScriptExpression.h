#pragma once
#ifndef SCRIPTEXPRESSION_H_INCLUDED
#define SCRIPTEXPRESSION_H_INCLUDED

#include "variableTree.h"
#include "functionalExpression.h"

#ifndef uint
    #define uint uint32_t
#endif // uint

namespace script
{
    template <typename CHAR>
    class ScriptExpression
    {
    private:
        variableTree<CHAR>* var_tree;
        functionalExpression<CHAR>* expr_handler;

    public:
        ScriptExpression() {var_tree = NULL; expr_handler = NULL;}
        ScriptExpression(const variableTree<CHAR>& Vars, const functionalExpression<CHAR>& Handler)
        {
            var_tree = &Vars;
            expr_handler = &Handler;
        }

        void bind(const variableTree<CHAR>& Vars, const functionalExpression<CHAR>& Handler)
        {
            var_tree = &Vars;
            expr_handler = &Handler;
        }

        bool good() { return (var_tree && expr_handler); }


        uint process(const core::string<CHAR>&,
                     const core::array< core::string<CHAR> >&);
    };



    template <typename CHAR>
    uint ScriptExpression<CHAR>::process(const core::string<CHAR>& input,
                                         const core::array< core::string<CHAR> >& output)
    {
        uint script_error = ERROR::NONE;


    }
}

#endif // SCRIPTEXPRESSION_H_INCLUDED
