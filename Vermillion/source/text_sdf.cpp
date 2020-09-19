#include"pch.h"
#include"text.h"
#include"file.h"
#include"services.h"
#include"GL/glew.h"
#include"opengl2_shader.h"
#include"nlohmann/json.hpp"

void TextSDF::Init(TextureService& textureService)
{
	font_height = 42.0f;
	const std::string fontFile = "sdf/Roboto-Black.json";
	std::string serializedJson = FileReader::ReadFileContent(FilePath{Paths::Fonts, fontFile});

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

	const std::string fontImage = "sdf/Roboto-Black_msdf.png";
	fontTexture = textureService.LoadTexture(FilePath{ Paths::Fonts, fontImage });

	sdfProgram.LoadProgram("msdf_shader");
}

void TextSDF::Deinit()
{
}

float TextSDF::CalculateWidth(const std::string& text, unsigned int fontHeight) const
{
	float width = 0.0f;
	float scale = static_cast<float>(fontHeight) / font_height;
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		auto cc = static_cast<int>(*c);
		const sdf_character& ch = sdfMap.at(cc);
		width += static_cast<float>(ch.xadvance) * scale;
	}

	return width;
}

float TextSDF::Print(double x, double y, const std::string& text, unsigned int fontHeight, const glm::vec3& color, bool center, bool newline) const
{
	sdfProgram.Use();
	sdfProgram.SetUniform("tex", 0);
	sdfProgram.SetUniform("color", glm::vec4(color, 1.0f));

	float scale = (float)fontHeight / font_height;

	GLCHECK(glColor4f(1, 1, 1, 1));
	GLCHECK(glEnable(GL_BLEND));
	GLCHECK(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

	GLCHECK(glPushMatrix());
	GLCHECK(glTranslated(x, y, 0));
	
	GLCHECK(glEnableClientState(GL_VERTEX_ARRAY));
	GLCHECK(glEnableClientState(GL_TEXTURE_COORD_ARRAY));

	glm::vec2 texCoords[6];
	glm::vec2 glyph[6];

	x = 0;
	y = 0;

	if(center) {
		glTranslated(-CalculateWidth(text, fontHeight) * 0.5f, -static_cast<float>(fontHeight) * 0.5f, 0.0f);
	}

	GLCHECK(glEnable(GL_TEXTURE_2D));
	GLCHECK(glBindTexture(GL_TEXTURE_2D, fontTexture.textureId));

	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		const sdf_character &ch = sdfMap.at(*c);

		float chx = static_cast<float>(x) + static_cast<float>(ch.xoffset) * scale;
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

		GLCHECK(glVertexPointer(2, GL_FLOAT, 0, &glyph[0].x));
		GLCHECK(glTexCoordPointer(2, GL_FLOAT, 0, &texCoords[0].x));
		GLCHECK(glDrawArrays(GL_TRIANGLES, 0, 6));

		const float advance = static_cast<float>(ch.xadvance);
		x += static_cast<double>(advance * scale);
	}

	GLCHECK(glPopMatrix());

	GLCHECK(glDisableClientState(GL_VERTEX_ARRAY));
	GLCHECK(glDisableClientState(GL_TEXTURE_COORD_ARRAY));

	GLCHECK(glDisable(GL_BLEND));
	GLCHECK(glDisable(GL_TEXTURE_2D));

	sdfProgram.NoProgram();

	return 0.0f;
}

void TextSDF::NewLine(unsigned int fontHeight) const
{
	throw std::string("TextSDF::NewLine Not Implmented");
}

void TextSDF::PrintCenter(const double x, const double y, const std::string& text, unsigned int fontHeight, const glm::vec3& color) const
{
	throw std::string("TextSDF::NewLine Not Implmented");
}
