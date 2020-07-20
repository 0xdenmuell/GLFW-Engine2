#pragma once
#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

enum Objects
{
	OBJECT, LIGHT, MODEL
};
enum Textures {
	COLORFUL, SHREK, WOODENBOX, WOODENBOXFRAME
};


class Shader
{
public:
	Shader(Objects object)
	{
		currentPath = getCurrentPath();

		currentPath += "\\source\\shader";

		std::string vertexPath;
		std::string	fragmentPath;

		switch (object)
		{
		case OBJECT:
			vertexPath = currentPath + cubeVerticesPath;
			fragmentPath = currentPath + cubeFragmentPath;
			break;
		case LIGHT:
			vertexPath = currentPath + lightVerticesPath;
			fragmentPath = currentPath + lightFragmentPath;
			break;
		case MODEL:
			vertexPath = currentPath + modelVerticesPath;
			fragmentPath = currentPath + modelFragmentPath;
			break;
		default:
			LOG("NO OBJECT SELECTED (DEFAULT CASE");
			break;
		}

		char* vertexPath_c = UtilityFunc::StringToChar(vertexPath);
		char* fragmentPath_c = UtilityFunc::StringToChar(fragmentPath);

		std::string vertexCode;
		std::string fragmentCode;

		std::ifstream vShaderFile;
		std::ifstream fShaderFile;

		vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
		try
		{
			vShaderFile.open(UtilityFunc::StringToChar(vertexPath_c));
			fShaderFile.open(UtilityFunc::StringToChar(fragmentPath_c));
			std::stringstream vShaderStream, fShaderStream;

			vShaderStream << vShaderFile.rdbuf();
			fShaderStream << fShaderFile.rdbuf();

			vShaderFile.close();
			fShaderFile.close();

			vertexCode = vShaderStream.str();
			fragmentCode = fShaderStream.str();
		}
		catch (std::ifstream::failure& e)
		{
			std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		}
		const char* vShaderCode = vertexCode.c_str();
		const char* fShaderCode = fragmentCode.c_str();

		unsigned int vertex, fragment;

		vertex = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertex, 1, &vShaderCode, NULL);
		glCompileShader(vertex);
		checkCompileErrors(vertex, "VERTEX");

