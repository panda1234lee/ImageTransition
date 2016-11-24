// ImageTransition.cpp : �������̨Ӧ�ó������ڵ㡣
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

	// ��ӡ������Ϣ
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

	// Ĭ�ϼ����һ�� shader program
	glh->setCurrentProgram(0);

	// ����������ProgramInfo
	glh->loadInitTexture("../Images/micky.png");
	glh->loadInitTexture("../Images/umbrella.png");
	glh->loadInitTexture("../Images/beard.jpg");

	// GlutHelper��GlHelper����룬�����Ժ��л���Ч
	gluth->setShaderHelper(glh);	// ��
	gluth->setCallbackFuncs();
	gluth->loop();

	glh->releaseInstance();
	gluth->releaseInstance();

    return 0;
}

