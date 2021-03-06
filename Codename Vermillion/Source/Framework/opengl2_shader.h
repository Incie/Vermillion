#pragma once

#include"pch.h"
#include"glm/vec4.hpp"


enum ShaderType {
	SHADER_VERTEX,
	SHADER_FRAGMENT,
	SHADER_GEOMETRY
};

class GLSLShader
{
public:
	GLSLShader();
	~GLSLShader();

	bool Load(const std::string& shader);
	void Unload();

	int shaderId;
	ShaderType shaderType;
};

class GLSLProgram
{
public:

	bool LoadProgram(const std::string& shader);
	void UnloadProgram();

	void SetUniform(const std::string& uniformname, int value);
	void SetUniform(const std::string& uniformname, const glm::vec4& color);
	void SetUniform(const std::string& uniformname, const glm::vec3& color);

	void Use();
	void NoProgram();

private:
	int programId;
	GLSLShader fragment;
	GLSLShader vertex;
};