#pragma once
#include <vulkan/vulkan.h>

#include <stdexcept>
#include <vector>

enum class BufferType {
	VertexBuffer,
	IndexBuffer,
	UniformBuffer,
	StorageBuffer
};

enum class ShaderDataType {
	None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, UInt, Int, Int2, Int3, Int4, Bool
};

static uint32_t ShaderDataTypeSize(ShaderDataType type) {
	switch (type) {
		case ShaderDataType::Float:	  return 4;
		case ShaderDataType::Float2:  return 4 * 2;
		case ShaderDataType::Float3:  return 4 * 3;
		case ShaderDataType::Float4:  return 4 * 4;
		case ShaderDataType::Mat3:	  return 4 * 3 * 3;
		case ShaderDataType::Mat4:	  return 4 * 4 * 4;
		case ShaderDataType::UInt:	  return 4;
		case ShaderDataType::Int:	  return 4;
		case ShaderDataType::Int2:	  return 4 * 2;
		case ShaderDataType::Int3:	  return 4 * 3;
		case ShaderDataType::Int4:	  return 4 * 4;
		case ShaderDataType::Bool:	  return 1;
	}
	std::runtime_error("Unknown ShaderDataType");

	return 0;
}

struct BufferElement {
	std::string Name;
	ShaderDataType Type;
	uint32_t Size;
	size_t Offset;
	uint32_t InstancedDivisor;
	bool Normalized;

	BufferElement() = default;

	BufferElement(ShaderDataType type, const std::string& name, uint32_t instancedDivisor = 0, bool normalized = false)
		: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), InstancedDivisor(instancedDivisor), Normalized(normalized) { }

	VkFormat GetVulkanType() const {
		switch (Type) {
			case ShaderDataType::None:		break;
			case ShaderDataType::Float:		return VK_FORMAT_R32_SFLOAT;
			case ShaderDataType::Float2:	return VK_FORMAT_R32G32_SFLOAT;
			case ShaderDataType::Float3:	return VK_FORMAT_R32G32B32_SFLOAT;
			case ShaderDataType::Float4:	return VK_FORMAT_R32G32B32A32_SFLOAT;
			case ShaderDataType::Mat3:		break;
			case ShaderDataType::Mat4:		break;
			case ShaderDataType::UInt:		return VK_FORMAT_R32_UINT;
			case ShaderDataType::Int:		return VK_FORMAT_R32_SINT;
			case ShaderDataType::Int2:		return VK_FORMAT_R32G32_SINT;
			case ShaderDataType::Int3:		return VK_FORMAT_R32G32B32_SINT;
			case ShaderDataType::Int4:		return VK_FORMAT_R32G32B32A32_SINT;
			case ShaderDataType::Bool:		return VK_FORMAT_R8_UINT;
		}

		std::runtime_error("ShaderDataType not convertable to Vulkan format");
		return VK_FORMAT_UNDEFINED;
	}

	uint32_t GetComponentCount() const {
		switch (Type) {
			case ShaderDataType::Float:  return 1;
			case ShaderDataType::Float2: return 2;
			case ShaderDataType::Float3: return 3;
			case ShaderDataType::Float4: return 4;
			case ShaderDataType::Mat3:   return 3; // 3* float3
			case ShaderDataType::Mat4:   return 4; // 4* float4
			case ShaderDataType::UInt:	 return 1;
			case ShaderDataType::Int:	 return 1;
			case ShaderDataType::Int2:	 return 2;
			case ShaderDataType::Int3:	 return 3;
			case ShaderDataType::Int4:	 return 4;
			case ShaderDataType::Bool:	 return 1;
		}

		std::runtime_error("Unknown ShaderDataType!");
		return 0;
	}
};

class BufferLayout {
public:
	BufferLayout() = default;

	BufferLayout(std::initializer_list<BufferElement> elements) : m_Elements(elements) {
		CalculateOffsetsAndStride();
	}

	inline uint32_t GetStride() const { return m_Stride; }
	inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }

	std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
	std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
	std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
	std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
private:
	//Loops through all of the elemets that are passed into this class and calculates the offset and stride automatically
	void CalculateOffsetsAndStride() {
		size_t offset = 0;
		m_Stride = 0;
		for (auto& element : m_Elements) {
			element.Offset = offset;
			offset += element.Size;
			m_Stride += element.Size;
		}
	}

	std::vector<BufferElement> m_Elements;
	uint32_t m_Stride = 0;
};

struct BufferDescription {
	BufferType Type;
	uint64_t Size;

	BufferLayout Layout;

	VkBufferUsageFlagBits GetVkBufferUsageFlagBits() {
		switch (Type) {
			case BufferType::VertexBuffer:	return VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
			case BufferType::IndexBuffer:	return VK_BUFFER_USAGE_INDEX_BUFFER_BIT;
			case BufferType::UniformBuffer:	return VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
			case BufferType::StorageBuffer:	return VK_BUFFER_USAGE_STORAGE_BUFFER_BIT;
		}

		std::runtime_error("BufferType not supported");
	}
};

class Buffer {
public:
	Buffer(BufferDescription description);
	~Buffer();

	void SetData(const void* data, uint64_t size);
	void Bind(VkCommandBuffer commandBuffer);

	const BufferDescription& GetDescription() const { return m_Description; }
private:
	uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
private:
	VkBuffer m_Buffer;
	VkDeviceMemory m_BufferMemory;

	BufferDescription m_Description;
};
