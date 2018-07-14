#include "text.h"
#include"ft2build.h"
#include"freetype\freetype.h"
#include"log.h"

#include"opengl2.h"
#include"GL\glew.h"
#include"glm\glm.hpp"

#include<algorithm>
#include<map>

void RenderGLTextures(FT_Face &face);

struct Character {
	GLuint     TextureID;  // ID handle of the glyph texture
	glm::ivec2 Size;       // Size of glyph
	glm::ivec2 Bearing;    // Offset from baseline to left/top of glyph
	GLuint     Advance;    // Offset to advance to next glyph
};

//std::pair<char, Character> *characters = nullptr;
std::map<GLchar, Character> characters;

void Text::Init()
{
	Log::Info("Freetype", "Initing Freetype");

	FT_Library ft;
	if (FT_Init_FreeType(&ft)) {
		Log::Error("Freetype", "Failed to initialize ft");
		return;
	}

	FT_Face face;
	if (FT_New_Face(ft, "fonts/Roboto-Black.ttf", 0, &face)) {
		Log::Error("Freetype", "Failed to load font");
		return;
	}

	FT_Set_Pixel_Sizes(face, 0, 48);

	Log::Info("Freetype", "Rendering Font");
	RenderGLTextures(face);
	Log::Info("Freetype", "Rendered Font");

	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

void Text::Deinit()
{
	//auto iter = ;
	Log::Info("Freetype", "Deleting");
	std::map<GLchar, Character>::const_iterator iter;
	
	for (iter = characters.begin(); iter != characters.end(); iter++) {
		auto ab = (*iter).second;
		glDeleteTextures(1, &ab.TextureID);
	}

	characters.clear();
}

void Text::Render(double x, double y, const std::string & text)
{
	glColor3f(1,1,1);
	glEnable(GL_BLEND);
	glEnable(GL_TEXTURE_2D);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPushMatrix();
	glTranslated(x, y, 0);
	glScaled(1, 1, 1);

	x = 0;
	y = 48;

	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	double scale = 1.0;

	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = characters[*c];

		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y - (-ch.Size.y + ch.Bearing.y) * scale;

		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;

		GLfloat vertices[6][2] = {
			{ xpos,     ypos - h},
			{ xpos,     ypos},
			{ xpos + w, ypos},

			{ xpos,     ypos - h},
			{ xpos + w, ypos},
			{ xpos + w, ypos - h}
		};

		GLfloat texcoords[6][2] = {
			{0, 0}, {0,1}, {1,1}, 
			{0, 0}, {1,1}, {1,0}
		};

		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		glVertexPointer(2, GL_FLOAT, 0, vertices);
		glTexCoordPointer(2, GL_FLOAT, 0, texcoords);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
	}

	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

}

void RenderGLTextures(FT_Face &face)
{
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
			face->glyph->advance.x
		};

		characters.insert(std::pair<GLchar, Character>(c, character));
	}

	glDisable(GL_TEXTURE_2D);
	//TODO: Reset glPixelStorei
}
