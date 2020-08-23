#include"pch.h"
#include"vulkan_validator.h"
#include"vulkan_common.h"

static const char* tag = "VulkanValidator";

void vulkan::CheckRequiredExtensions(const std::vector<std::string>& requiredExtensions)
{
    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> vulkanExtensions(extensionCount);
    if(vulkanExtensions.capacity() < extensionCount)
        throw std::string("vector capacity not correct");

    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, vulkanExtensions.data());

    Log::Info(tag, "Extensions");
    for(auto ex : vulkanExtensions) {
        Log::Info(tag, fmt::format("{}:{}", ex.extensionName, ex.specVersion));
    }

    Log::Info(tag, "Checking Required Extensions");
    for(auto requiredExtension : requiredExtensions) {

        auto found = std::find_if(vulkanExtensions.begin(), vulkanExtensions.end(), [](const auto& vext) { return true; });
        if(found == vulkanExtensions.end()) {
            Log::Error(tag, fmt::format("Extension [{}] not found", requiredExtension));
            throw fmt::format("Extension [{}] not found", requiredExtension);
        }

        Log::Info(tag, fmt::format("Extension [{}] found", requiredExtension));
    }
}

void vulkan::CheckValidationLayers()
{
    uint32_t layerCount = 0;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
    std::vector<VkLayerProperties> layers(layerCount);

    vkEnumerateInstanceLayerProperties(&layerCount, layers.data());

    Log::Info(tag, "Enumerating Layers");
    for(const auto& layer : layers)
        Log::Info(tag, fmt::format("[Name: {}][Description: {}][impver: {}][specver{}]",
            layer.layerName,
            layer.description,
            layer.implementationVersion,
            layer.specVersion));
}

void vulkan::CheckDeviceExtensionSupport(VkPhysicalDevice physicalDevice)
{
    uint32_t extensionCount = 0;
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, nullptr);
    std::vector<VkExtensionProperties> extensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(physicalDevice, nullptr, &extensionCount, extensions.data());

    std::set<const char*> requiredExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

    for(auto extension : extensions) {
        Log::Info(tag, fmt::format("Vulkan Device Extensions: [{}] [{}]", extension.extensionName, extension.specVersion));

        auto extensionFound = std::find_if(requiredExtensions.begin(), requiredExtensions.end(), [&extension](auto ext){ return strcmp(extension.extensionName, ext) == 0; });
        if( extensionFound != requiredExtensions.end()) {
            requiredExtensions.erase(extensionFound);
        }
    }

    if(!requiredExtensions.empty())
        throw std::string("Device Extension not found");
}

void vulkan::LogPhysicalDevice(VkPhysicalDevice physicalDevice)
{

    VkPhysicalDeviceProperties deviceProperties;
    vkGetPhysicalDeviceProperties(physicalDevice, &deviceProperties);
    
    Log::Info(tag, fmt::format("DeviceProperties: [{}] [{}] [{}] [{}] [{}] [{}]", 
        deviceProperties.apiVersion, 
        deviceProperties.deviceID, 
        deviceProperties.deviceName, 
        deviceProperties.deviceType, 
        deviceProperties.driverVersion,
        deviceProperties.vendorID));

    VkPhysicalDeviceFeatures deviceFeatures;
    vkGetPhysicalDeviceFeatures(physicalDevice, &deviceFeatures);
    Log::Info(tag, fmt::format("DeviceFeatures: [geometryshader: {}] [tesselationshader: {}] ",
        deviceFeatures.geometryShader,
        deviceFeatures.tessellationShader));
}

vulkan::QueueFamilies vulkan::EnumerateQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface)
{
    vulkan::QueueFamilies queues{};

    uint32_t queueCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueCount, queueFamilies.data());

    Log::Info(tag, fmt::format("Queues {}", queueFamilies.size()));
    uint32_t index = 0;
    for(auto queue : queueFamilies) {
        Log::Info(tag, fmt::format("queue #{} [count: {} flags: [{}]", index, queue.queueCount, queue.queueFlags));

        if(queue.queueFlags & VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT) {
            Log::Info(tag, fmt::format("{}:{}", "Graphics Bit", static_cast<int>(VkQueueFlagBits::VK_QUEUE_GRAPHICS_BIT)));

            if( queues.graphics.has_value() == false)
                queues.graphics = index;
        }

        if(queue.queueFlags & VkQueueFlagBits::VK_QUEUE_COMPUTE_BIT) {
            Log::Info(tag, fmt::format("{}:{}", "Compute Bit", static_cast<int>(VkQueueFlagBits::VK_QUEUE_COMPUTE_BIT)));
        }

        if(queue.queueFlags & VkQueueFlagBits::VK_QUEUE_PROTECTED_BIT) {
            Log::Info(tag, fmt::format("{}:{}", "Protected Bit", static_cast<int>(VkQueueFlagBits::VK_QUEUE_PROTECTED_BIT)));
        }

        if(queue.queueFlags & VkQueueFlagBits::VK_QUEUE_SPARSE_BINDING_BIT) {
            Log::Info(tag, fmt::format("{}:{}", "Sparse binding Bit", static_cast<int>(VkQueueFlagBits::VK_QUEUE_SPARSE_BINDING_BIT)));
        }

        if(queue.queueFlags & VkQueueFlagBits::VK_QUEUE_TRANSFER_BIT) {
            Log::Info(tag, fmt::format("{}:{}", "Transfer Bit", static_cast<int>(VkQueueFlagBits::VK_QUEUE_TRANSFER_BIT)));
        }

        VkBool32 presentSupport = false;
        VKCHECK(vkGetPhysicalDeviceSurfaceSupportKHR(physicalDevice, index, surface, &presentSupport));

        if(presentSupport) {
            Log::Info(tag, "Present 'Bit'");

            if( queues.present.has_value() == false )
                queues.present = index;
        }

        index++;
    }

    Log::Info(tag, fmt::format("Chosen familyindices {} {}", queues.present.value(), queues.graphics.value()));

    return queues;
}

vulkan::SwapChainSupportDetails vulkan::EnumerateSwapchainDetails(VkSurfaceKHR surface, VkPhysicalDevice physicalDevice)
{
    vulkan::SwapChainSupportDetails details;

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &details.capabilities);

    uint32_t formatCount;
    vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr);

    if(formatCount != 0) {
        details.formats.resize(formatCount);
        vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount;
    vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr);

    if(presentModeCount != 0) {
        details.presentModes.resize(presentModeCount);
        vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, details.presentModes.data());
    }

    return details;
}
