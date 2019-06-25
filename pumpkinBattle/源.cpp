#define STB_IMAGE_IMPLEMENTATION
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <ctime>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "screenshot.h"
#include "shader_m.h"
#include "camera.h"
#include "point.h"
#include "wall.h"
#include "ball.h"
#include "model.h"
#include "monster.h"
#include "draw.h"
#include "maze.h"

#include "text.h"


#include <vector>
#include <iostream>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow *window);
unsigned int loadTexture(const char *path);
void renderScene1(const Shader &shader);
void renderScene2(const Shader &shader);
void renderScene3(const Shader &shader);
void renderCuboid(float *vectices);
unsigned int loadCubemap(std::vector<std::string> faces);
void drawModel(const Shader &shader);
double FlipFace(bool modified);
void shoot();

const double bulletspeed = 0.5;
const double clickdist = 5;

bool bSwap = false;

const unsigned int SCR_WIDTH = 1280;
const unsigned int SCR_HEIGHT = 720;

Camera camera(glm::vec3(0.0f, 3.0f, 3.0f));
float lastX = (float)SCR_WIDTH / 2.0;
float lastY = (float)SCR_HEIGHT / 2.0;
bool firstMouse = true;
bool endgame = false;

float deltaTime = 0.0f;
float lastFrame = 0.0f;

extern vector<Wall*> WallManager;
vector<Cube>trans;

extern vector<Ball*> BallManager;
OurModel *bulletModel;

extern vector<OurModel*> ModelManager;
//OurModel *gun;
OurModel *grenade;

extern vector<Cuboid*> CuboidManager;
Point3D goal;

unsigned int planeVAO = 0, planeVBO = 0;
unsigned int skyVAO = 0, skyVBO = 0;
unsigned int cubeVAO = 0, cubeVBO = 0;
unsigned int sphereVAO = 0, sphereVBO = 0;
unsigned int cylinderVAO = 0, cylinderVBO = 0;
unsigned int coneVAO = 0, coneVBO = 0;
unsigned int textVAO = 0, textVBO = 0;

