#pragma once

#include <vector>
#include <string>
#include "Sprite.h"

     //(RUS) ��������� ��������, ��������� ����� �� �������� � ������������, 
    //       ������ ���������� �� ������� ����� ������� � ��� ��������, 
   //        ����������� ��������� ���������, � ����� �� ��������� ��� ��������
  //(ENG) Animation state, describes frames from sprites and duration, 
 //       a list of sources from which this animation can jump, 
//        the default next state, and whether this animation can be interrupted

namespace Renderer{
	class StateAnimation {
	public:
		StateAnimation(const std::vector<std::pair<std::shared_ptr<Renderer::Sprite>, double>>& frames, const std::vector<std::string>& sources, const std::string& nextState, const bool& uninterrupted);

		bool canChange(const std::string& stateName);

		bool getUninterrupted();
		std::vector<std::pair<std::shared_ptr<Renderer::Sprite>, double>> getFrames();
		std::string getNextStateName();

	private:
		std::vector<std::pair<std::shared_ptr<Renderer::Sprite>, double>> m_frames;
		std::vector<std::string> m_sources;
		std::string m_nextState;
		bool m_uninterrupted;
	};
}
