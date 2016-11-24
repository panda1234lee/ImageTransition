#pragma once

#include "GlHelper.h"
#include <time.h>
//#include <gl/freeglut.h>

#define TIMER 40

class GlutHelper
{
public:
	static GlutHelper* getInstance()
	{
		if (NULL == s_gluth)
			return new GlutHelper();
		else
			return s_gluth;
	}

	static void releaseInstance()
	{
		if (NULL == s_gluth)
			return;

		delete s_gluth;
		s_gluth = NULL;
	}

    inline void init(int *argc, char *argv[])
    {
        glutInit(argc, argv);
        // GLUT_DOUBLE： 双缓冲
        glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA /*| GLUT_STENCIL | GLUT_DEPTH*/);
        glutInitWindowPosition(100, 100);
        glutInitWindowSize(SCREEN_W, SCREEN_H);
        glutCreateWindow("ImageTransition");

        /*GLenum l = */glewInit();
		if (glewIsSupported("GL_VERSION_2_0"))
			printf("Ready for OpenGL 2.0\n");
		else 
		{
			printf("OpenGL 2.0 not supported\n");
			exit(1);
		}
    }

	inline void setCallbackFuncs()
	{
		glutDisplayFunc(display);
		glutIdleFunc(&display);
		glutKeyboardFunc(processNormalKeys);
	}

	inline void loop()
	{
		glutMainLoop();
	}

    inline static void setShaderHelper(GlHelper *sh)
    {
        s_glh = sh;
    }

    // 定时执行渲染方法
    //inline static void timeFunc(int value)
    //{
        //display();
        // 定时循环播放
        // 间隔value ms，并传入value
        //glutTimerFunc(value, timeFunc, value);
		//saveImage();
    //}

	inline static void processNormalKeys(unsigned char key, int x, int y) 
	{
		// Esc
		if (key == 27)
		{
			exit(0);
		}
		// 截屏
		else if(key == 's' || key == 'S')
		{
			saveImage();
		}
		// 切换显卡纹理
		else if (key == 'c' || key == 'C')
		{
			s_glh->replaceTexture();
		}
		else if (key == 'b' || key == 'B')
		{
			s_glh->replaceTextureInTurns(false);
		}
		else if (key == 'f' || key == 'F')
		{
			s_glh->replaceTextureInTurns(true);
		}
		// 切换本地纹理
		else if (key == 't' || key == 'T')
		{
			s_glh->badReplaceTexture("../source/lena.bmp", 0);
		}
		else if (key == 'y' || key == 'Y')
		{
			s_glh->badReplaceTexture("../source/V.jpg", 0);
		}
		// 切换特效
		else if (key == 'r' || key == 'R')
		{
			static int i = 0;
			
			s_glh->setCurrentProgram(i);

			int size = s_glh->getProgramInfosSize();
			i = (i + 1) % size;
		}
	}

	inline static void printGLVersionInfo()
	{
		printf("Version: %s\n", glGetString(GL_VERSION));
	}

	// 渲染方法
	static void display();

	inline static void saveImage()
	{
		clock_t begin = clock();
		char name[100];
		//static cv::RNG r = cv::RNG(1024);
		//sprintf(name, "../image/%lf.bmp", r.uniform(0., 100.));
		static long int li = 0;
		sprintf(name, "../image/%ld.bmp", li++);

		GlHelper::saveCVImage(name);
		clock_t end = clock();
		std::cout << "(ms)" << 1000.*(end - begin) / CLOCKS_PER_SEC << std::endl;
	}

private:
	GlutHelper();
	~GlutHelper();

private:
    static GlHelper *s_glh;
	static GlutHelper * s_gluth;
};

