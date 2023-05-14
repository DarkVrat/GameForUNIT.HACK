 #pragma once

#include <glm/vec3.hpp>
#include <map>
#include <memory>
#include <vector>
#include <iostream>
#include <functional>
#include <glm/gtx/hash.hpp>
#include "Texture2D.h"
#include "VertexArray.h"

#define PRINT_TEXT Renderer::PrintText
#define TEXT Renderer::PrintText::Text
#define CENTR true
#define LEFT false

 //(RUS) ����� ��� ���������� ������
//(ENG) Text rendering class

namespace Renderer {
	class PrintText {

	public:
		 //(RUS) ��������� ����������� ����� ��� ���������� �� �������
	    //(ENG) Structure describing text to render from buffers
		struct Text {
			std::string ms_text;
			glm::vec3 ms_position;
			GLfloat ms_scale;
			glm::vec3 ms_color;

			Text(const std::string& text, const glm::vec3& position, const GLfloat& scale=0.01, const glm::vec3& color=glm::vec3(1,1,1))
				:ms_text(text), ms_position(position), ms_scale(scale), ms_color(color) {}
			Text() :ms_text(""), ms_position(glm::vec3(0, 0, 0)), ms_scale(0.01), ms_color(glm::vec3(1, 1, 1)) {}
		};

		static void init(const std::string& fontPath, const std::shared_ptr<Texture2D>& texture);

		static void printTextWrapping(Text text, float size, const bool& centr = LEFT, const double& Time = -1.0);
		static void printTextReduction(Text text, float size, const bool& centr = LEFT, const double& Time = -1.0);
		static void printText(const Text& text, const double& Time = -1.0);

		static void updateBuffer(const double& duration);
		static void renderBuffer();

		static float sizeText(Text text) { return sizeText(text.ms_text, text.ms_scale); }
		static float sizeText(std::string text, GLfloat scale);

		static void terminate();

		static void setWindow(const glm::vec2& window);
	private:
		PrintText();
		~PrintText() {};

		template <class T>
		static void hash_combine(size_t& seed, const T& value);
		static std::size_t getHash();

		static std::vector<float> m_advanceChar;
		static std::vector<glm::vec4> m_textureChar;
		static std::shared_ptr<Texture2D> m_texture;
		static glm::vec2 m_window;

		static std::shared_ptr<VertexArray> m_VAO;
		static VertexBuffer m_PositionVBO;
		static VertexBuffer m_ColorVBO;
		static VertexBuffer m_TextureVBO;
		static float m_fontSize;

		static std::vector<std::pair<Text, double>> m_timeBufferText;
		static std::vector<Text> m_bufferText;
		static size_t m_hashBuffer;
	};

	template<class T>
	inline void PrintText::hash_combine(size_t& seed, const T& value){
		std::hash<T> hasher;
		seed ^= hasher(value) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
	}
}