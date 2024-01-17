#include "glpch.h"

#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	SetupMesh();
}

void Mesh::Destroy()
{
	GLCall(glDeleteVertexArrays(1, &VAO));
	GLCall(glDeleteBuffers(1, &VBO));
	GLCall(glDeleteBuffers(1, &EBO));

	for (int i = 0, len = textures.size(); i < len; i++)
	{
		textures[i].Destroy();
	}
}

void Mesh::Draw(Shader& shader)
{
	unsigned int diffuseNr = 1;
	unsigned int specularNr = 1;
	unsigned int normalNr = 1;
	unsigned int heightNr = 1;

	for (unsigned int i = 0; i < textures.size(); i++)
	{
		// activate texture
		GLCall(glActiveTexture(GL_TEXTURE0 + i));
		std::string number;
		std::string name = textures[i].type;
		std::string number;
		std::string name = textures[i].type;
		std::string number;
		std::string name = textures[i].type;

		// retrieve texture info
		std::string name;
		switch (textures[i].texType)
		{
			case aiTextureType_DIFFUSE:
				name = "texture_diffuse" + std::to_string(diffuseNr++);
				break;
			case aiTextureType_NORMALS:
				name = "texture_normal" + std::to_string(normalNr++);
				shader.setBool("noNormalMap", false);
				break;
			case aiTextureType_SPECULAR:
				name = "texture_specular" + std::to_string(specularNr++);
				break;
			default:
				name = textures[i].name;
				break;
		}

		// set the shader value
		shader.setInt(name, i);
		// bind texture
		textures[i].Bind();
	}

	// draw mesh
	GLCall(glBindVertexArray(VAO));
	GLCall(glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(indices.size()), GL_UNSIGNED_INT, 0));
	GLCall(glBindVertexArray(0));

	// always good practice to set everything back to defaults once configured.
	GLCall(glActiveTexture(GL_TEXTURE0));
}

void Mesh::SetupMesh()
{
	// create buffers/arrays
	GLCall(glGenVertexArrays(1, &VAO));
	GLCall(glGenBuffers(1, &VBO));
	GLCall(glGenBuffers(1, &EBO));

	GLCall(glBindVertexArray(VAO));
	// load data into vertex buffers
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, VBO));
	GLCall(glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW));

	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO));
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW));

	// vertex Positions
	GLCall(glEnableVertexAttribArray(0));
	GLCall(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0));

	// vertex normals
	GLCall(glEnableVertexAttribArray(1));
	GLCall(glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal)));

	// vertex texture coords
	GLCall(glEnableVertexAttribArray(2));
	GLCall(glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords)));

	// vertex tangent
	GLCall(glEnableVertexAttribArray(3));
	GLCall(glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent)));

	// vertex bitangent
	GLCall(glEnableVertexAttribArray(4));
	GLCall(glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent)));

	// ids
	GLCall(glEnableVertexAttribArray(5));
	GLCall(glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), (void*)offsetof(Vertex, m_BoneIDs)));

	// weights
	GLCall(glEnableVertexAttribArray(6));
	GLCall(glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, m_Weights)));
	GLCall(glBindVertexArray(0));
}
