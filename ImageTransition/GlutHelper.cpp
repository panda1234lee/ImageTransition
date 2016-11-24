#include "stdafx.h"
#include "GlutHelper.h"

GlHelper* GlutHelper::s_glh = NULL;
GlutHelper * GlutHelper::s_gluth = NULL;

GlutHelper::GlutHelper()
{
}

GlutHelper::~GlutHelper()
{
	if (s_glh)
	{
		s_glh = NULL;
	}	
}

// TODO: 有待重构
void GlutHelper::display()
{
	if (NULL == s_glh)
	{
		printf("ShaderHelper Pointer has not been set yet! \n");

		// 退出程序
		//glutDestroyWindow(1);
		//glutLeaveMainLoop();
		//system("exit");
		//return;

		exit(1);
	}

	s_glh->display();

	glutSwapBuffers();
}

