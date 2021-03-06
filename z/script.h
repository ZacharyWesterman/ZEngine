/**
 * \file z/script.h
 * \namespace z::script
 * \brief Library containing all script-related
 * functions and classes.
 *
 * This library adds advanced functionality
 * including generic data types and the script
 * compiler.
**/

#pragma once
#ifndef SCRIPT_H_INCLUDED
#define SCRIPT_H_INCLUDED

///Basic supporting files
#include "script/error.h"
#include "script/generic.h"

///Extensible script parts
#include "script/function.h"
#include "script/command.h"

///
#include "script/compiler.h"

#endif // SCRIPT_H_INCLUDED
