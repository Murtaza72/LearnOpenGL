#pragma once

#include "tests/Test.h"

namespace test {

	class TestModel :public Test
	{
	public:
		TestModel();
		~TestModel();

		void OnRender(Camera camera) override;
		void OnImGuiRender() override;

	private:
		Shader m_Shader;
		Model m_Model;
	};
}