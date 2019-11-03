#include"pch.h"
#include"text.h"
#include"file.h"
#include"services.h"
#include"GL/glew.h"
#include"opengl2_shader.h"
#include"nlohmann/json.hpp"


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

float font_height = 42.0f;

std::map<int, sdf_character> sdfMap;
Texture fontTexture;

GLSLProgram sdfProgram;

void TextSDF::Init(TextureService& textureService)
{
	//load png
	//load json
	//parse json

	const std::string fontFile = "fonts/sdf/Roboto-Black.json";
	std::string serializedJson = FileReader::ReadFileContent(fontFile);

	using json = nlohmann::json;
	json j = json::parse(serializedJson);


	auto chars = j["chars"];

	for (int i = 0; i < chars.size(); ++i) {
		auto character = chars[i];

		sdf_character sdfc = { 0 };

		sdfc.charvalue = character.value("id", -1);
		sdfc.width = character.value("width", -1);
		sdfc.height = character.value("height", -1);
		sdfc.xadvance = character.value("xadvance", -1);
		sdfc.xoffset = character.value("xoffset", -1);
		sdfc.yoffset = character.value("yoffset", -1);
		sdfc.x = character.value("x", -1);
		sdfc.y = character.value("y", -1);

		sdfMap[sdfc.charvalue] = sdfc;
	}


	const std::string fontImage = "fonts/sdf/Roboto-Black_msdf.png";
	fontTexture = textureService.LoadTexture(fontImage);


	sdfProgram.LoadProgram("shaders/msdf_shader");
}

void TextSDF::Deinit()
{
}

float TextSDF::CalculateWidth(const std::string& text, unsigned int fontHeight) const
{
	return 0.0f;
}

float TextSDF::Print(double x, double y, const std::string& text, unsigned int fontHeight, const glm::vec3& color, bool center, bool newline) const
{
	sdfProgram.Use();
	sdfProgram.SetUniform("tex", 0);
	sdfProgram.SetUniform("color", glm::vec4(color, 1.0f));

	double scale = (double)fontHeight / (double)font_height;


	glColor4f(1, 1, 1, 1);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glPushMatrix();
	glTranslated(x, y, 0);
	
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glm::vec2 texCoords[6];
	glm::vec2 glyph[6];

	x = 0;
	y = 0;

	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, fontTexture.textureId);

	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		const sdf_character &ch = sdfMap[*c];

		float chx = x + static_cast<float>(ch.xoffset)* scale;
		float chy = static_cast<float>(ch.yoffset)*scale;
		float chw = ch.width * scale;
		float chh = ch.height * scale;

		glyph[0] = { chx      , chy };
		glyph[1] = { chx + chw, chy };
		glyph[2] = { chx + chw, chy + chh};

		glyph[3] = { chx      , chy };
		glyph[4] = { chx + chw, chy + chh };
		glyph[5] = { chx      , chy + chh };
		
		texCoords[0] = { static_cast<float>(ch.x) / 512.0f, static_cast<float>(ch.y) / 512.0f };
		texCoords[1] = { static_cast<float>(ch.x + ch.width) / 512.0f, static_cast<float>(ch.y) / 512.0f };
		texCoords[2] = { static_cast<float>(ch.x + ch.width) / 512.0f, static_cast<float>(ch.y + ch.height) / 512.0f };

		texCoords[3] = { static_cast<float>(ch.x) / 512.0f, static_cast<float>(ch.y) / 512.0f };
		texCoords[4] = { static_cast<float>(ch.x + ch.width) / 512.0f, static_cast<float>(ch.y + ch.height) / 512.0f };
		texCoords[5] = { static_cast<float>(ch.x) / 512.0f, static_cast<float>(ch.y + ch.height) / 512.0f };

		glVertexPointer(2, GL_FLOAT, 0, &glyph[0].x);
		glTexCoordPointer(2, GL_FLOAT, 0, &texCoords[0].x);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		x += ch.xadvance * scale;
	}

	glPopMatrix();

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);

	sdfProgram.NoProgram();

	return 0.0f;
}

void TextSDF::NewLine(unsigned int fontHeight) const
{
}

void TextSDF::PrintCenter(const double x, const double y, const std::string& text, unsigned int fontHeight, const glm::vec3& color) const
{
}