int main()
{
	srand(time(0));

	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "PumpkinBattle", NULL, NULL);

	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	glEnable(GL_DEPTH_TEST);

	Shader shader("3.2.1.point_shadows.vs", "3.2.1.point_shadows.fs");
	Shader simpleDepthShader("3.2.1.point_shadows_depth.vs", "3.2.1.point_shadows_depth.fs", "3.2.1.point_shadows_depth.gs");
	Shader skyshader("sky.vs", "sky.fs");

	Shader textshader("text.vs", "text.frag");
	glm::mat4 projection = glm::ortho(0.0f, static_cast<GLfloat>(SCR_WIDTH), 0.0f, static_cast<GLfloat>(SCR_HEIGHT));
	textshader.use();
	textshader.setMat4("projection", projection);
	textInit(textVAO, textVBO);

	std::vector<std::string> faces
	{
		"resources/textures/skybox/nightsky_ft.png",
		"resources/textures/skybox/nightsky_bk.png",
		"resources/textures/skybox/nightsky_up.png",
		"resources/textures/skybox/nightsky_dn.png",

		"resources/textures/skybox/nightsky_rt.png",
		"resources/textures/skybox/nightsky_lf.png"

	};
	unsigned int skyTexture = loadCubemap(faces);
	unsigned int woodTexture = loadTexture("resources/textures/wood.png");
	unsigned int brickTexture = loadTexture("resources/textures/bricks2.jpg");
	unsigned int shapeTexture = loadTexture("resources/textures/shape.png");
	unsigned int shapeTexture2 = loadTexture("resources/textures/shape2.png");

	
	Monster *one = new Monster("resources/objects/ghost/pumpkin_OBJ.obj",
	Point3D(rand() % 41 - 20, 0.f, rand() % 41 - 20), 0.001f, 7, Point3D(-0.27 / 2, 0.01, -0.2 / 2), 0.27, 0.25, 0.2);
	ModelManager.emplace_back(one);
	
	for (int i = 0; i < 5; i++) {
		Monster *two = new Monster(*one);
		two->pos = Point3D(rand() % 31 - 15, 0.f, rand() % 31 - 15);
		two->Front = Point3D(rand() % 2 * 2 - 1, 0.f, rand() % 41 - 20);
		ModelManager.emplace_back(two);
	}
	

	grenade = new OurModel("resources/objects/explosive/hand.obj", Point3D(0.0f, 0.0f, 0.0f), 0.0002f);

	ModelManager.emplace_back(new OurModel("resources/objects/planet/planet.obj",
		Point3D(-0.0f, 2.0f, 0.0f), 0.1f, 0, Point3D(-0.5/2, -0.3 / 2 - 3.4, -0.5 / 2), 0.5, 1.8, 0.5));

	bulletModel = new OurModel("resources/objects/bullet/lowpolybullet.obj",
		Point3D(0.0f, 0.0f, .0f), 0.02f);
	

	const unsigned int SHADOW_WIDTH = 1 << 10, SHADOW_HEIGHT = 1 << 10;
	GLuint depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);
	// create depth texture
	GLuint depthCubemap;
	glGenTextures(1, &depthCubemap);
	glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
	for (GLuint i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_DEPTH_COMPONENT,
			SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	// attach depth texture as FBO's depth buffer
	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, depthCubemap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
        std::cout << "Framebuffer not complete!" << std::endl;
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	shader.use();
	shader.setInt("diffuseTexture", 0);
	shader.setInt("depthMap", 1);
	glm::vec3 lightPos(0.0f, 10.0f, 0.0f);
	
	buildMaze();


	int timecnt = 0;
	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = glfwGetTime();
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		++timecnt;
		if (timecnt % 20 == 0)printf("%lf\n", 1./deltaTime);
		processInput(window);
		
		lightPos.z = sin(glfwGetTime() * 0.05) * 20.0;
		lightPos.x = cos(glfwGetTime() * 0.05) * 20.0;

		trans.clear();
		
		for (auto it : WallManager)
			it->Display(trans);
		
		for (auto it : ModelManager) {
			if (it->Trymove(camera.Position, camera.MovementSpeed * deltaTime * 0.5) == true) {
				if (endgame == false)puts("Wasted");
				endgame = true;
			}
		}

		Ball::CollBalls();
        camera.fall(deltaTime);

		float near_plane = 1.0f;
		float far_plane = 60.0f;
		glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT, near_plane, far_plane);
		std::vector<glm::mat4> shadowTransforms;
		shadowTransforms.emplace_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		shadowTransforms.emplace_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(-1.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		shadowTransforms.emplace_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 1.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
		shadowTransforms.emplace_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, -1.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f)));
		shadowTransforms.emplace_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));
		shadowTransforms.emplace_back(shadowProj * glm::lookAt(lightPos, lightPos + glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, -1.0f, 0.0f)));

		glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);
		simpleDepthShader.use();
		for (unsigned int i = 0; i < 6; ++i)
			simpleDepthShader.setMat4("shadowMatrices[" + std::to_string(i) + "]", shadowTransforms[i]);
		simpleDepthShader.setFloat("far_plane", far_plane);
		simpleDepthShader.setVec3("lightPos", lightPos);
		renderScene1(simpleDepthShader);
		renderScene2(simpleDepthShader);
		renderScene3(simpleDepthShader);
		drawModel(simpleDepthShader);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shader.use();
		glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		glm::mat4 view = camera.GetViewMatrix();
		shader.setMat4("projection", projection);
		shader.setMat4("view", view);
		// set lighting uniforms
		shader.setVec3("lightPos", lightPos);
		shader.setVec3("viewPos", camera.Position);
		shader.setInt("shadows", true); // enable/disable shadows by pressing 'SPACE'
		shader.setFloat("far_plane", far_plane);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, woodTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
		renderScene1(shader);

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, brickTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
		renderScene2(shader);

		shader.setFloat("hit", 0);	
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, bSwap ? shapeTexture2 : shapeTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, depthCubemap);
		renderScene3(shader);
		drawModel(shader);
		shader.setFloat("hit", 0);

		renderSky(skyVAO, skyVBO);
		glDepthFunc(GL_LEQUAL);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skyTexture);
		skyshader.use();
		projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
		view = glm::mat4(glm::mat3(camera.GetViewMatrix()));
		skyshader.setInt("skybox", 2);
		skyshader.setMat4("projection", projection);
		skyshader.setMat4("view", view);

		glBindVertexArray(skyVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glDepthFunc(GL_LESS);

		int number = 0;
		for (auto it : ModelManager)
			if (it->typ == 1) number += it->Checkhitpoint();
		string message1 = (string)("Ghosts remaining: ") + to_string(number);
		string message2 = "You win!!!";
		RenderText(textshader, textVAO, textVBO, number > 0 ? message1.c_str() : message2.c_str(),
			25.0f, SCR_HEIGHT - 50.0, 1.0f, glm::vec3(1.0, 1.0f, 1.0f));
		if (number == 0) {
			camera.velocity = glm::vec3(0.0);
			camera.Position.y += deltaTime * camera.g * 2;
		}

		glfwSwapBuffers(window);
		glfwPollEvents();

	}

	glDeleteVertexArrays(1, &planeVAO);
	glDeleteBuffers(1, &planeVBO);
	glfwTerminate();
	return 0;

}

