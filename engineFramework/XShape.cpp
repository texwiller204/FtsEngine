#include "stdafx.h"
#include "XShape.h"
#include <cmath>

namespace core
{

	typedef glm::vec3 vec3;
	typedef glm::vec2 vec2;

#define M_PI       3.14159265358979323846

	static GLuint sGLXShapeAttributePosition = 0;
	static GLuint sGLXShapeAttributeNormal = 1;
	static GLuint sGLXShapeAttributeTextureUV = 2;


	void GLXShape::SetAttributePosition(GLuint index)
	{
		sGLXShapeAttributePosition = index;
	}

	void GLXShape::SetAttributeNormal(GLuint index)
	{
		sGLXShapeAttributeNormal = index;
	}

	void GLXShape::SetAttributeTextureUV(GLuint index)
	{
		sGLXShapeAttributeTextureUV = index;
	}


	GLXShape::GLXShape()
	{
		mProgram = new BasicShader;
		mProgram->LoadShaders();
	}


	GLXShape::~GLXShape()
	{
		glDeleteBuffers(1, &mVertexBuffer);
		delete mProgram;
		mProgram = NULL;
	}
	
	void GLXShape::addPolygon(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3,
		const glm::vec2& uv1, const glm::vec2& uv2, const glm::vec2& uv3)
	{
		glm::vec3 d1 = glm::normalize(p2 - p1);
		glm::vec3 d2 = glm::normalize(p3 - p1);
		glm::vec3 normal = glm::normalize(glm::cross(d1, d2));

		
		GLXVertex v1, v2, v3;
		v1.pos = p1;
		v2.pos = p2;
		v3.pos = p3;
		v1.normal = normal;
		v2.normal = normal;
		v3.normal = normal;
		v1.texUV = uv1;
		v2.texUV = uv2;
		v3.texUV = uv3;

		mVertexData.push_back(v1);
		mVertexData.push_back(v2);
		mVertexData.push_back(v3);
	}

