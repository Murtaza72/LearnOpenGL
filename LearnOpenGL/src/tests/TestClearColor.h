#pragma once

#include "Test.h"

namespace test {

	class TestClearColor : public Test
	{
	public:
		TestClearColor();

		void OnRender(Camera camera) override;
		void OnImGuiRender() override;

	private:
		float m_Color[4];
	};
}