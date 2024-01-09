#include "VkBuffer.h"
#include "VkDevice.h"

Buffer::Buffer(BufferDescription description) \
    : m_Description(description) {
    VkDevice device = Device::Get().GetDevice();

    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = description.Size;
    bufferInfo.usage = description.GetVkBufferUsageFlagBits();
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(device, &bufferInfo, nullptr, &m_Buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create vertex buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(device, m_Buffer, &memRequirements);

    //Allocate and assgin memory to buffer
    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = FindMemoryType(memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    if (vkAllocateMemory(device, &allocInfo, nullptr, &m_BufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate vertex buffer memory!");
    }

    //Associate m_BufferMemory with m_Buffer
    vkBindBufferMemory(device, m_Buffer, m_BufferMemory, 0);
}

Buffer::~Buffer() {
	VkDevice device = Device::Get().GetDevice();
	vkDestroyBuffer(device, m_Buffer, nullptr);
	vkFreeMemory(device, m_BufferMemory, nullptr);
}

void Buffer::SetData(const void* data, uint64_t size) {
    VkDevice device = Device::Get().GetDevice();

	void* bufferData;
	vkMapMemory(device, m_BufferMemory, 0, size, 0, &bufferData);
	memcpy(bufferData, data, size);
	vkUnmapMemory(device, m_BufferMemory);
}

void Buffer::Bind(VkCommandBuffer commandBuffer) {
    VkBuffer vertexBuffers[] = { m_Buffer };
    VkDeviceSize offsets[] = { 0 };
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
}

uint32_t Buffer::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDevice physicalDevice = Device::Get().GetPhysicalDevice();

    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
}