	void GLXShape::makeVBO()
	{

		mVertexCount = mVertexData.size();

		glGenBuffers(1, &mVertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLXVertex)*mVertexCount, &mVertexData[0], GL_STATIC_DRAW);		

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		GL_CHECK
	}



	void GLXShape::draw()
	{
		glBindBuffer(GL_ARRAY_BUFFER, mVertexBuffer);
		//mProgram->Use();

		glEnableVertexAttribArray(sGLXShapeAttributePosition);
		glVertexAttribPointer(sGLXShapeAttributePosition, 3, GL_FLOAT, GL_FALSE, sizeof(GLXVertex), (void*)0);
		glEnableVertexAttribArray(sGLXShapeAttributeNormal);
		glVertexAttribPointer(sGLXShapeAttributeNormal, 3, GL_FLOAT, GL_FALSE, sizeof(GLXVertex), (void*)(sizeof(glm::vec3)));
		glEnableVertexAttribArray(sGLXShapeAttributeTextureUV);
		glVertexAttribPointer(sGLXShapeAttributeTextureUV, 2, GL_FLOAT, GL_FALSE, sizeof(GLXVertex), (void*)(sizeof(glm::vec3) + sizeof(glm::vec3)));

		glDrawArrays(GL_TRIANGLES, 0, mVertexCount);

		glDisableVertexAttribArray(sGLXShapeAttributeTextureUV);
		glDisableVertexAttribArray(sGLXShapeAttributePosition);
		glDisableVertexAttribArray(sGLXShapeAttributeTextureUV);
		GL_CHECK
		//mProgram->Unuse();
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}


	GLXShapePlane::GLXShapePlane()
	{
		addPolygon(vec3(0.0, 0.0, 0.0), vec3(1.0, 0.0, 0.0), vec3(1.0, 1.0, 0.0),
			vec2(0.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 0.0));

		addPolygon(vec3(1.0, 1.0, 0.0), vec3(0.0, 1.0, 0.0), vec3(0.0, 0.0, 0.0),
			vec2(1.0, 0.0), vec2(0.0, 0.0), vec2(0.0, 1.0));

		makeVBO();
	}


	GLXShapeCube::GLXShapeCube(float size)
	{
	
		addPolygon(vec3(-size, -size, size), vec3(size, -size, size), vec3(size, size, size),
			vec2(0.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 0.0));
		addPolygon(vec3(size, size, size), vec3(-size, size, size), vec3(-size, -size, size),
			vec2(1.0, 0.0), vec2(0.0, 0.0), vec2(0.0, 1.0));

		
		addPolygon(vec3(size, -size, size), vec3(size, -size, -size), vec3(size, size, -size),
			vec2(0.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 0.0));
		addPolygon(vec3(size, size, -size), vec3(size, size, size), vec3(size, -size, size),
			vec2(1.0, 0.0), vec2(0.0, 0.0), vec2(0.0, 1.0));

		
		addPolygon(vec3(size, -size, -size), vec3(-size, -size, -size), vec3(-size, size, -size),
			vec2(0.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 0.0));
		addPolygon(vec3(-size, size, -size), vec3(size, size, -size), vec3(size, -size, -size),
			vec2(1.0, 0.0), vec2(0.0, 0.0), vec2(0.0, 1.0));

		
		addPolygon(vec3(-size, -size, -size), vec3(-size, -size, size), vec3(-size, size, size),
			vec2(0.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 0.0));
		addPolygon(vec3(-size, size, size), vec3(-size, size, -size), vec3(-size, -size, -size),
			vec2(1.0, 0.0), vec2(0.0, 0.0), vec2(0.0, 1.0));

		
		addPolygon(vec3(-size, size, size), vec3(size, size, size), vec3(size, size, -size),
			vec2(0.0, 1.0), vec2(1.0, 1.0), vec2(1.0, 0.0));
		addPolygon(vec3(size, size, -size), vec3(-size, size, -size), vec3(-size, size, size),
			vec2(1.0, 0.0), vec2(0.0, 0.0), vec2(0.0, 1.0));

	
		addPolygon(vec3(-size, -size, size), vec3(-size, -size, -size), vec3(size, -size, -size),
			vec2(1.0, 1.0), vec2(1.0, 0.0), vec2(0.0, 0.0));
		addPolygon(vec3(size, -size, -size), vec3(size, -size, size), vec3(-size, -size, size),
			vec2(0.0, 0.0), vec2(0.0, 1.0), vec2(1.0, 1.0));

		makeVBO();
	}


	GLXShapeCylinder::GLXShapeCylinder(int divCount, float radius, float height)
	{
		float yTop = height / 2;
		float yBottom = -height / 2;

		float angleOne = M_PI * 2 / divCount;
		float texOne = 1.0 / divCount;

		for (int i = 0; i < divCount; i++) {
			float angle1 = angleOne * i;
			float angle2 = angleOne * (i + 1);
			vec3 v1Top = vec3(0.0f, yTop, 0.0f);
			vec3 v2Top = vec3(cosf(angle2) * radius, yTop, sinf(angle2) * radius);
			vec3 v3Top = vec3(cosf(angle1) * radius, yTop, sinf(angle1) * radius);
			float texTopX1 = sinf(M_PI * 2 - angle1) * 0.5f + 0.5f;
			float texTopY1 = cosf(M_PI * 2 - angle1) * 0.5f + 0.5f;
			float texTopX2 = sinf(M_PI * 2 - angle2) * 0.5f + 0.5f;
			float texTopY2 = cosf(M_PI * 2 - angle2) * 0.5f + 0.5f;
			addPolygon(v1Top, v2Top, v3Top, vec2(0.5, 0.5), vec2(texTopX1, texTopY1), vec2(texTopX2, texTopY2));

			vec3 v1Bottom = v1Top;
			vec3 v2Bottom = v3Top;
			vec3 v3Bottom = v2Top;
			v1Bottom.y = yBottom;
			v2Bottom.y = yBottom;
			v3Bottom.y = yBottom;
			float texBottomX1 = sinf(angle1) * 0.5f + 0.5f;
			float texBottomY1 = cosf(angle1) * 0.5f + 0.5f;
			float texBottomX2 = sinf(angle2) * 0.5f + 0.5f;
			float texBottomY2 = cosf(angle2) * 0.5f + 0.5f;
			addPolygon(v1Bottom, v2Bottom, v3Bottom, vec2(0.5, 0.5), vec2(texBottomX1, texBottomY1), vec2(texBottomX2, texBottomY2));

			float texX1 = (1.0 - texOne * i);
			float texX2 = (1.0 - texOne * (i + 1));

			addPolygon(v2Bottom, v3Top, v2Top, vec2(texX1, 1.0), vec2(texX1, 0.0), vec2(texX2, 0.0));
			addPolygon(v2Top, v3Bottom, v2Bottom, vec2(texX2, 0.0), vec2(texX2, 1.0), vec2(texX1, 1.0));
		}

		makeVBO();
	}


	vec2 test(float x, float y) {
		return vec2(1.0, 1.0);
	}

	GLXShapeCone::GLXShapeCone(int divCount, float radius, float height)
	{
		vec3 vTop = vec3(0.0, height / 2, 0.0);
		float yBottom = -height / 2;

		float angleOne = M_PI * 2 / divCount;
		float texOne = 1.0 / divCount;

		for (int i = 0; i < divCount; i++) {
			float angle1 = angleOne * i;
			float angle2 = angleOne * (i + 1);

			vec3 v1Bottom = vec3(0.0f, yBottom, 0.0f);
			vec3 v2Bottom = vec3(cosf(angle1) * radius, yBottom, sinf(angle1) * radius);
			vec3 v3Bottom = vec3(cosf(angle2) * radius, yBottom, sinf(angle2) * radius);
			float texBottomX1 = sinf(angle1) * 0.5f + 0.5f;
			float texBottomY1 = cosf(angle1) * 0.5f + 0.5f;
			float texBottomX2 = sinf(angle2) * 0.5f + 0.5f;
			float texBottomY2 = cosf(angle2) * 0.5f + 0.5f;
			addPolygon(v1Bottom, v2Bottom, v3Bottom, vec2(0.5, 0.5), vec2(texBottomX1, texBottomY1), vec2(texBottomX2, texBottomY2));

			float texX1 = (1.0 - texOne * i);
			float texX2 = (1.0 - texOne * (i + 1));
			addPolygon(vTop, v3Bottom, v2Bottom, vec2(texX2, 0.0), vec2(texX2, 1.0), vec2(texX1, 1.0));
		}

		makeVBO();
	}


	GLXShapeSphere::GLXShapeSphere(int divX, int divY, float radius)
	{
		for (int j = 0; j < divY; j++) {
			float top = (float)(j + 1) / divY;
			float bottom = (float)j / divY;

			float top_y = cosf(top*M_PI) * radius;
			float bottom_y = cosf(bottom*M_PI) * radius;

			float top_r = sinf(top*M_PI) * radius;
			float bottom_r = sinf(bottom*M_PI) * radius;

			for (int i = 0; i < divX; i++) {
				float i1 = (float)i / divX;
				float i2 = (float)(i + 1) / divX;

				float top_x1 = cosf(i1*M_PI * 2) * top_r;
				float top_x2 = cosf(i2*M_PI * 2) * top_r;

				float top_z1 = sinf(i1*M_PI * 2) * top_r;
				float top_z2 = sinf(i2*M_PI * 2) * top_r;

				float bottom_x1 = cosf(i1*M_PI * 2) * bottom_r;
				float bottom_x2 = cosf(i2*M_PI * 2) * bottom_r;

				float bottom_z1 = sinf(i1*M_PI * 2) * bottom_r;
				float bottom_z2 = sinf(i2*M_PI * 2) * bottom_r;

				vec3 v1(top_x1, top_y, top_z1);
				vec3 v2(bottom_x1, bottom_y, bottom_z1);
				vec3 v3(top_x2, top_y, top_z2);
				vec3 v4(bottom_x2, bottom_y, bottom_z2);

				addPolygon(v1, v2, v3, vec2(1.0 - i1, top), vec2(1.0 - i1, bottom), vec2(1.0 - i2, top));
				addPolygon(v3, v2, v4, vec2(1.0 - i2, top), vec2(1.0 - i1, bottom), vec2(1.0 - i2, bottom));
			}
		}

		makeVBO();
	}


	static inline vec3 calcTorus(float angleA, float angleB, float radius1, float radius2) {
		return vec3((radius1 + radius2 * cosf(angleB)) * cosf(angleA),
			(radius1 + radius2 * cosf(angleB)) * sinf(angleA),
			radius2 * sinf(angleB));
	}

	GLXShapeTorus::GLXShapeTorus(int divCount, float radius1, float radius2)
	{
		for (int j = 0; j < divCount; j++) {
			float j1 = (float)j / divCount;
			float j2 = (float)(j + 1) / divCount;

			float angleA_1 = j1 * M_PI * 2;
			float angleA_2 = j2 * M_PI * 2;

			for (int i = 0; i < divCount; i++) {
				float i1 = (float)i / divCount;
				float i2 = (float)(i + 1) / divCount;

				float angleB_1 = i1 * M_PI * 2;
				float angleB_2 = i2 * M_PI * 2;

				vec3 v1 = calcTorus(angleA_1, angleB_1, radius1, radius2);
				vec3 v2 = calcTorus(angleA_2, angleB_1, radius1, radius2);
				vec3 v3 = calcTorus(angleA_1, angleB_2, radius1, radius2);
				vec3 v4 = calcTorus(angleA_2, angleB_2, radius1, radius2);

				vec2 t1(j1, i1);
				t1.y += 0.5;
				if (t1.y > 1.0) {
					t1.y -= 1.0;
				}
				t1.x = 1.0 - t1.x;

				vec2 t2(j2, i2);
				t2.y += 0.5;
				if (t2.y > 1.0) {
					t2.y -= 1.0;
				}
				t2.x = 1.0 - t2.x;

				addPolygon(v1, v2, v3, vec2(t1.x, t1.y), vec2(t2.x, t1.y), vec2(t1.x, t2.y));
				addPolygon(v3, v2, v4, vec2(t1.x, t2.y), vec2(t2.x, t1.y), vec2(t2.x, t2.y));
			}
		}

		makeVBO();
	}


	static inline vec3 calcKlein(float angleA, float angleB, float radius1, float radius2) {
		return vec3((radius1 + radius2 * cosf(angleB)) * cosf(angleA),
			(radius1 + radius2 * cosf(angleB)) * sinf(angleA),
			radius2 * sinf(angleB));
	}

	GLXShapeKlein::GLXShapeKlein(int divCount, float radius1, float radius2)
	{
		for (int j = 0; j < divCount; j++) {
			float j1 = (float)j / divCount;
			float j2 = (float)(j + 1) / divCount;

			float angleA_1 = j1 * M_PI * 2;
			float angleA_2 = j2 * M_PI * 2;

			for (int i = 0; i < divCount; i++) {
				float i1 = (float)i / divCount;
				float i2 = (float)(i + 1) / divCount;

				float angleB_1 = i1 * M_PI * 2;
				float angleB_2 = i2 * M_PI * 2;

				vec3 v1 = calcKlein(angleA_1, angleB_1, radius1, radius2);
				vec3 v2 = calcKlein(angleA_2, angleB_1, radius1, radius2);
				vec3 v3 = calcKlein(angleA_1, angleB_2, radius1, radius2);
				vec3 v4 = calcKlein(angleA_2, angleB_2, radius1, radius2);

				vec2 t1(j1, i1);
				t1.y += 0.5;
				if (t1.y > 1.0) {
					t1.y -= 1.0;
				}
				t1.x = 1.0 - t1.x;

				vec2 t2(j2, i2);
				t2.y += 0.5;
				if (t2.y > 1.0) {
					t2.y -= 1.0;
				}
				t2.x = 1.0 - t2.x;

				addPolygon(v1, v2, v3, vec2(t1.x, t1.y), vec2(t2.x, t1.y), vec2(t1.x, t2.y));
				addPolygon(v3, v2, v4, vec2(t1.x, t2.y), vec2(t2.x, t1.y), vec2(t2.x, t2.y));
			}
		}

		makeVBO();
	}



	GLXShapeDrop::GLXShapeDrop(int divCount, float radius, float height)
	{
		for (int j = 0; j < divCount; j++) {
			float top = (float)(j + 1) / divCount;
			float bottom = (float)j / divCount;

			float top_y = sinf(top*M_PI / 2) * height - height / 2;
			float bottom_y = sinf(bottom*M_PI / 2) * height - height / 2;

			float top_r = sinf(top*M_PI) * radius;
			float bottom_r = sinf(bottom*M_PI) * radius;

			for (int i = 0; i < divCount; i++) {
				float i1 = (float)i / divCount;
				float i2 = (float)(i + 1) / divCount;

				float top_x1 = cosf(i1*M_PI * 2) * top_r;
				float top_x2 = cosf(i2*M_PI * 2) * top_r;

				float top_z1 = sinf(i1*M_PI * 2) * top_r;
				float top_z2 = sinf(i2*M_PI * 2) * top_r;

				float bottom_x1 = cosf(i1*M_PI * 2) * bottom_r;
				float bottom_x2 = cosf(i2*M_PI * 2) * bottom_r;

				float bottom_z1 = sinf(i1*M_PI * 2) * bottom_r;
				float bottom_z2 = sinf(i2*M_PI * 2) * bottom_r;

				vec3 v1(top_x1, top_y, top_z1);
				vec3 v2(top_x2, top_y, top_z2);
				vec3 v3(bottom_x1, bottom_y, bottom_z1);
				vec3 v4(bottom_x2, bottom_y, bottom_z2);

				addPolygon(v1, v2, v3, vec2(1.0 - i1, 1.0 - top), vec2(1.0 - i2, 1.0 - top), vec2(1.0 - i1, 1.0 - bottom));
				addPolygon(v3, v2, v4, vec2(1.0 - i1, 1.0 - bottom), vec2(1.0 - i2, 1.0 - top), vec2(1.0 - i2, 1.0 - bottom));
			}
		}

		makeVBO();
	}
}