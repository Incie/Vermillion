#include"pch.h"
#include"vulkan.h"
#include"vulkan_common.h"
#include"vulkan_validator.h"

void VulkanRenderer::CreateSurface()
{
    CreateWin32Surface();
}