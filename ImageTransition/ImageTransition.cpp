// ImageTransition.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "JsonUtils.h"
#include "GlHelper.h"
#include "GlutHelper.h"

#include <iostream>

int main(int argc, char *argv[])
{
	std::vector<std::string> json;
	JsonUtils::parseFile("../Jsons/Shader_0.txt", json);

	std::vector<std::string> json_;
	JsonUtils::parseFile("../Jsons/Shader_1.txt", json_);

	// 打印调试信息
	//for (int i = 0; i < json.size(); i++)
	//	printf("%s\n", json[i].c_str());

	GlutHelper* gluth = GlutHelper::getInstance();
	gluth->init(&argc, argv);
	gluth->printGLVersionInfo();

	GlHelper* glh = GlHelper::getInstance();

	glh->initShaderProgram(json);
	glh->initShaderProgram(json_);

	// clear
	std::vector<std::string>().swap(json);
	std::vector<std::string>().swap(json_);

	// 默认激活第一个 shader program
	glh->setCurrentProgram(0);

	// 更新了所有ProgramInfo
	glh->loadInitTexture("../Images/micky.png");
	glh->loadInitTexture("../Images/umbrella.png");
	glh->loadInitTexture("../Images/beard.jpg");

	// GlutHelper与GlHelper相分离，便于以后切换特效
	gluth->setShaderHelper(glh);	// ☆
	gluth->setCallbackFuncs();
	gluth->loop();

	glh->releaseInstance();
	gluth->releaseInstance();

    return 0;
}

