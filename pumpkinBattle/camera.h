#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include "ball.h"
#include <vector>
#include "wall.h"
#include "model.h"
extern const int bordersize;

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	JUMP
};

extern bool endgame;

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;
bool tcmp(pair<double, Point3D> a, pair<double, Point3D> b);

// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
	// Camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	glm::vec3 velocity;

	// Euler Angles
	float Yaw;
	float Pitch;
	// Camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;
	const float g = 1.;
	// Constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		velocity = glm::vec3(0.0f, 0.0f, 0.0f);
		updateCameraVectors();
	}
	// Constructor with scalar values
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		velocity = glm::vec3(0.0f, 0.0f, 0.0f);
		updateCameraVectors();
	}

	// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}
	const double cameradi = 0.3;

	pair<bool, double> getDisToFace(glm::vec3 position, glm::vec3 velo, Point3D a, Point3D b, Point3D c)
	{
		Ball tmp = Ball(Point3D(position), Point3D(velo));
		pair<double, Point3D> r = tmp.collide(a, b, c);
		bool flag = sgn(r.first - 1) < 0;
		return make_pair(flag, r.first);
	}

	pair<bool, Point3D> CheckValiPosi(const glm::vec3 &last) {
		glm::vec3 velo_vec = Position - last;
		if (Position.y <= -0.2) return make_pair(false, Point3D(0, 1, 0));
		if (abs(Position.x) > bordersize) return make_pair(false, Point3D(1, 0, 0));
		if (abs(Position.z) > bordersize) return make_pair(false, Point3D(0, 0, 1));
		for (Wall *pwall : WallManager) {
			int siz = pwall->allface.size();
			
			vector< pair<double, Point3D> > collisions;
			for (int i = 0; i < siz; i += 4) {
				Point3D n;
				pair<bool, double> res = getDisToFace(last, velo_vec, pwall->allface[i], pwall->allface[i + 1], pwall->allface[i + 2]);
				n = (pwall->allface[i + 1] - pwall->allface[i]) * (pwall->allface[i + 2] - pwall->allface[i]);
				if (res.first) collisions.push_back(make_pair(res.second, n));

				res = getDisToFace(last, velo_vec, pwall->allface[i + 2], pwall->allface[i + 3], pwall->allface[i]);
				n = (pwall->allface[i + 3] - pwall->allface[i + 2]) * (pwall->allface[i] - pwall->allface[i + 2]);
				if (res.first) collisions.push_back(make_pair(res.second, n));
			}
			if (collisions.size() > 0)
			{
				sort(collisions.begin(), collisions.end(), tcmp);
				return make_pair(false, collisions[0].second);
			}
		}

		for (OurModel *pmodel : ModelManager) {
			if (pmodel->typ != 0) continue;
			int siz = pmodel->allface.size();
			vector< pair<double, Point3D> > collisions;
			for (int i = 0; i < siz; i += 3) {
				Point3D n;
				pair<bool, double> res = getDisToFace(last - pmodel->pos.to_glvec(), velo_vec, pmodel->allface[i], pmodel->allface[i + 1], pmodel->allface[i + 2]);
				n = (pmodel->allface[i + 1] - pmodel->allface[i]) * (pmodel->allface[i + 2] - pmodel->allface[i]);
				if (res.first) collisions.push_back(make_pair(res.second, n));
			}
			if (collisions.size() > 0)
			{
				sort(collisions.begin(), collisions.end(), tcmp);
				return make_pair(false, collisions[0].second);
			}
		}
		return make_pair(true, Point3D(0, 0, 0));
	}

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	
	Point3D subsao(Point3D vx, Point3D vo)
	{
		double len = (vx % vo) / vo.length();
		Point3D diao = vo * (len / vo.length());
		return vx - diao * 2;
	}

	void fall(float deltaTime)
	{
		float k = MovementSpeed * deltaTime;
		glm::vec3 last = Position;
		if (Position[1] <= 0)
		{
			Position[1] = 0;
			velocity = glm::vec3(0, 0, 0);
		}
		else
		{
			//Point3D(Position).debug();
			velocity[1] -= g * k;
			Position += k * velocity;
			pair<bool, Point3D> ret = CheckValiPosi(last);
			if (ret.first == false)
			{
				if (sgn((ret.second - Point3D(0, 0, 0)).length())>0)
				{
					Point3D newv = subsao(Point3D(velocity), ret.second);
					bool fg = Position.y <= -0.2;
					velocity.x = newv.x;
					velocity.y = newv.y;
					velocity.z = newv.z;
					Position += k * velocity;
					if (fg)velocity = glm::vec3(0);
				}
				else
				{
					Position = last;
				}
			}
		}
	}

	void ProcessKeyboard(Camera_Movement direction, float deltaTime)
	{
		
		if (endgame) return;
		float k = MovementSpeed * deltaTime;
		glm::vec3 last = Position;
		
		if (Position[1] <= 0)
		{
			//Point3D(Position).debug();
			Position[1] = 0;
			velocity[1] = 0;
			glm::vec3 vFront, vRight;
			vFront = glm::vec3(Front[0], 0, Front[2]);
			vRight = glm::vec3(Right[0], 0, Right[2]);
			normalize(vFront); normalize(vRight);
			if (direction == FORWARD)
				velocity = vFront;
			else if (direction == BACKWARD)
				velocity = -vFront;
			else if (direction == RIGHT)
				velocity = vRight;
			else if (direction == LEFT)
				velocity = -vRight;
			else if (direction == JUMP)
				velocity[1] = 1.4;
			Position += velocity * k;

			pair<bool, Point3D> ret = CheckValiPosi(last);
			if (ret.first == false) 
				Position = last;
		}
		else
		{
		}
	}
	

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw += xoffset;
		Pitch += yoffset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}

		// Update Front, Right and Up Vectors using the updated Euler angles
		updateCameraVectors();
	}

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(float yoffset)
	{
		if (Zoom >= 1.0f && Zoom <= 45.0f)
			Zoom -= yoffset;
		if (Zoom <= 1.0f)
			Zoom = 1.0f;
		if (Zoom >= 45.0f)
			Zoom = 45.0f;
	}



private:
	// Calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors()
	{
		// Calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		Front = glm::normalize(front);
		// Also re-calculate the Right and Up vector
		Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Up = glm::normalize(glm::cross(Right, Front));
	}

	static void normalize(glm::vec3 &vec)
	{
		float len = vec[0] * vec[0] + vec[1] * vec[1] + vec[2] * vec[2];
		if (len < eps)
			return;
		vec *= 1 / sqrt(len);
	}
};
#endif