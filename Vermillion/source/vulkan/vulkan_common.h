#pragma once

#include<vulkan/vulkan.h>

#ifdef _DEBUG
#include<assert.h>
#include<string>
#include"../log.h"

//from vulkan_error_helpers.cpp
std::string VkResult_To_String(VkResult result);

#define VKCHECK(statement)\
do {\
    VkResult vk_result = statement;\
    if(vk_result != VK_SUCCESS) {\
        auto cstrStatement = #statement;\
        auto errorMessage = fmt::format("{} failed with {}", cstrStatement, VkResult_To_String(vk_result));\
        Log::Error("VKCHECK", errorMessage);\
        Log::Trace("VKCHECK", __FUNCTION__, __FILE__, __LINE__);\
        assert(0);\
    }\
} while(0); 
#else
#define VKCHECK(statement)
#endif