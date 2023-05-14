#pragma once

#include <glad/glad.h>

namespace Renderer {

	class VertexBuffer {
	public:
		VertexBuffer();
		~VertexBuffer();

		VertexBuffer& operator=(VertexBuffer&& vertexBuffer) noexcept;
		VertexBuffer(VertexBuffer&& indexBuffer) noexcept;

		void init(const void* data, const unsigned& size);
		void update(const void* data, const unsigned& size) const;
		void bind() const;
		void unbind() const;

	private:
		GLuint m_id;
	};

}