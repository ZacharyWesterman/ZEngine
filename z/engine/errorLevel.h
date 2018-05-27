#pragma once
#ifndef ERRORLEVEL_H_INCLUDED
#define ERRORLEVEL_H_INCLUDED

namespace z
{
	namespace engine
	{
		enum errorLevel
		{
			NO_ERROR = 0,

			LOW,
			MED,
			HIGH,
			SEVERE,

			ERRORLEVEL_COUNT
		};
	}
}

#endif //ERRORLEVEL_H_INCLUDED
