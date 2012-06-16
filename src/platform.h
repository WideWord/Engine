
#ifndef PLATFORM_H
#define PLATFORM_H

#ifdef WINDOWS
	#ifdef DLL
		#define ENGINE_API __declspec(dllexport)
	#else
		#define ENGINE_API __declspec(dllimport)
	#endif
#else
	#define ENGINE_API
#endif

#ifndef nullptr
	#define nullptr __null
#endif


#include <string>
#include <vector>
#include <utility>
#include <list>
#include <iostream>
#include <sstream>


#endif
