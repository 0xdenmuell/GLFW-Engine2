#pragma once
#include <glad/glad.h>
#include <shader/shader.h>
#include <glm/glm/glm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <precom.h>


extern Camera cam;

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
};

struct Texture {
	unsigned int id;
	std::string type;
	std::string path;
};


class Mesh {
public:
	// mesh data
	std::vector<Vertex>       m_vertices;
	std::vector<unsigned int> m_indices;
	std::vector<Texture>      m_textures;

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) {
		m_vertices = vertices;
		m_indices = indices;
		m_textures = textures;

		setupMesh();
	}

	void Draw(Shader& shader) {

		shader.setVec3("viewPos", cam.m_position);

		shader.loadNormalLight();

		for (int i = 0; i < 1; i++)
		{
			shader.loadPointLight(cam.m_position, i);
		}

		unsigned int diffuseNr = 1;
		unsigned int specularNr = 1;

		for (unsigned int i = 0; i < m_textures.size(); i++)
		{
			glActiveTexture(GL_TEXTURE0 + i);

			unsigned int number;
			std::string name = m_textures[i].type;

			if (name == "texture_diffuse") {
				number = diffuseNr++;
			}
			else if (name == "texture_specular") {
				number = specularNr++;
			}
			shader.setInt("material." + name + std::to_string(number), i);
			glBindTexture(GL_TEXTURE_2D, m_textures[i].id); 
		}
		// draw mesh
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glActiveTexture(GL_TEXTURE0);
	}

private:
	//  render data
	unsigned int VAO, VBO, EBO;

	void setupMesh() {
		// Generate GLAD Objects
		glGenVertexArrays(1, &VAO);
		glGenBuffers(1, &VBO);
		glGenBuffers(1, &EBO);

		// Bind GLAD Objects to Buffer
		glBindVertexArray(VAO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, m_vertices.size() * sizeof(m_vertices), &m_vertices[0], GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_indices.size() * sizeof(unsigned int), &m_indices[0], GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
		glEnableVertexAttribArray(0);
		// unit attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
		glEnableVertexAttribArray(1);
		// texture attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0);
	}
};