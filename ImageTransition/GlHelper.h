#pragma once

#include <malloc.h>
#include <stdio.h>
#include <string.h>
#include <gl/glew.h>
#include <opencv2/opencv.hpp>
#include <memory.h>
#include <iostream>

#include"ProgramInfo.h"

#define ATTRIB_VERTEX 3
#define ATTRIB_TEXTURE 4
#define SCREEN_W 640
#define SCREEN_H 640

#define SAFE_DELETE(pointer)	\
if(NULL != pointer)					\
{												\
	try										\
	{											\
		delete pointer;					\
	} catch(...) {}							\
	pointer=NULL;						\
}

// -----------------
class GlHelper
{
public:

    inline static GlHelper *getInstance()
    {
        if (NULL == s_glh)
        {
            s_glh = new GlHelper();
        }
        return s_glh;
    }

	static void releaseInstance()
	{
		if (NULL == s_glh)
			return;

		delete s_glh;
		s_glh = NULL;
	}

    inline void initShaderProgram(std::vector<std::string>& json)
    {
		ProgramInfo pi = ProgramInfo();

		GLuint program_id = initShaders(json[0].c_str(), json[1].c_str());
        pi.setProgramId(program_id);

        setUniformLocation(pi, json);	// ☆ 每个shader不一样，需要解析 JSON 文件

		m_pis.push_back(pi);			// 调用拷贝构造函数，如果没有实现拷贝构造函数将出错
    }

	void setCurrentProgram(int i)
	{
		int size = m_pis.size();
		if (i<0 || i>size)
		{
			printf("Parameter i is invalid! \n");
			return;
		}

		m_cur_pi = &m_pis[i];
		glUseProgram(m_cur_pi->getProgramId());	// △
	}

	ProgramInfo* getCurrentProgram()
	{
		return m_cur_pi;
	}

	void ratioResize(const cv::Mat& src, cv::Mat& dst, int dst_w, int dst_h, float t = 1.f);

	void loadInitTexture(const char *file_path);
	// 首尾交换
	void replaceTexture();
	// 顺序交换
	void replaceTextureInTurns(bool type);
	// 仅作为对比
	void badReplaceTexture(const char* file_path, int index);

    // texUnit <=> value
    // texUL: uniform location
    inline void activeTexture(
        GLuint tex_unit, GLuint tex_id, GLuint tex_ul, GLuint value)
    {
        glActiveTexture(tex_unit);
        glBindTexture(GL_TEXTURE_2D, tex_id);
        glUniform1i(tex_ul, value);	 // ☆
    }

	void display();

    // -----------------------------------
    const ProgramInfo* getProgramInfo(int i) const
    {
		int size = m_pis.size();
		if (i<0 || i>size)
		{
			printf("Parameter i is invalid! \n");
			return NULL;
		}

        return static_cast<const ProgramInfo*>(&m_pis[i]);
    }

	int getProgramInfosSize()
	{
		return m_pis.size();
	}

    GLuint getPixelWidth() const
    {
        return m_pixel_w;
    }

    GLuint getPixelHeight() const
    {
        return m_pixel_h;
    }

    // 读取本地文本
    inline static char *textFileRead(const char *file_name)
    {
        char *s = (char *)malloc(8000);
        memset(s, 0, 8000);
        FILE *infile = fopen(file_name, "rb");
        int len = fread(s, 1, 8000, infile);
        fclose(infile);
        s[len] = 0;

        return s;
    }

	void static saveCVImage(const char *file_name);

    // -----------------------------------
private:
	// 私有构造方法，禁止外部构造和继承
    GlHelper() : m_pixel_w(512), m_pixel_h(512)
    {
		m_cur_pi = NULL;

		enableVertexAttrib();
    }

	// 如果为private， 则外部无法delete
	~GlHelper()
	{
		//SAFE_DELETE(m_pi);
		std::vector<ProgramInfo>().swap(m_pis);
	}

    // 初始化 Shader Program，返回 Program ID
    GLuint initShaders(const char *v_shader_path, const char *f_shader_path);

