#pragma once

#include "tests/Test.h"

namespace test {

	class TestInstancing : public Test
	{
	public:
		TestInstancing();
		~TestInstancing();

		void OnRender(Camera camera) override;
		void OnImGuiRender() override;

	private:
		Shader m_AsteroidShader, m_PlanetShader;
		Model m_PlanetModel, m_AsteroidModel;
		glm::mat4* m_ModelMatrices;
		int m_Amount;
	};
}

