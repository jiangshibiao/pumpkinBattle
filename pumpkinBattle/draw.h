#ifndef _DRAW_H_
#define _DRAW_H_
#include "point.h"
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

void renderPlane(unsigned int &planeVAO, unsigned int &planeVBO) {
	float planeVertices[] = {
		// positions            // normals         // texcoords

		 bordersize, -0.5f,  bordersize,  0.0f, 1.0f, 0.0f,  bordersize,  0.0f,
		-bordersize, -0.5f,  bordersize,  0.0f, 1.0f, 0.0f,   0.0f,  0.0f,
		-bordersize, -0.5f, -bordersize,  0.0f, 1.0f, 0.0f,   0.0f, bordersize,

		 bordersize, -0.5f,  bordersize,  0.0f, 1.0f, 0.0f,  bordersize,  0.0f,
		-bordersize, -0.5f, -bordersize,  0.0f, 1.0f, 0.0f,   0.0f, bordersize,
		 bordersize, -0.5f, -bordersize,  0.0f, 1.0f, 0.0f,  bordersize, bordersize
	};

	// plane VAO
	if (planeVAO == 0) {
		glGenVertexArrays(1, &planeVAO);
		glGenBuffers(1, &planeVBO);
		glBindVertexArray(planeVAO);
		glBindBuffer(GL_ARRAY_BUFFER, planeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(planeVertices), planeVertices, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindVertexArray(0);
	}
	glBindVertexArray(planeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void renderSky(unsigned int &skyVAO, unsigned int &skyVBO) {
	float skyVertices[] = {
		 -25.0f,  25.0f, -25.0f,
		-25.0f, -25.0f, -25.0f,
		 25.0f, -25.0f, -25.0f,
		 25.0f, -25.0f, -25.0f,
		 25.0f,  25.0f, -25.0f,
		-25.0f,  25.0f, -25.0f,

		-25.0f, -25.0f,  25.0f,
		-25.0f, -25.0f, -25.0f,
		-25.0f,  25.0f, -25.0f,
		-25.0f,  25.0f, -25.0f,
		-25.0f,  25.0f,  25.0f,
		-25.0f, -25.0f,  25.0f,

		 25.0f, -25.0f, -25.0f,
		 25.0f, -25.0f,  25.0f,
		 25.0f,  25.0f,  25.0f,
		 25.0f,  25.0f,  25.0f,
		 25.0f,  25.0f, -25.0f,
		 25.0f, -25.0f, -25.0f,

		-25.0f, -25.0f,  25.0f,
		-25.0f,  25.0f,  25.0f,
		 25.0f,  25.0f,  25.0f,
		 25.0f,  25.0f,  25.0f,
		 25.0f, -25.0f,  25.0f,
		-25.0f, -25.0f,  25.0f,

		-25.0f,  25.0f, -25.0f,
		 25.0f,  25.0f, -25.0f,
		 25.0f,  25.0f,  25.0f,
		 25.0f,  25.0f,  25.0f,
		-25.0f,  25.0f,  25.0f,
		-25.0f,  25.0f, -25.0f,

	};

	glGenVertexArrays(1, &skyVAO);
	glGenBuffers(1, &skyVBO);
	glBindVertexArray(skyVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyVertices), skyVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);


}

void renderCube(unsigned int &cubeVAO, unsigned int &cubeVBO, Point3D texture, bool type = 0)
{

	static float vertices[] = {
		// back face
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
		 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
		 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
		 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
		-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // bottom-left
		-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f, // top-left
		// front face
		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left
		 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f, // bottom-right
		 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
		 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f, // top-right
		-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f, // top-left
		-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f, // bottom-left

		// left face
		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right
		-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-left
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
		-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-left
		-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-right
		-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-right

		// right face
		 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
		 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
		 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f, // top-right         
		 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f, // bottom-right
		 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f, // top-left
		 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f, // bottom-left     

		// bottom face
		-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right
		 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f, // top-left
		 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
		 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f, // bottom-left
		-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f, // bottom-right
		-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f, // top-right

		// top face
		-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
		 1.0f,  1.0f , 1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
		 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f, // top-right     
		 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f, // bottom-right
		-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f, // top-left
		-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f  // bottom-left        

	};
	if (cubeVAO == 0)
	{
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// link vertex attributes
		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}


	for (int st = 0; st < 6; st++) {
		vertices[st * 48 + 8 * 0 + 6] = vertices[st * 48 + 8 * 4 + 6] = vertices[st * 48 + 8 * 5 + 6] = 0.05;
		vertices[st * 48 + 8 * 0 + 7] = vertices[st * 48 + 8 * 1 + 7] = vertices[st * 48 + 8 * 5 + 7] = 0.05;
		vertices[st * 48 + 8 * 1 + 6] = vertices[st * 48 + 8 * 2 + 6] = vertices[st * 48 + 8 * 3 + 6] = 0.06;
		vertices[st * 48 + 8 * 2 + 7] = vertices[st * 48 + 8 * 3 + 7] = vertices[st * 48 + 8 * 4 + 7] = 0.06;
	}

	if (type == 0) {
		vertices[0 * 48 + 8 * 0 + 6] = vertices[0 * 48 + 8 * 4 + 6] = vertices[0 * 48 + 8 * 5 + 6] = texture.x;
		vertices[0 * 48 + 8 * 0 + 7] = vertices[0 * 48 + 8 * 2 + 7] = vertices[0 * 48 + 8 * 4 + 7] = texture.y;
		vertices[0 * 48 + 8 * 1 + 6] = vertices[0 * 48 + 8 * 2 + 6] = vertices[0 * 48 + 8 * 3 + 6] = texture.x + texture.z;
		vertices[0 * 48 + 8 * 1 + 7] = vertices[0 * 48 + 8 * 3 + 7] = vertices[0 * 48 + 8 * 5 + 7] = texture.y + texture.z;

		vertices[1 * 48 + 8 * 0 + 6] = vertices[1 * 48 + 8 * 4 + 6] = vertices[1 * 48 + 8 * 5 + 6] = texture.x;
		vertices[1 * 48 + 8 * 0 + 7] = vertices[1 * 48 + 8 * 1 + 7] = vertices[1 * 48 + 8 * 5 + 7] = texture.y;
		vertices[1 * 48 + 8 * 1 + 6] = vertices[1 * 48 + 8 * 2 + 6] = vertices[1 * 48 + 8 * 3 + 6] = texture.x + texture.z;
		vertices[1 * 48 + 8 * 2 + 7] = vertices[1 * 48 + 8 * 3 + 7] = vertices[1 * 48 + 8 * 4 + 7] = texture.y + texture.z;
	}
	else {
		vertices[2 * 48 + 8 * 1 + 6] = vertices[2 * 48 + 8 * 2 + 6] = vertices[2 * 48 + 8 * 3 + 6] = texture.x;
		vertices[2 * 48 + 8 * 2 + 7] = vertices[2 * 48 + 8 * 3 + 7] = vertices[2 * 48 + 8 * 4 + 7] = texture.y;
		vertices[2 * 48 + 8 * 0 + 6] = vertices[2 * 48 + 8 * 4 + 6] = vertices[2 * 48 + 8 * 5 + 6] = texture.x + texture.z;
		vertices[2 * 48 + 8 * 0 + 7] = vertices[2 * 48 + 8 * 1 + 7] = vertices[2 * 48 + 8 * 5 + 7] = texture.y + texture.z;

		vertices[3 * 48 + 8 * 0 + 6] = vertices[3 * 48 + 8 * 4 + 6] = vertices[3 * 48 + 8 * 5 + 6] = texture.x;
		vertices[3 * 48 + 8 * 1 + 7] = vertices[3 * 48 + 8 * 3 + 7] = vertices[3 * 48 + 8 * 5 + 7] = texture.y;
		vertices[3 * 48 + 8 * 1 + 6] = vertices[3 * 48 + 8 * 2 + 6] = vertices[3 * 48 + 8 * 3 + 6] = texture.x + texture.z;
		vertices[3 * 48 + 8 * 0 + 7] = vertices[3 * 48 + 8 * 2 + 7] = vertices[3 * 48 + 8 * 4 + 7] = texture.y + texture.z;
	}

	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

void renderCuboid(float *vertices, unsigned int &cubeVAO, unsigned int &cubeVBO)
{
	if (cubeVAO == 0)
	{
		glGenVertexArrays(1, &cubeVAO);
		glGenBuffers(1, &cubeVBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
		glBufferData(GL_ARRAY_BUFFER, DATASIZE * 36 * sizeof(float), vertices, GL_STATIC_DRAW);
		// link vertex attributes
		glBindVertexArray(cubeVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
	glBufferData(GL_ARRAY_BUFFER, DATASIZE * 36 * sizeof(float), vertices, GL_STATIC_DRAW);

	glBindVertexArray(cubeVAO);
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	glBindVertexArray(0);
}

void renderTriangle(unsigned int &triVAO, unsigned int &triVBO, vector<pair<pair<Point3D, Point3D>, Point2D>> &vec)
{
	float *vertices = new float [vec.size() * DATASIZE];
	for (int i = 0; i < vec.size(); i++)
		vec[i].first.first.PushPoint(vec[i].first.second, vertices + DATASIZE * i, vec[i].second);

	if (triVAO == 0)
	{
		glGenVertexArrays(1, &triVAO);
		glGenBuffers(1, &triVBO);
		// fill buffer
		glBindBuffer(GL_ARRAY_BUFFER, triVBO);
		glBufferData(GL_ARRAY_BUFFER, vec.size() * DATASIZE * sizeof(float), vertices, GL_STATIC_DRAW);
		// link vertex attributes
		glBindVertexArray(triVAO);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	glBindBuffer(GL_ARRAY_BUFFER, triVBO);
	glBufferData(GL_ARRAY_BUFFER, vec.size() * DATASIZE * sizeof(float), vertices, GL_STATIC_DRAW);

	glBindVertexArray(triVAO);
	glDrawArrays(GL_TRIANGLES, 0, vec.size());
	glBindVertexArray(0);

	delete[] vertices;
}

const int sphereprec = 15;
const double PI = acos(-1);
void generatesphere(vector<pair<pair<Point3D, Point3D>, Point2D>> &vec) {
	Point3D tmp[sphereprec + 1][sphereprec];
	for (int i = 0; i <= sphereprec; ++i) {
		double the = PI * i / sphereprec - PI / 2;
		for (int j = 0; j < sphereprec; ++j) {
			double alp = 2 * PI * j / sphereprec;
			tmp[i][j] = Point3D(cos(the)*cos(alp), sin(the), cos(the)*sin(alp));
			if (i == 0 || i == sphereprec) tmp[i][j].x = tmp[i][j].z = 0;
		}
	}
	for (int i = 1; i < sphereprec; ++i) {
		for (int j = 0; j < sphereprec; ++j) {
			int k = (j + 1) % sphereprec;

#define vebp(i,j,x,y) vec.emplace_back(make_pair(make_pair(tmp[i][j],tmp[i][j]),Point2D(x,y)))

			vebp(i, k, 1, 1); vebp(i, j, 0, 1); vebp(i - 1, j, 0, 0);
			vebp(i, j, 0, 0); vebp(i, k, 1, 0); vebp(i + 1, k, 1, 1);
		}
	}
}

void generatecylinder(vector<pair<pair<Point3D, Point3D>, Point2D>> &vec) {
	Point3D tmp[sphereprec + 1][sphereprec];
	for (int i = 0; i <= sphereprec; ++i) {
		double hei = 2. * i / sphereprec - 1.;
		for (int j = 0; j < sphereprec; ++j) {
			double alp = 2 * PI * j / sphereprec;
			tmp[i][j] = Point3D(cos(alp), hei, sin(alp));
			if (i == 0 || i == sphereprec) tmp[i][j].x = tmp[i][j].z = 0;
		}
	}

	for (int i = 1; i < sphereprec; ++i) {
		for (int j = 0; j < sphereprec; ++j) {
			int k = (j + 1) % sphereprec;

			vebp(i, k, 1, 1); vebp(i, j, 0, 1); vebp(i - 1, j, 0, 0);
			vebp(i, j, 0, 0); vebp(i, k, 1, 0); vebp(i + 1, k, 1, 1);
		}
	}
}

void generatecone(vector<pair<pair<Point3D, Point3D>, Point2D>> &vec) {
	Point3D tmp[sphereprec + 1][sphereprec];
	for (int i = 0; i <= sphereprec; ++i) {
		double hei = 2. * i / sphereprec - 1., rad = 1 - 1. * (i + 1) / sphereprec;
		for (int j = 0; j < sphereprec; ++j) {
			double alp = 2 * PI * j / sphereprec;
			tmp[i][j] = Point3D(rad * cos(alp), hei, rad * sin(alp));
			if (i == 0 || i == sphereprec) tmp[i][j].x = tmp[i][j].z = 0;
		}
	}
	for (int i = 1; i < sphereprec; ++i) {
		for (int j = 0; j < sphereprec; ++j) {
			int k = (j + 1) % sphereprec;

			vebp(i, k, 1, 1); vebp(i, j, 0, 1); vebp(i - 1, j, 0, 0);
			vebp(i, j, 0, 0); vebp(i, k, 1, 0); vebp(i + 1, k, 1, 1);
		}
	}
}



#endif