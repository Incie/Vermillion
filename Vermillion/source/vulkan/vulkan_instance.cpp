#include"pch.h"
#include"vulkan.h"
#include"vulkan_common.h"
#include"vulkan_validator.h"

static constexpr auto tag = "VulkanRenderer:Instance";

VKAPI_ATTR uint32_t VKAPI_CALL debugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity, VkDebugUtilsMessageTypeFlagsEXT messageType, const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData, void* pUserData)
{
    Log::Debug("VulkanValidationLayer", fmt::format("{}: {}", pCallbackData->pMessageIdName, pCallbackData->pMessage));
    return VK_FALSE;
}

void VulkanRenderer::CreateInstance()
{
    TRACE(tag);
    Log::Info(tag, "Creating Vulkan Instance");

    VkApplicationInfo appInfo{VK_STRUCTURE_TYPE_APPLICATION_INFO};
    appInfo.pApplicationName = "Vulkan";
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pEngineName = "Vermillion";
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.apiVersion = VK_API_VERSION_1_0;

    const char* validationLayers[1] = {"VK_LAYER_KHRONOS_validation"};

    VkInstanceCreateInfo createInfo{VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO};
    createInfo.pApplicationInfo = &appInfo;
    createInfo.pNext = nullptr;
    createInfo.flags = 0;
    createInfo.enabledLayerCount = 1;
    createInfo.ppEnabledLayerNames = validationLayers;
    createInfo.enabledExtensionCount = static_cast<uint32_t>(requiredExtensionsC.size());
    createInfo.ppEnabledExtensionNames = requiredExtensionsC.data();

    vulkan::CheckRequiredExtensions(requiredExtensions);

    VKCHECK(vkCreateInstance(&createInfo, nullptr, &instance));

    vulkan::CheckValidationLayers();
    EnableValidationLayers();
}

void VulkanRenderer::EnableValidationLayers()
{
    VkDebugUtilsMessengerCreateInfoEXT createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    createInfo.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    createInfo.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    createInfo.pfnUserCallback = debugCallback;
    createInfo.pUserData = nullptr;

    auto vkCreateDebugUtilsMessengerEXT_ = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if(vkCreateDebugUtilsMessengerEXT_ != nullptr) {
        VKCHECK(vkCreateDebugUtilsMessengerEXT_(instance, &createInfo, nullptr, &debugMessenger));
    }
    else {
        Log::Error(tag, "DebugUtilsMessenger Not Found");
        //return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}