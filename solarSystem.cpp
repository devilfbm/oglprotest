#include "stdafx.h"
#include "solarSystem.h"
#include <cmath>
#include <cstring>

// 绘制轨道参数
const int n = 1000; /* 轨道分段数 */
const GLfloat R = 0.5f;
const GLfloat Pi = 3.1415926536f;

SolarSystem::SolarSystem()
{
	glGenLists(1);
}

SolarSystem::SolarSystem(GLuint* texArray)
{

	textureArray = texArray;
	glGenLists(1);
}

void SolarSystem::Init()
{
	planetArray[0] = new Planet(textureArray[0], 0.0f, 0.0f, 0.0f, 1.0f, 0.5f, 0.0f, 0.0f, 3.0f, "Sun");
	planetArray[1] = new Planet(textureArray[1], 0.0f, 0.0f, 2.0f, 0.2f, 0.7f, 0.7f, 0.4f, 4.8f, "Mercury");
	planetArray[2] = new Planet(textureArray[2], 5.0f, 0.0f, 0.0f, 0.3f, 0.9f, 0.6f, 0.6f, 3.5f, "Venus");
	planetArray[3] = new Planet(textureArray[3], -5.0f, 0.0f, 3.0f, 0.3f, 1.0f, 1.0f, 1.0f, 3.0f, "Earth");
	planetArray[4] = new Planet(textureArray[4], -6.0f, .2f, -4.0f, 0.32f, 0.7f, 0.3f, 0.2f, 2.4f, "Mars");
	planetArray[5] = new Planet(textureArray[5], 6.0f, .1f, 5.0f, 0.34f, 1.0f, 1.0f, 1.0f, 1.3f, "Jupiter");
	planetArray[6] = new Planet(textureArray[6], 7.0f, 0.0f, -6.0f, 0.22f, 0.7f, 0.6f, 0.3f, 0.94f,"Saturn");
	planetArray[7] = new Planet(textureArray[7], -9.0f, 0.0f, 8.0f, 0.5f, 0.5f, 0.5f, 0.5f, 0.68f, "Uranus");
	planetArray[8] = new Planet(textureArray[7], -10.0f, 0.0f, -10.0f, 0.5f, 0.5f, 0.8f, 0.5f, 0.54f, "Neptune");
}

void SolarSystem::Draw()
{
	glNewList(1, GL_COMPILE);
	for (int i = 0; i < NUM_PLANET; i++) {
		planetArray[i]->Draw();
	}
	glEndList();
	glCallList(1);
}

void SolarSystem::Update()
{
	for (int i = 0; i < NUM_PLANET; i++) {
		planetArray[i]->Update();
	}
}

bool SolarSystem::Crash(GLfloat x, GLfloat y, GLfloat z, GLfloat r)
{
	for (int i = 0; i < NUM_PLANET; i++) {
		if (planetArray[i]->Crash(x, y, z, r)) {
			return true;
		}
	}
	return false;
}

void SolarSystem::setRoadActive(bool active)
{
	for (int i = 0; i < NUM_PLANET; i++) {
		planetArray[i]->setRoadActive(active);
	}
}

void SolarSystem::setTextActive(bool active)
{
	for (int i = 0; i < NUM_PLANET; i++) {
		planetArray[i]->setTextActive(active);
	}
}