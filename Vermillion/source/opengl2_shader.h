#pragma once

#include"file.h"
#include"glm/vec4.hpp"


enum class ShaderType {
	SHADER_VERTEX,
	SHADER_FRAGMENT,
	SHADER_GEOMETRY,
	SHADER_NOT_INITIALIZED
};

class GLSLShader
{
public:
	GLSLShader();
	~GLSLShader();

	bool Load(const FilePath& shader);
	void Unload();

	int shaderId;
	ShaderType shaderType;
};

class GLSLProgram
{
public:
	GLSLProgram();
	~GLSLProgram();

	bool LoadProgram(const std::string& shader);
	void UnloadProgram();

	void SetUniform(const std::string& uniformname, int value) const;
	void SetUniform(const std::string& uniformname, const glm::vec4& color) const;
	void SetUniform(const std::string& uniformname, const glm::vec3& color) const;

	void Use() const;
	void NoProgram() const;

private:
	int programId;
	GLSLShader fragment;
	GLSLShader vertex;
};