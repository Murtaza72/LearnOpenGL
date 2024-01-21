#pragma once

#include "tests/Test.h"

namespace test {

	class TestPBR : public Test
	{
	public:
		TestPBR();
		~TestPBR();

		void OnRender(Camera camera) override;
		void OnImGuiRender() override;

	private:
		Shader m_Shader;
		std::pair<std::vector<glm::vec3>, std::vector<glm::vec3> > m_Lights;
		Texture m_Albedo, m_Normal, m_Metallic, m_Rough, m_AO;

		int nrRows = 7;
		int nrColumns = 7;
		float spacing = 2.5;
	};
}
