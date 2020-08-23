#include"pch.h"
#include"vulkan.h"
#include"vulkan_common.h"
#include"vulkan_validator.h"

static constexpr auto tag = "VulkanRenderer:Device";

void VulkanRenderer::PickPhysicalDevice()
{
    physicalDevice = VK_NULL_HANDLE;
    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if(deviceCount == 0) {
        std::string errorMessage = "No physical devices found";
        Log::Error(tag, errorMessage);
        throw std::string(errorMessage);
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    if(deviceCount > 1) {
        throw std::string("More than one gpu found");
    }

    physicalDevice = devices[0];
    vulkan::CheckDeviceExtensionSupport(physicalDevice);
    vulkan::LogPhysicalDevice(physicalDevice);
}

void VulkanRenderer::CreateLogicalDevice()
{
    auto queueFamilies = vulkan::EnumerateQueueFamilies(physicalDevice, surface);
    std::set<uint32_t> createQueueFamilySet = {queueFamilies.graphics.value(), queueFamilies.present.value()};

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    float queuePriority = 1.0f;
    for(uint32_t queueFamilyId : createQueueFamilySet) {
        VkDeviceQueueCreateInfo queueCreateInfo{VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO};
        queueCreateInfo.queueFamilyIndex = queueFamilyId;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;

        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};
    deviceFeatures.samplerAnisotropy = VK_TRUE;
    //deviceFeatures.geometryShader = true;
    //deviceFeatures.tessellationShader = true;

    VkDeviceCreateInfo createInfo{VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO};
    createInfo.pQueueCreateInfos = queueCreateInfos.data();
    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
    createInfo.pEnabledFeatures = &deviceFeatures;
    createInfo.enabledLayerCount = 0;

    std::vector<const char*> deviceExtensions = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};
    createInfo.enabledExtensionCount = 1;
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    VKCHECK(vkCreateDevice(physicalDevice, &createInfo, nullptr, &device));

    vkGetDeviceQueue(device, queueFamilies.graphics.value(), 0, &graphicsQueue);
    vkGetDeviceQueue(device, queueFamilies.present.value(), 0, &presentQueue);
}

