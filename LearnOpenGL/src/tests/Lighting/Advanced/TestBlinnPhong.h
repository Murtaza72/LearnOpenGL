#pragma once

#include "tests/Test.h"

namespace test {

	class TestBlinnPhong : public Test
	{
	public:
		TestBlinnPhong();
		~TestBlinnPhong();

		void OnRender(Camera camera) override;
		void OnImGuiRender() override;

	private:
		unsigned int m_VAO, m_VBO;
		Texture m_FloorTex;
		Shader m_Shader;

		glm::vec3 m_LightPos;
		bool m_Blinn;
		int m_Shininess;
	};
}
