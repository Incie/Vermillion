#pragma once

#include<vector>
#include<string>
#include<Windows.h>
#include<vulkan/vulkan.h>

class VulkanRenderer 
{
public:
	VulkanRenderer();
	~VulkanRenderer();

	void SetHandles(HWND hWnd);

	bool Initialize();
	void Deinitialize();

private:
	void CreateInstance();
	void EnableValidationLayers();
	void AddRequiredExtension(const std::string& ext);
	void PickPhysicalDevice();
	void CreateLogicalDevice();
	void CreateSurface();
	void CreateSwapchain();
	void CreateImageViews();
	void CreatePipeline();
	void CreateRenderpass();
	void CreateFramebuffers();
	void CreateCommandPool();
	void CreateCommandBuffers();

	VkInstance instance;
	VkDebugUtilsMessengerEXT debugMessenger;
	VkPhysicalDevice physicalDevice;
	VkDevice device;
	VkQueue graphicsQueue;
	VkQueue presentQueue;
	VkSurfaceKHR surface;
	VkRenderPass renderPass;
	VkPipelineLayout pipelineLayout;
	VkPipeline graphicsPipeline;
	VkCommandPool commandPool;

	std::vector<VkFramebuffer> swapChainFramebuffers;
	std::vector<VkCommandBuffer> commandBuffers;

	VkSwapchainKHR swapChain;
	std::vector<VkImage> swapChainImages;
	std::vector<VkImageView> swapChainImageViews;
	VkFormat swapChainImageFormat;
	VkExtent2D swapChainExtent;

	VkShaderModule CreateShaderModule(const std::vector<char>& bytes);

	std::vector<const char*> requiredExtensionsC;
	std::vector<std::string> requiredExtensions;

	HWND hWnd;
};