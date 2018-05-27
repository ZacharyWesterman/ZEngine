#pragma once
#ifndef DRIVER_H_INCLUDED
#define DRIVER_H_INCLUDED

namespace z
{
	namespace core
	{
		template <typename T>
		class array;
	}

	namespace util
	{
		class generic;
	}

	namespace engine
	{
		class program;

		typedef void zengine_gui;
		typedef void zengine_gfx;
		typedef void zengine_aud;

		class driver
		{
		public:
			zengine_gui* gui;
			zengine_gfx* graphics;
			zengine_aud* audio;

			core::array<program>* programs;
			// core::array<generic>

			driver(int argc, char** argv);
			~driver();

			
		};
	}
}

#endif //DRIVER_H_INCLUDED
