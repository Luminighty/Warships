#include <iostream>
#include "gCamera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <math.h>

/// <summary>
/// Initializes a new instance of the <see cref="gCamera"/> class.
/// </summary>
gCamera::gCamera(void) : m_eye(0.0f, 20.0f, 20.0f), m_at(0.0f), m_up(0.0f, 1.0f, 0.0f), m_speed(16.0f), m_goFw(0), m_goRight(0), m_slow(false)
{
	SetView( glm::vec3(0,20,20), glm::vec3(0,0,0), glm::vec3(0,1,0));

	m_dist = glm::length( m_at - m_eye );	

	SetProj(45.0f, 640/480.0f, 0.001f, 1000.0f);
	distance = 10.0;
	targetDistance = distance;
	followObject = nullptr;
	cameraSpeed = 1;
	cameraPos = { M_PI / 2, 1 };
}

gCamera::gCamera(glm::vec3 _eye, glm::vec3 _at, glm::vec3 _up) : m_speed(16.0f), m_goFw(0), m_goRight(0), m_dist(10), m_slow(false)
{
	SetView(_eye, _at, _up);
}

gCamera::~gCamera(void)
{
}

void gCamera::SetAt(glm::vec3 _at)
{
	SetView(m_eye, _at, m_up);
}

void gCamera::SetPosition(glm::vec3 _pos)
{
	SetView(_pos, m_at, m_up);
}

void gCamera::SetView(glm::vec3 _eye, glm::vec3 _at, glm::vec3 _up)
{
	m_eye	= _eye;
	m_at	= _at;
	m_up	= _up;

	m_fw  = glm::normalize( m_at - m_eye  );
	m_st = glm::normalize( glm::cross( m_fw, m_up ) );

	m_dist = glm::length( m_at - m_eye );	

	m_u = atan2f( m_fw.z, m_fw.x );
	m_v = acosf( m_fw.y );
}

void gCamera::SetProj(float _angle, float _aspect, float _zn, float _zf)
{
	m_matProj = glm::perspective( _angle, _aspect, _zn, _zf);
	m_matViewProj = m_matProj * m_viewMatrix;
}

glm::mat4 gCamera::GetViewMatrix()
{
	return m_viewMatrix;
}

void gCamera::Update(float _deltaTime)
{

	if (followObject != nullptr) {
		glm::vec3 position = followObject->position;

		if (SDL_fabs(targetDistance - distance) > 0.1)
			distance += (targetDistance - distance) * cameraSpeed * _deltaTime;

		float xPos = distance * SDL_cos(cameraPos.x) * SDL_sin(cameraPos.y);
		float zPos = distance * SDL_sin(cameraPos.x) * SDL_sin(cameraPos.y);
		float yPos = distance * SDL_cos(cameraPos.y);

		m_eye = position + glm::vec3(xPos, yPos, zPos);
		m_at = position + glm::vec3(0, 2, 0);
	}
	else {
		m_eye += (m_goFw * m_fw + m_goRight * m_st) * m_speed * _deltaTime;
		m_at += (m_goFw * m_fw + m_goRight * m_st) * m_speed * _deltaTime;
	}

	m_viewMatrix = glm::lookAt( m_eye, m_at, m_up);
	m_matViewProj = m_matProj * m_viewMatrix;
}

void gCamera::UpdateUV(float du, float dv)
{
	m_u		+= du;
	m_v		 = glm::clamp<float>(m_v + dv, 0.1f, 3.1f);

	m_at = m_eye + m_dist*glm::vec3(	cosf(m_u)*sinf(m_v), 
										cosf(m_v), 
										sinf(m_u)*sinf(m_v) );

	m_fw = glm::normalize( m_at - m_eye );
	m_st = glm::normalize( glm::cross( m_fw, m_up ) );
}

void gCamera::SetSpeed(float _val)
{
	m_speed = _val;
}

void gCamera::Resize(int _w, int _h)
{
	m_matProj = glm::perspective(	45.0f, _w/(float)_h, 0.01f, 1000.0f);

	m_matViewProj = m_matProj * m_viewMatrix;
}

void gCamera::KeyboardDown(SDL_KeyboardEvent& key)
{
	if (followObject != nullptr)
		return;
	switch ( key.keysym.sym )
	{
	case SDLK_LSHIFT:
	case SDLK_RSHIFT:
		if ( !m_slow )
		{
			m_slow = true;
			m_speed /= 4.0f;
		}
		break;
	case SDLK_UP:
			m_goFw = 1;
		break;
	case SDLK_DOWN:
			m_goFw = -1;
		break;
	case SDLK_LEFT:
			m_goRight = -1;
		break;
	case SDLK_RIGHT:
			m_goRight = 1;
		break;
	}
}

void gCamera::KeyboardUp(SDL_KeyboardEvent& key)
{
	float current_speed = m_speed;
	switch ( key.keysym.sym )
	{
	case SDLK_LSHIFT:
	case SDLK_RSHIFT:
		if ( m_slow )
		{
			m_slow = false;
			m_speed *= 4.0f;
		}
		break;
	case SDLK_UP:
	case SDLK_DOWN:
			m_goFw = 0;
		break;
	case SDLK_LEFT:
	case SDLK_RIGHT:
			m_goRight = 0;
		break;
	}
}

void gCamera::MouseMove(SDL_MouseMotionEvent& mouse)
{
	float xrel = mouse.xrel / 100.0f;
	float yrel = mouse.yrel / 100.0f;

	UpdateUV(xrel, yrel);

	cameraPos.x += xrel;
	cameraPos.y -= yrel;

	if (cameraPos.y <= 0.1)
		cameraPos.y = 0.1;
	if (cameraPos.y >= 1.25)
		cameraPos.y = 1.25;
}

void gCamera::MouseWheel(SDL_MouseWheelEvent& wheel)
{
	targetDistance -= wheel.y;
	if (targetDistance <= 5)
		targetDistance = 5;
	if (targetDistance >= 20)
		targetDistance = 20;

}

void gCamera::LookAt(glm::vec3 _at)
{
	SetView(m_eye, _at, m_up);
}

void gCamera::SetFollowObject(GameObject* follow)
{
	followObject = follow;
}

GameObject* gCamera::GetFollowObject()
{
	return followObject;
}