		fragment = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragment, 1, &fShaderCode, NULL);
		glCompileShader(fragment);
		checkCompileErrors(fragment, "FRAGMENT");

		ID = glCreateProgram();
		glAttachShader(ID, vertex);
		glAttachShader(ID, fragment);
		glLinkProgram(ID);
		checkCompileErrors(ID, "PROGRAM");

		glDeleteShader(vertex);
		glDeleteShader(fragment);
	}

	unsigned int getShaderID() {
		return ID;
	}

	void use()
	{
		glUseProgram(ID);
	}

	void setBool(const std::string& name, bool value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
	}
	void setInt(const std::string& name, int value) const
	{
		glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setFloat(const std::string& name, float value) const
	{
		glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
	}
	void setVec3(const std::string& name, const glm::vec3& value) const
	{
		glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
	}
	void setMatrix4(const std::string& name, glm::mat4 value) const
	{
		glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
	}

	static unsigned int loadMaterialTexture(std::string texturePath) {

		unsigned int textureID;
		glGenTextures(1, &textureID);
		LOG(textureID << " ------- " << texturePath);
		int width, height, channels;
		stbi_set_flip_vertically_on_load(true);
		unsigned char* textureData = stbi_load(UtilityFunc::StringToChar(texturePath), &width, &height, &channels, 0);
		
		if (textureData)
		{
			GLenum format;
			if (channels == 1)
				format = GL_RED;
			else if (channels == 3)
				format = GL_RGB;
			else if (channels == 4)
				format = GL_RGBA;

			glBindTexture(GL_TEXTURE_2D, textureID);
			glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, textureData);
			glGenerateMipmap(GL_TEXTURE_2D);
			
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			


			stbi_image_free(textureData);
		}
		else
		{
			LOG("TEXTURE FAILED TO LOAD AT PATH: " << texturePath);
			stbi_image_free(textureData);
		}
		return textureID;
	}


	void loadNormalLight() {
		glm::vec3 lightColor = glm::vec3(1.0f, 1.0f, 1.0f);

		glm::vec3 diffuseColor = lightColor * glm::vec3(0.5f);
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);

		setVec3("light.ambient", ambientColor);
		setVec3("light.diffuse", diffuseColor);
		setVec3("light.specular", glm::vec3(1.0f, 1.0f, 1.0f));
	}
	void loadDirLight(glm::vec3 direction = glm::vec3(-0.2f, -1.0f, -0.3f)) {

		setVec3("dirLight.direction", direction);
	}
	void loadPointLight(glm::vec3 position, int index) {

		switch (index)
		{
		case 0:
			setVec3("pointLight[0].position", position);
			setFloat("pointLight[0].constant", 1.0f);
			setFloat("pointLight[0].linear", 0.09f);
			setFloat("pointLight[0].quadratic", 0.032f);
			break;
		case 1:
			setVec3("pointLight[1].position", position);
			setFloat("pointLight[1].constant", 1.0f);
			setFloat("pointLight[1].linear", 0.09f);
			setFloat("pointLight[1].quadratic", 0.032f);
			break;
		case 2:
			setVec3("pointLight[2].position", position);
			setFloat("pointLight[2].constant", 1.0f);
			setFloat("pointLight[2].linear", 0.09f);
			setFloat("pointLight[2].quadratic", 0.032f);
			break;
		case 3:
			setVec3("pointLight[3].position", position);
			setFloat("pointLight[3].constant", 1.0f);
			setFloat("pointLight[3].linear", 0.09f);
			setFloat("pointLight[3].quadratic", 0.032f);
			break;
		case 4:
			setVec3("pointLight[4].position", position);
			setFloat("pointLight[4].constant", 1.0f);
			setFloat("pointLight[4].linear", 0.09f);
			setFloat("pointLight[4].quadratic", 0.032f);
			break;
		default:
			LOG("UNIFORM NAME IS DEFAULT");
			break;
		}
	}
	void loadSpotLight(glm::vec3 position, glm::vec3 direction) {
		setVec3("spotLight.position", position);
		setVec3("spotLight.direction", direction);

		setVec3("spotLight.ambient", glm::vec3(0.0f, 0.0f, 0.0f));
		setVec3("spotLight.diffuse", glm::vec3(1.0f, 1.0f, 1.0f));
		setVec3("spotLight.specular", glm::vec3(1.0f, 1.0f, 1.0f));

		setFloat("spotLight.constant", 1.0f);
		setFloat("spotLight.linear", 0.09f);
		setFloat("spotLight.quadratic", 0.032f);

		setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		setFloat("spotLight.outerCutOff", glm::cos(glm::radians(17.5f)));
	}

private:
	unsigned int ID;

	std::string currentPath;

	std::string getCurrentPath() {
		char pathChar[FILENAME_MAX];
		_getcwd(pathChar, FILENAME_MAX);

		std::string pathString = pathChar;
		return pathString;
	}

	std::string cubeVerticesPath = "\\glsl\\cubeVertices.glsl";
	std::string cubeFragmentPath = "\\glsl\\cubeFragment.glsl";
	std::string lightVerticesPath = "\\glsl\\lightVertices.glsl";
	std::string lightFragmentPath = "\\glsl\\lightFragment.glsl";
	std::string modelVerticesPath = "\\glsl\\modelVertices.glsl";
	std::string modelFragmentPath = "\\glsl\\modelFragment.glsl";

	std::string colorfulTexturePath = "\\ressources\\colorful.jpg";
	std::string shrekTexturePath = "\\ressources\\shrek.png";
	std::string woodenBoxTexturePath = "\\ressources\\woodenBox.png";
	std::string woodenBoxFrameTexturePath = "\\ressources\\woodenBoxFrame.png";

	void checkCompileErrors(unsigned int shader, std::string type)
	{
		int success;
		char infoLog[1024];
		if (type != "PROGRAM")
		{
			glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
			if (!success)
			{
				glGetShaderInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
		else
		{
			glGetProgramiv(shader, GL_LINK_STATUS, &success);
			if (!success)
			{
				glGetProgramInfoLog(shader, 1024, NULL, infoLog);
				std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
			}
		}
	}
};
#endif