#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <functional>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "imgui/imgui.h"
#include "stb_image/stb_image.h"

#include "Camera.h"
#include "Utils.h"

namespace test
{
	class Test {
	public:
		Test() {}
		virtual ~Test() {}

		virtual void OnRender(Camera camera) {}
		virtual void OnImGuiRender() {}
	};

	class TestMenu : public Test
	{
	public:
		TestMenu(Test*& currentTestPointer);

		template <typename T>
		void RegisterTest(const std::string& name)
		{
			std::cout << "Registering test " << name << std::endl;

			m_Tests.push_back(std::make_pair(name, []() { return new T(); }));
		}

		void OnImGuiRender() override;

	private:
		Test*& m_CurrentTest;
		std::vector<std::pair<std::string, std::function<Test* (void)>>> m_Tests;
	};
}