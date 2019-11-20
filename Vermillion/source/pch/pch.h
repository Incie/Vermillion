#pragma once

#include<cstdlib>

#ifdef _DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include<crtdbg.h>

	#define vnew new(_NORMAL_BLOCK, __FILE__, __LINE__)
#else
	#define vnew new
#endif


#include<vector>
#include<string>
#include<sstream>
#include<algorithm>
#include<map>
#include<set>
#include<functional>

#include"glm\glm.hpp"
#include"fmt\format.h"
#include"..\constants.h"

void CheckOpenGLErrors(const char* statement, const char* sourceFileName, int lineNumber);

#ifdef _DEBUG
	#define GLCHECK(statement) statement; CheckOpenGLErrors(#statement, __FILE__, __LINE__);
#else
	#define GLCHECK(statement) statement
#endif

#include"../log.h"

#ifdef _DEBUG
	#define TRACE(tag) Log::Debug(tag, fmt::format("{}, {}, {}", __FUNCTION__, __FILE__, __LINE__));
#else
	#define TRACE(tag) 
#endif

