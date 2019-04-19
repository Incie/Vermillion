#include"pch.h"
#include "Hexagon.h"
#include"gl/glew.h"

Hexagon::Hexagon()
	: highlight(glm::vec3()), color(glm::vec3())
{
	memset(points, 0, sizeof(points));
	memset(points, 0, sizeof(normals));

	SetColor(glm::vec3(1, 1, 1));
}

void Hexagon::Generate(const glm::vec2& v, float innerRadius, float outerRadius)
{
	const float TAU = 3.14159265f * 2.0f;

	glm::vec3 vertices[12];
	for (int i = 0; i < 6; ++i) {
		float angle = TAU * ((float)i / 6.0f);
		float x = cos(angle);
		float y = sin(angle);

		auto index = i * 2;
		vertices[index].x = x * innerRadius + v.x;
		vertices[index].y = y * innerRadius + v.y;
		vertices[index].z = 0.0f;

		vertices[index + 1].x = x * outerRadius + v.x;
		vertices[index + 1].y = y * outerRadius + v.y;
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
}

void Hexagon::Render() const
{
	glBegin(GL_TRIANGLES);
		glColor3fv(&highlight.x);
		for (auto i = 0; i < num_points; ++i) {
			glNormal3fv(&normals[i].x);
			glVertex3fv(&points[i].x);
		}
	glEnd();


	//glBegin(GL_LINES);
	//	for (auto i = 0; i < num_points; ++i) {
	//		glVertex3fv(&points[i].x);
	//		auto normalPoint = (points[i] + normals[i]*5.0f);
	//		glVertex3fv(&normalPoint.x);
	//	}
	//glEnd();
}