void drawModel(const Shader &shader) {
	for (auto it : ModelManager) {
		it->sethit();
		if (it->typ != 0 && it->Checkhitpoint() == false) continue;
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3((it->pos).x, (it->pos).y, (it->pos).z));
		Point3D origin = Point3D(0, 0, 1);
		double theta = (it->Front % origin) / (it->Front).length();
		Point3D axis = (it->Front) * origin;
		if (sgn(axis.length()) != 0)
			model = glm::rotate(model, (float)(-acos(theta)), glm::vec3(axis.x, axis.y, axis.z));
		model = glm::scale(model, glm::vec3(it->size, it->size, it->size));
		shader.setMat4("model", model);
		
		if (it->checkhit()) {
			shader.setFloat("hit", 0.12*it->gethit());
			cout << "hit:" << it->checkhit() << endl;
		}
		
		it->Draw(shader);
		shader.setFloat("hit", 0);
	}

	for (auto it : BallManager) {
		if (it->exstime <= 0) continue;
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3((it->posi).x, (it->posi).y, (it->posi).z));
		Point3D origin = Point3D(0, 0, 1);
		double theta = (it->velo % origin) / it->velo.length();
		Point3D axis = it->velo * origin;
		model = glm::rotate(model, (float)(-acos(theta)), glm::vec3(axis.x, axis.y, axis.z));
		model = glm::scale(model, glm::vec3(0.02f));
		shader.setMat4("model", model);
		bulletModel->Draw(shader);
	}
	double tmpflipface = FlipFace(false);
	goal = Point3D(camera.Position) + Point3D(camera.Front).resize(tmpflipface - 0.05);
	if(tmpflipface < clickdist){
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(goal.x, goal.y, goal.z));
		model = glm::rotate(model, (float)(acos(-1.0) / 2.0), glm::vec3(1.0, 0.0, 0.0));
		model = glm::scale(model, glm::vec3(grenade->size));
		shader.setMat4("model", model);
		grenade->Draw(shader);
	}
	
}

void renderScene1(const Shader &shader)
{
	// floor
	glm::mat4 model = glm::mat4(1.0f);
	shader.setMat4("model", model);
	renderPlane(planeVAO, planeVBO);
}

