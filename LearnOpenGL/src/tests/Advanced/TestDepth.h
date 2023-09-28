#pragma once

#include "tests/Test.h"

#include "Model.h"

namespace  test {

	class TestDepth : public Test
	{
	public:
		TestDepth();
		~TestDepth();

		void OnRender(Camera camera) override;
		void OnImGuiRender() override;

	private:
		Model m_Model;
		Shader m_Shader;
	};
}