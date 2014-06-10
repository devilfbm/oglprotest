//=========================================================================
/**
*  @file  oglprotest.cpp
*
*  项目描述： 天空博物馆
*  文件描述:  摄像机碰撞实现类
*  适用平台： VS2013/WIN 8.1 (I only tested this)
*
*  作者：     李嘉良
*  电子邮件:  763450215@qq.com
*  创建日期： 2014-06-09
*  修改日期： 2014-06-09
*
*/
//=========================================================================
#pragma once
#ifndef _SOURCEMANAGER_H
#define _SOURCEMANAGER_H
#include <stdio.h>
#include <vector>
#include <string>
#include <glm\glm.hpp>
class SourceManager
{
public:
	SourceManager();
	~SourceManager();

	void Add(const char * path);
	void Remove(const char * path);
	bool LoadOBJ(
		const char * path,
		std::vector<glm::vec3> & out_vertices,
		std::vector<glm::vec2> & out_uvs,
		std::vector<glm::vec3> & out_normals
		);
	std::vector<std::string> ResList;
	std::vector<std::vector<glm::vec3>> out_vertices_list;
	std::vector<std::vector<glm::vec2>> out_uvs_list;
	std::vector<std::vector<glm::vec3>> out_normals_list;
};

#endif