void renderScene2(const Shader &shader){
	//wall
	for (auto t : trans) {
		Point3D pos = t.pos;
		glm::mat4 model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(pos.x, pos.y, pos.z));
		model = glm::scale(model, glm::vec3(t.size));
		shader.setMat4("model", model);
		renderCube(cubeVAO, cubeVBO, t.text, t.type);
	}
	
	//for debug: cuboid
	for (auto it : CuboidManager) {
		static float vertices[DATASIZE * 36];
		it->Drawcube(vertices);
		glm::mat4 model = glm::mat4(1.0f);
		shader.setMat4("model", model);
		renderCuboid(vertices, cubeVAO, cubeVBO);
	}
}

void renderScene3(const Shader &shader) {
	
	static vector<pair<pair<Point3D, Point3D>, Point2D>>vec;

	//sphere
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0, 0.4, 0.0));
	model = glm::scale(model, glm::vec3(0.3));
	shader.setMat4("model", model);
	vec.clear(); generatesphere(vec);
	renderTriangle(sphereVAO, sphereVBO, vec);

	//cylinder
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0, -0.2, 0.0));
	model = glm::scale(model, glm::vec3(0.3));
	shader.setMat4("model", model);
	vec.clear(); generatecylinder(vec);
	renderTriangle(cylinderVAO, cylinderVBO, vec);

	//cone
	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0, 1.0, 0.0));
	model = glm::scale(model, glm::vec3(0.3));
	shader.setMat4("model", model);
	vec.clear(); generatecone(vec);
	renderTriangle(coneVAO, coneVBO, vec);
}


void processInput(GLFWwindow *window)

{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.ProcessKeyboard(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.ProcessKeyboard(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.ProcessKeyboard(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.ProcessKeyboard(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS) {
		puts("Revived");
		endgame = false;
	}
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS) {
        camera.ProcessKeyboard(JUMP, deltaTime);
    }
	if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS) {
		screeshot(SCR_WIDTH, SCR_HEIGHT);
	}
	if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS) {
		bSwap ^= 1;
	}
}




void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}



void mouse_callback(GLFWwindow* window, double xpos, double ypos)

{
	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = xpos - lastX;
	float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

	lastX = xpos;
	lastY = ypos;
	camera.ProcessMouseMovement(xoffset, yoffset);
}


void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}


void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_RIGHT)
		FlipFace(true), puts("Clicked.");
	if (action == GLFW_PRESS && button == GLFW_MOUSE_BUTTON_LEFT)
		shoot();
}
double FlipFace(bool modified) {
	double mi = inf;
	if (endgame)return mi;
	pair<Wall*, pair<int, int>> ret = make_pair(nullptr,make_pair(-1, -1));
	for (Wall *pwall : WallManager) {
		pair<double, pair<Wall*, pair<int, int>>>temp = pwall->clicked(
			Point3D(camera.Position),
			Point3D(camera.Front)
		);
		if (temp.first < mi) {
			mi = temp.first;
			ret = temp.second;
		}
	}
	if (ret.first != nullptr && mi < clickdist && modified) {
		ret.first->bkdown(ret.second.first, ret.second.second), puts("Modified");
	}
	return mi;
}

void shoot() {
	puts("Shoots");
	BallManager.emplace_back(new Ball(
		Point3D(camera.Position),
		Point3D(camera.Front).resize(bulletspeed),
		0.02
	));
}


unsigned int loadTexture(char const * path)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	int width, height, nrComponents;
	unsigned char *data = stbi_load(path, &width, &height, &nrComponents, 0);
	if (data)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		stbi_image_free(data);
	}
	else
	{
		std::cout << "Texture failed to load at path: " << path << std::endl;
		stbi_image_free(data);
	}
	return textureID;

}
unsigned int loadCubemap(std::vector<std::string> faces){
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++)
	{
		unsigned char *data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}