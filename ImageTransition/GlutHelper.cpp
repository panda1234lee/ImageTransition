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

// TODO: �д��ع�
void GlutHelper::display()
{
	if (NULL == s_glh)
	{
		printf("ShaderHelper Pointer has not been set yet! \n");

		// �˳�����
		//glutDestroyWindow(1);
		//glutLeaveMainLoop();
		//system("exit");
		//return;

		exit(1);
	}

	s_glh->display();

	glutSwapBuffers();
}

