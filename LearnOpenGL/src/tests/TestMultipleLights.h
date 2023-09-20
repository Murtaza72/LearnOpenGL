#pragma once

#include "Test.h"

struct DirectionalLight {
	glm::vec3 direction;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;
};

struct PointLight {
	glm::vec3 position;
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	float constant;
	float linear;
	float quadratic;
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
};

namespace test
{
	class TestMultipleLights : public Test {
	public:
		TestMultipleLights();
		~TestMultipleLights();

		void OnRender(Camera camera) override;
		void OnImGuiRender() override;

	private:
		unsigned int m_VBO, m_LightCubeVAO, m_CubeVAO;
		Shader m_LightingShader, m_LightCubeShader;
		Camera m_Camera;
		glm::vec3 m_LightPos;
		unsigned int m_DiffuseMap, m_SpecularMap;
		std::vector<glm::vec3> m_CubePositions, m_PointLightPos, m_PointLightColor;
		float m_RotationSpeed;
		DirectionalLight m_DirLight;
		PointLight m_PointLight;
		SpotLight m_SpotLight;
		bool m_EnableDirLight, m_EnablePointLight, m_EnableSpotLight;
	};
}