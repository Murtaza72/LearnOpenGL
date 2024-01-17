#pragma once

#include "tests/Test.h"

struct DirectionalLight
{
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	bool enable;
};

struct PointLight
{
	glm::vec3 position;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constant;
	float linear;
	float quadratic;

	glm::vec3 color;
	bool enable;
};

struct SpotLight
{
	glm::vec3 position;
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float cutOff;
	float outerCutOff;

	bool enable;
};

namespace test {

	class TestMultipleLights : public Test
	{
	public:
		TestMultipleLights();
		~TestMultipleLights();

		void OnRender(Camera camera) override;
		void OnImGuiRender() override;

	private:
		unsigned int m_VBO, m_LightCubeVAO, m_CubeVAO;
		Shader m_LightingShader, m_LightCubeShader;
		Camera m_Camera;
		Texture m_DiffuseMap, m_SpecularMap;

		glm::vec3 m_LightPos, m_ClearColor;
		DirectionalLight m_DirLight;
		SpotLight m_SpotLight;
		std::vector<PointLight> m_PointLights;
		std::vector<glm::vec3> m_CubePositions;
		float m_RotationSpeed;
	};
}