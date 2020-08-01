#pragma once

#include<vulkan/vulkan.h>
#include<string>
#include<vector>
#include<optional>

namespace vulkan {
	void ValidateInstance(int result);
	void CheckRequiredExtensions(const std::vector<std::string>& requiredExtensions);
	void CheckValidationLayers();
	void CheckDeviceExtensionSupport(VkPhysicalDevice physicalDevice);

	
	struct QueueFamilies {
		std::optional<uint32_t> graphics;
		std::optional<uint32_t> present;

		bool isComplete() {
			return graphics.has_value() && 
				   present.has_value();
		}
	};

	void LogPhysicalDevice(VkPhysicalDevice physicalDevice);
	QueueFamilies EnumerateQueueFamilies(VkPhysicalDevice physicalDevice, VkSurfaceKHR surface);

	struct SwapChainSupportDetails {
		VkSurfaceCapabilitiesKHR capabilities;
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	SwapChainSupportDetails EnumerateSwapchainDetails(VkSurfaceKHR surface, VkPhysicalDevice physicalDevice);
};