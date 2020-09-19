#pragma once

#include"services.h"
#include"glm/vec3.hpp"
#include"glm/vec4.hpp"
#include"opengl2_shader.h"
#include<map>

class TextureService;

struct Character {
	unsigned int TextureID;  // ID handle of the glyph texture
	glm::ivec2   Size;       // Size of glyph
	glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
	int          Advance;    // Offset to advance to next glyph
};

class Text : public TextService
{
public:
	Text();

	void Init();
	void Deinit();
	
	void PrintText(double x, double y, const std::string &text, unsigned int fontHeight, const glm::vec3& color) const;

	void NewLine(unsigned int fontHeight) const;
	float Print(double x, double y, const std::string& text, unsigned int fontHeight, const glm::vec3& color, bool center = false, bool newline = false) const;
	void PrintCenter(double x, double y, const std::string& text, unsigned int fontHeight, const glm::vec3& color) const;

	float CalculateWidth(const std::string& text, unsigned int fontHeight) const;

private:
	unsigned int font_face_rendered_height;
	std::map<char, Character> characters;
	GLSLProgram fontProgram;
};

struct sdf_character {
	int charvalue;
	int x;
	int y;
	int width;
	int height;
	int xoffset;
	int yoffset;
	int xadvance;
};

class TextSDF : public TextService
{
public:
	void Init(TextureService& textureService);
	void Deinit();

	float CalculateWidth(const std::string& text, unsigned int fontHeight) const;
	float Print(double x, double y, const std::string& text, unsigned int fontHeight, const glm::vec3& color, bool center = false, bool newline = false) const;
	void NewLine(unsigned int fontHeight) const;
	void PrintCenter(const double x, const double y, const std::string& text, unsigned int fontHeight, const glm::vec3& color) const;

private:
	float font_height;

	std::map<int, sdf_character> sdfMap;
	Texture fontTexture;

	GLSLProgram sdfProgram;
};