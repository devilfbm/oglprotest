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
	roadActive = true;
	textActive = true;
}

SolarSystem::SolarSystem(bool ra , bool ta, GLuint* texArray)
{
	roadActive = ra;
	textActive = ta;
	textureArray = texArray;
}

void SolarSystem::Init()
{
	planetArray[0] = new Planet(100, 0.0f, 0.0f, 0.0f, .15f, 0.5f, 0.0f, 0.0f, 3.0f);
	planetArray[1] = new Planet(textureArray[1], 0.0f, 0.0f, 2.0f, 0.2f, 0.7f, 0.7f, 0.4f, 4.8f);
	planetArray[2] = new Planet(textureArray[2], 5.0f, 0.0f, 0.0f, 0.3f, 0.9f, 0.6f, 0.6f, 3.5f);
	planetArray[3] = new Planet(textureArray[3], -5.0f, 0.0f, 3.0f, 0.3f, 1.0f, 1.0f, 1.0f, 3.0f);
	planetArray[4] = new Planet(textureArray[4], -6.0f, .2f, -4.0f, 0.32f, 0.7f, 0.3f, 0.2f, 2.4f);
	planetArray[5] = new Planet(textureArray[5], 6.0f, .1f, 5.0f, 0.34f, 1.0f, 1.0f, 1.0f, 1.3f);
	planetArray[6] = new Planet(textureArray[6], 7.0f, 0.0f, -6.0f, 0.22f, 0.7f, 0.6f, 0.3f, 0.94f);
	planetArray[7] = new Planet(textureArray[7], -9.0f, 0.0f, 8.0f, 0.5f, 0.5f, 0.5f, 0.5f, 0.68f);
	planetArray[8] = new Planet(textureArray[7], -10.0f, 0.0f, -10.0f, 0.5f, 0.5f, 0.8f, 0.5f, 0.54f);
}

void SolarSystem::Draw()
{
	for (int i = 1; i < NUM_PLANET; i++) {
		planetArray[i]->Draw();
	}
}

void SolarSystem::Update()
{
	for (int i = 0; i < NUM_PLANET; i++) {
		planetArray[i]->Update;
	}
}