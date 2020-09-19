#include"pch.h"
#include "Hexagon.h"
#include"gl/glew.h"

Hexagon::Hexagon()
	: highlight(glm::vec3()), color(glm::vec3()), texture(nullptr)
{
	memset(points, 0, sizeof(points));
	memset(points, 0, sizeof(normals));

	SetColor(glm::vec3(1, 1, 1));
}

void Hexagon::Generate(const glm::vec2& center, float innerRadius, float outerRadius)
{
	const float TAU = 3.14159265f * 2.0f;

	glm::vec3 vertices[12];
	for (int i = 0; i < 6; ++i) {
		float angle = TAU * ((float)i / 6.0f);
		float x = cos(angle);
		float y = sin(angle);

		auto index = i * 2;
		vertices[index].x = x * innerRadius + center.x;
		vertices[index].y = y * innerRadius + center.y;
		vertices[index].z = 0.0f;

		vertices[index + 1].x = x * outerRadius + center.x;
		vertices[index + 1].y = y * outerRadius + center.y;
		vertices[index + 1].z = -5.0f;
	}

	//Generate Triangles
	for (int i = 0; i < 6; ++i) {
		int index = i * 6;
		int vIndex = i * 2;
		points[index + 0] = vertices[vIndex];
		points[index + 1] = vertices[(vIndex + 1) % 12];
		points[index + 2] = vertices[(vIndex + 2) % 12];

		points[index + 3] = vertices[(vIndex + 1) % 12];
		points[index + 4] = vertices[(vIndex + 3) % 12];
		points[index + 5] = vertices[(vIndex + 2) % 12];
	}

	for (int i = 0; i < 4; ++i) {
		points[36 + i * 3] = vertices[0];
		points[37 + i * 3] = vertices[2 + i * 2];
		points[38 + i * 3] = vertices[4 + i * 2];
	}

	for (int i = 0; i < num_points; i += 3) {
		auto v1 = glm::normalize(points[i + 1] - points[i]);
		auto v2 = glm::normalize(points[i + 2] - points[i]);

		auto normal = glm::normalize(glm::cross(v2, v1));
		normals[i] = normal;
		normals[i + 1] = normal;
		normals[i + 2] = normal;
	}

	std::vector<glm::vec2> tc = { 
		glm::vec2(474,249),
		glm::vec2(361,444),
		glm::vec2(137,444),
		glm::vec2(22,249),
		glm::vec2(135,54),
		glm::vec2(361, 54) 
	};

	texcoords[0] = tc[0];
	texcoords[1] = tc[1];
	texcoords[2] = tc[2];
	texcoords[3] = tc[0];
	texcoords[4] = tc[2];
	texcoords[5] = tc[3];
	texcoords[6] = tc[0];
	texcoords[7] = tc[3];
	texcoords[8] = tc[4];
	texcoords[9] = tc[0];
	texcoords[10] = tc[4];
	texcoords[11] = tc[5];

	for (auto& tc : texcoords) {
		tc.x /= 500.0f;
		tc.y /= 500.0f;
	}
}

#include"..//textures.h"
void Hexagon::Render() const
{
	glBegin(GL_TRIANGLES);
		glColor3fv(&highlight.x);
		for (auto i = 0; i < num_points; ++i) {
			glNormal3fv(&normals[i].x);
			glVertex3fv(&points[i].x);
		}
	glEnd();

	if (texture != nullptr) {
		glDisable(GL_LIGHTING);
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, texture->textureId);

		glEnable(GL_BLEND);
		glColor4f(1, 1, 1, 1);

		glBegin(GL_TRIANGLES);
		for (int i = 0; i < 12; ++i) {
				glTexCoord2fv(&texcoords[i].x);
				glVertex2fv(&points[i + 36].x);
			}
		glEnd();

		glDisable(GL_TEXTURE_2D);
		glDisable(GL_BLEND);
		glEnable(GL_LIGHTING);
	}


	//glBegin(GL_LINES);
	//	for (auto i = 0; i < num_points; ++i) {
	//		glVertex3fv(&points[i].x);
	//		auto normalPoint = (points[i] + normals[i]*5.0f);
	//		glVertex3fv(&normalPoint.x);
	//	}
	//glEnd();
}