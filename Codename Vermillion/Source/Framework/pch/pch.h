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
#include"..\Framework\constants.h"

