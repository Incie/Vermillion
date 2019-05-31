#pragma once

#define _CRTDBG_MAP_ALLOC
#include<cstdlib>
#include<crtdbg.h>

#define vnew new(_NORMAL_BLOCK, __FILE__, __LINE__)

#include<vector>
#include<string>
#include<sstream>
#include<algorithm>
#include<map>
#include<set>
#include<functional>

#include"glm\glm.hpp"
#include"fmt\format.h"