	// 对应shader的uniform 变量
    inline static void setUniformLocation(ProgramInfo& pi, std::vector<std::string>& ul)
    {
		GLuint p = pi.getProgramId();

		// 至少有6个json 字符串，即shader的路径、2张图片、时间、分辨率
		if (ul.size() < 6)
		{
			printf("Json Uniform Location parse error! \n");
			return;
		}

		pi.addTexUniformLocation(glGetUniformLocation(p, ul[2].c_str()));
		pi.addTexUniformLocation(glGetUniformLocation(p, ul[3].c_str()));
		pi.setGlobalTimeUniformLocation(glGetUniformLocation(p, ul[4].c_str()));
		pi.setResolutionUniformLocation(glGetUniformLocation(p, ul[5].c_str()));

		if (ul.size() > 6)
		{
			// TODO: 比如传入矩阵信息
			std:: cout << "###############################" << std::endl;
			std::cout << "Other Uniform Test : " << ul[6] << std::endl;
			std::cout << "###############################" << std::endl;
		}


		// 引用计数测试，但只能防住对象的复制，防不住指针的复制！
		//pi->printRefCount();
		//ProgramInfo ppi  = *pi;
		//ppi.printRefCount();
    }

    // 激活顶点/纹理属性
    inline void enableVertexAttrib()
    {
        static const GLfloat vertex_vertices[] =
        {
            -1.0f, -1.0f,
            1.0f, -1.0f,
            -1.0f,  1.0f,
            1.0f,  1.0f,
        };

        static const GLfloat texture_vertices[] =
        {
            0.0f,  1.0f,
            1.0f,  1.0f,
            0.0f,  0.0f,
            1.0f,  0.0f,
        };

        glVertexAttribPointer(ATTRIB_VERTEX, 2, GL_FLOAT, 0, 0, vertex_vertices);// Set Arrays
        glEnableVertexAttribArray(ATTRIB_VERTEX);												// Enable it

        glVertexAttribPointer(ATTRIB_TEXTURE, 2, GL_FLOAT, 0, 0, texture_vertices);
        glEnableVertexAttribArray(ATTRIB_TEXTURE);
    }

    //  -----------------------Debug-----------------------
    // 打印 shader 信息
    inline void printShaderInfoLog(GLuint obj)
    {
        int info_log_len = 0;
        int chars_written = 0;
        char *info_log;

        glGetShaderiv(obj, GL_INFO_LOG_LENGTH, &info_log_len);

        if (info_log_len > 0)
        {
            info_log = (char *)malloc(info_log_len);
            glGetShaderInfoLog(obj, info_log_len, &chars_written, info_log);	 // ☆
            printf("%s\n", info_log);
            free(info_log);
        }
    }

    // 打印 Program 信息
    inline void printProgramInfoLog(GLuint obj)
    {
        int info_log_len = 0;
        int chars_written = 0;
        char *info_log;

        glGetProgramiv(obj, GL_INFO_LOG_LENGTH, &info_log_len);

        if (info_log_len > 0)
        {
            info_log = (char *)malloc(info_log_len);
            glGetProgramInfoLog(obj, info_log_len, &chars_written, info_log);	// ☆
            printf("%s\n", info_log);
            free(info_log);
        }
    }

    // 打印出错的文件和行号：char *file, int line
    static int printOglError(const char *file_name, const int line) // __FILE__, __LINE__
    {
        //
        // Returns 1 if an OpenGL error occurred, 0 otherwise.
        //
        GLenum gl_err;
        int    retCode = 0;

        gl_err = glGetError();
        while (gl_err != GL_NO_ERROR)
        {
            printf("glError in file %s @ line %d: %s\n", file_name, line, gluErrorString(gl_err));
            retCode = 1;
            gl_err = glGetError();
        }
        return retCode;
    }

    // ==========================================

private:
    //
    std::vector<ProgramInfo> m_pis;
	ProgramInfo* m_cur_pi;

    GLuint m_pixel_w, m_pixel_h;	// 统一的图像大小
    //GLuint m_screen_w, m_screen_h;

    static GlHelper *s_glh;
};



