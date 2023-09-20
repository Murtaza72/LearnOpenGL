#include "TestClearColor.h"

namespace test {

	test::TestClearColor::TestClearColor()
		: m_Color{ 0.2f, 0.3f, 0.8f, 1.0f }
	{
	}

	void test::TestClearColor::OnRender(Camera camera)
	{
		glClearColor(m_Color[0], m_Color[1], m_Color[2], m_Color[3]);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void test::TestClearColor::OnImGuiRender()
	{
		ImGui::ColorPicker4("Clear Color", m_Color);
	}
}