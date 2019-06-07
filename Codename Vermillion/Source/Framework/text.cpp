#include"pch.h"

#include "text.h"
#include"ft2build.h"
#include"freetype\freetype.h"
#include"log.h"

#include"opengl2.h"
#include"opengl2_shader.h"
#include"GL\glew.h"
#include"glm\glm.hpp"


void RenderFaceToTextures(FT_Face &face);

struct Character {
	GLuint     TextureID;  // ID handle of the glyph texture
	glm::ivec2 Size;       // Size of glyph
	glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
	int        Advance;    // Offset to advance to next glyph
};

std::map<GLchar, Character> characters;

GLSLProgram fontProgram;

unsigned int font_face_rendered_height = 48;

void Text::Init()
{
	Log::Info("Freetype", "Initing Freetype");

	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		Log::Error("Freetype", "Failed to initialize ft");
		return;
	}

	//std::string fontName{ "fonts/PirataOne-Gloomhaven.ttf" };
	std::string fontName{ "fonts/Roboto-Black.ttf" };

	FT_Face face;
	if (FT_New_Face(ft, fontName.c_str(), 0, &face)) {
		Log::Error("Freetype", "Failed to load font");
		return;
	}

	FT_Set_Pixel_Sizes(face, 0, font_face_rendered_height);

	Log::Info("Freetype", "Rendering Font");
	RenderFaceToTextures(face);
	Log::Info("Freetype", "Rendered Font");

	FT_Done_Face(face);
	FT_Done_FreeType(ft);


	fontProgram.LoadProgram("shaders/helloworld");
}

void Text::Deinit()
{
	Log::Info("Freetype", "Deleting");
	std::map<GLchar, Character>::const_iterator iter;
	
	for (iter = characters.begin(); iter != characters.end(); iter++) {
		auto ab = (*iter).second;
		glDeleteTextures(1, &ab.TextureID);
	}

	characters.clear();

	fontProgram.UnloadProgram();
}

float Text::CalculateWidth(const std::string& text, unsigned int fontHeight) const
{
	float textWidth = 0;
	float scale = static_cast<float>(fontHeight) / static_cast<float>(font_face_rendered_height);

	for (std::string::const_iterator c = text.begin(); c != text.end(); c++)
	{
		const Character& ch = characters[*c];

		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		textWidth += static_cast<float>(ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
	}

	return textWidth;
}

void Text::NewLine(unsigned int fontHeight) const 
{
	glTranslatef(0, fontHeight + 2.0f, 0);
}

float Text::Print(double x, double y, const std::string& text, unsigned int fontHeight, const glm::vec3& color, bool center, bool newline) const
{
	int textWidth = 0;
	if (center)
		textWidth = (int)CalculateWidth(text, fontHeight);

	PrintText(x - textWidth*0.5 , y, text, fontHeight, color);
	
	if (newline)
		NewLine(fontHeight);

	return (float)textWidth;
}

void Text::PrintText(double x, double y, const std::string& text, unsigned int fontHeight, const glm::vec3& color) const
{
	fontProgram.Use();
	fontProgram.SetUniform("tex", 0);
	fontProgram.SetUniform("color", glm::vec4(color, 1.0f));

	glColor4f(1,1,1,1);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPushMatrix();
	glTranslated(x, y, 0);
	glScaled(1, 1, 1);

	x = 0;
	double scale = (double)fontHeight / (double)font_face_rendered_height;
	y = fontHeight;

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	const double texcoords[6][2] = {
		{ 0,0 },{ 0,1 },{ 1,1 },
		{ 0,0 },{ 1,1 },{ 1,0 }
	};

	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = characters[*c];

		double xpos = x + ch.Bearing.x * scale;
		double ypos = y - ((double)-ch.Size.y + (double)ch.Bearing.y) * scale;

		double width = ch.Size.x * scale;
		double height = ch.Size.y * scale;

		double vertices[6][2] = {
			{ xpos,     ypos - height},
			{ xpos,     ypos},
			{ xpos + width, ypos},

			{ xpos,     ypos - height},
			{ xpos + width, ypos},
			{ xpos + width, ypos - height}
		};

		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		glVertexPointer(2, GL_DOUBLE, 0, vertices);
		glTexCoordPointer(2, GL_DOUBLE, 0, texcoords);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
	}

	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glDisable(GL_BLEND);

	fontProgram.NoProgram();
}

void Text::PrintCenter(const double x, const double y, const std::string& text, unsigned int fontHeight, const glm::vec3& color) const
{
	double centerY = y - fontHeight * 0.5;

	double w = 0;
	double scale = (double)fontHeight / (double)font_face_rendered_height;

	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		const Character &ch = characters[*c];

		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		w += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
	}

	Print(x - w * 0.5, centerY, text, fontHeight, color);
}

void RenderFaceToTextures(FT_Face &face)
{
	int previousUnpackAlignment = -1;
	glGetIntegerv(GL_UNPACK_ALIGNMENT, &previousUnpackAlignment);
	Log::Debug("RenderFaceToTextures", fmt::format("Unpack Alignment = {0}", previousUnpackAlignment));

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glEnable(GL_TEXTURE_2D);

	for (int c = 0; c < 128; ++c) {
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			Log::Error("Freetype", "Failed to load face");
			continue;
		}

		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);

		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


		// Now store character for later use
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			(int)face->glyph->advance.x
		};

		characters.insert(std::pair<GLchar, Character>(c, character));
	}

	glDisable(GL_TEXTURE_2D);
	glPixelStorei(GL_UNPACK_ALIGNMENT, previousUnpackAlignment);
}
