#pragma once
#ifndef IDENTITY_H_INCLUDED
#define IDENTITY_H_INCLUDED

#include <z/core/string.h>
#include <z/int.h>
#include <z/char.h>

namespace z
{
	namespace compiler
	{
		class identity
		{
		public:
			const Int type;
			const Int subType;

			const core::string<Char>* symbol;

			const Int line;
			const Int column;
			const Int indent;


			//Constructor
			identity(Int Type, Int SubType, const core::string<Char>* Symbol, Int Line, Int Column, Int Indent) :
				type(Type), subType(SubType), symbol(Symbol), line(Line), column(Column), indent(Indent)
			{}


			//equality operators

			bool operator==(const identity& other) const
			{
				return (type == other.type) && (subType == other.subType);
			}

			bool operator!=(const identity& other) const
			{
				return (type != other.type) || (subType != other.subType);
			}

			bool operator>(const identity& other) const
			{
				return (type > other.type) || (subType > other.subType);
			}

			bool operator>=(const identity& other) const
			{
				return (*this > other) || (*this == other);
			}

			bool operator<(const identity& other) const
			{
				return (type < other.type) || (subType < other.subType);
			}

			bool operator<=(const identity& other) const
			{
				return (*this < other) || (*this == other);
			}
		};
	}
}

#endif // IDENTITY_H_INCLUDED
