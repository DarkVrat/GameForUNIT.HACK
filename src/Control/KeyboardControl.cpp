#include "KeyboardControl.h"

#include <thread>

std::array<Control::KeyboardControl::E_BUTTON_ACTION, 349> Control::KeyboardControl::m_keys;
char Control::KeyboardControl::m_buffer;
bool Control::KeyboardControl::m_writeText;

namespace Control {
	 //(RUS) ��������� ������ ������� �������, �������� key
	//(ENG) setting the event of the pressed key, checking the key
	void KeyboardControl::setKey(GLFWwindow* pWindow, int key, int scancode, int action, int mode) {
		if (key == -1) return;

		if (action == GLFW_REPEAT) {
			m_keys[key] = E_BUTTON_ACTION::CLAMPED;
		}
		else if (action == GLFW_PRESS){
			m_keys[key] = E_BUTTON_ACTION::PRESSED;
		}
		else if (action == GLFW_RELEASE) {
			m_keys[key] = E_BUTTON_ACTION::RELEASED;
		}
	}

	 //(RUS) �������� ������ ������
	//(ENG) button event check
	bool KeyboardControl::ifPressed(const int& key) {
		if (m_keys[key] == E_BUTTON_ACTION::PRESSED) {
			m_keys[key] = E_BUTTON_ACTION::CLAMPED;
			return true;
		}
		return false;
	}
	bool KeyboardControl::ifClamped(const int& key) {
		if (m_keys[key] == E_BUTTON_ACTION::PRESSED || m_keys[key] == E_BUTTON_ACTION::CLAMPED) 
			return true;
		return false;
	}
	bool KeyboardControl::ifReleased(const int& key) {
		if (m_keys[key] == E_BUTTON_ACTION::PRESSED) {
			m_keys[key] = E_BUTTON_ACTION::NOT_CLAMPED;
			return true;
		}
		return false;
	}

	 //(RUS) ��������� WritingText �� true ��� ������ ����� ������, ��������� WritingText � ������ � �������
	//(ENG) Setting WritingText to true for text input mode, getting WritingText and text buffer
	void KeyboardControl::startWritingText() { m_writeText = true; }
	void KeyboardControl::endWritingText() { m_writeText = false; }

	 //(RUS) ���������� ������� � �����. ��������� �������� ��� ������������ ������� ���������
	//(ENG) Adding a character to the buffer. changing the value to match the Cyrillic character
	void KeyboardControl::addCharInBuffer(GLFWwindow* ����, unsigned codepoint) {
		if (m_writeText) {
			if (codepoint > 128)  {codepoint-= 848;}
			if (codepoint == 7622){codepoint = 185;}
			if (codepoint == 257) {codepoint = 184;}
			if (codepoint == 177) {codepoint = 168;}
			m_buffer = (char)codepoint;
		} 
	}

	bool KeyboardControl::getWritingText(){
		return m_writeText;
	}

	char KeyboardControl::getBuffer(){
		char res = m_buffer;
		m_buffer = 0;
		return res;
	}

	 //(RUS) �������� � ���������� ���������� ���������� �����������
	//(ENG) creating and filling variables with initial parameters
	KeyboardControl::KeyboardControl() { 
		m_keys.fill(E_BUTTON_ACTION::NOT_CLAMPED); m_buffer = 0; m_writeText = false;
	}
}