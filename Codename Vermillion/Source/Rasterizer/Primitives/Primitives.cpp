#include"pch.h"
#include "Primitives.h"

Triangle::Triangle()
{
	vertices[0] = glm::vec4();
	vertices[1] = glm::vec4();
	vertices[2] = glm::vec4();
	vertices[0].w = 1.0f;
	vertices[1].w = 1.0f;
	vertices[2].w = 1.0f;
}

Triangle::Triangle(const Triangle& triangle)
{
	vertices[0].x = triangle.vertices[0].x;
	vertices[0].y = triangle.vertices[0].y;
	vertices[0].z = triangle.vertices[0].z;
	vertices[0].w = 1.0f;

	vertices[1].x = triangle.vertices[1].x;
	vertices[1].y = triangle.vertices[1].y;
	vertices[1].z = triangle.vertices[1].z;
	vertices[1].w = 1.0f;				

	vertices[2].x = triangle.vertices[2].x;
	vertices[2].y = triangle.vertices[2].y;
	vertices[2].z = triangle.vertices[2].z;
	vertices[2].w = 1.0f;
}

Triangle::Triangle(float x0, float y0, float z0, float x1, float y1, float z1, float x2, float y2, float z2)
{
	vertices[0].x = x0;
	vertices[0].y = y0;
	vertices[0].z = z0;
	vertices[0].w = 1.0f;

	vertices[1].x = x1;
	vertices[1].y = y1;
	vertices[1].z = z1;
	vertices[1].w = 1.0f;

	vertices[2].x = x2;
	vertices[2].y = y2;
	vertices[2].z = z2;
	vertices[2].w = 1.0f;
}

Triangle::Triangle(const glm::vec3& p0, const glm::vec3& p1, const glm::vec3& p2)
{
	vertices[0].x = p0.x;
	vertices[0].y = p0.y;
	vertices[0].z = p0.z;
	vertices[0].w = 1.0f;

	vertices[1].x = p1.x;
	vertices[1].y = p1.y;
	vertices[1].z = p1.z;
	vertices[1].w = 1.0f;

	vertices[2].x = p2.x;
	vertices[2].y = p2.y;
	vertices[2].z = p2.z;
	vertices[2].w = 1.0f;
}

Triangle::~Triangle()
{
}

void Triangle::Transform(const glm::mat4& transform)
{
	for (auto& v : vertices) {
		v = transform * v;

		if (v.w != 0.0f)
		{
			v.x /= v.w;
			v.y /= v.w;
			v.z /= v.w;
		}
	}
}
