#pragma once
#ifndef INST_H_INCLUDED
#define INST_H_INCLUDED

namespace z
{
	namespace engine
	{
		//zengine instruction list
		enum inst
		{
			//Arithmetic
			ADD,
			SUB,
			MUL,
			DIV,
			IDIV,
			REMD,
			POW,
			FAC,

			//Assign
			CONST,
			ASGMEM,
			MEMALLOC,
			MEMFREE,
			MEMSIZE,
			DEFCMD,
			DEFFUNC,

			//Shared
			ASGSHARED,
			GETSHARED,

			//Comparison
			EQ,
			NE,
			GT,
			GE,
			LT,
			LE,

			//Boolean (Logical)
			AND,
			OR,
			NAND,
			NOR,
			XOR,
			XNOR,
			NOT,

			//Boolean (Bitwise)
			ANDB,
			ORB,
			NANDB,
			NORB,
			XORB,
			XNORB,
			NOTB,

			//Datatype (Get Convert)
			REDUCE,
			POINTER,
			ARRAY,
			STRING,
			COMPLEX,
			FLOAT,
			INT,
			BOOL,

			//Datatype (Get Status)
			TYPE,
			TYPESTR,
			ISPOINTER,
			ISARRAY,
			ISSTRING,
			ISARITH,
			ISCOMPLEX,
			ISFLOAT,
			ISINT,
			ISNULL,

			//Array Manip
			ARRAYIZE,
			APPEND,
			INSERT,
			DELETE,
			GETELEM,
			SETELEM,
			ELEMCT,
			CONCAT,

			//Control
			JUMPTRUE,
			JUMPFALSE,
			JUMP,
			JUMPPUSH,
			RETURN,

			//Language
			INITGUI,
			INITENGINE,
			INITAUDIO,
			TERMGUI,
			TERMENGIN,
			TERMAUDIO,
			SETGOALFPS,
			GETGOALFPS,
			SETGOALIPS,
			GETGOALIPS,
			WAIT,
			SETERRLOW,
			SETERRMED,
			SETERRSEVERE,
			SETERRIGNORE,
			EXITONERROR,
			GETLASTERROR,
			EXIT,
			RUN,
			CPLRUN,
			CHECKSTATUS,
			WAITEXIT,
			WAITANYEXIT,
			WAITALLEXIT,
			KILLPROGRAM,
			RUNCMD,
			CALLFUNC,

			//Debug
			DEBUGLINE,
			DEBUGFUNCTION,
			DEBUGFILE,

			//Instruction count
			INST_COUNT
		};


		typedef unsigned char opCode;
		typedef unsigned int memID;
		typedef unsigned int instrID;
	}
}

#endif //INST_H_INCLUDED
