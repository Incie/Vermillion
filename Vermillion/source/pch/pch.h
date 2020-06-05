#pragma once

#include<cstdlib>
#include"../log.h"

#ifdef _DEBUG
	#define _CRTDBG_MAP_ALLOC
	#include<crtdbg.h>

	#define vnew new(_NORMAL_BLOCK, __FILE__, __LINE__)

	void CheckOpenGLErrors(const char* statement, const char* sourceFileName, int lineNumber);

	#define GLCHECK(statement) statement; CheckOpenGLErrors(#statement, __FILE__, __LINE__);
	#define TRACE(tag) Log::Trace(tag, __FUNCTION__, __FILE__, __LINE__);
#else
	#define vnew new
	#define GLCHECK(statement) statement
	#define TRACE(tag) 
#endif

#include<vector>
#include<string>
#include<sstream>
#include<memory>
#include<algorithm>
#include<map>
#include<set>
#include<functional>

#include"glm\glm.hpp"
#include"fmt\format.h"
#include"../file.h"
#include"..\constants.h"
