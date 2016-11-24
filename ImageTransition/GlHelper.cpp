
#include "stdafx.h"
#include"GlHelper.h"
#include <time.h>

GlHelper *GlHelper::s_glh = NULL;

void GlHelper::ratioResize(const cv::Mat& src, cv::Mat& dst, int dst_w, int dst_h, float t )
{
	int w = src.cols;
	int h = src.rows;
	float m = float(w) / h;

	//float t = 1.;											// ������ڵ���1������ϵ��
	if (t < 1.)
	{
		std::cout << "Bad t value !" << std::endl;
		return;
	}

	float mm = float(dst_w) / dst_h;

	dst = cv::Mat(cv::Size(dst_w, dst_h), CV_8UC3, cv::Scalar(0));

	// ���ԭͼС��ָ���ߴ�
	if (dst_w > w && dst_h > h)
	{
		//std::cout << " dst_w > w and dst_h > h !" << std::endl;
		// ֱ����չ�ڱ߼���
		int delta_x = .5 * (dst_w - w);
		int delta_y = .5 * (dst_h - h);
		// ��ֹ dst_w - w��dst_h - h Ϊ����
		copyMakeBorder(src, dst, dst_h - h - delta_y, delta_y, dst_w - w - delta_x, delta_x,
			cv::BORDER_CONSTANT, cv::Scalar(0, 0, 0));

		//imshow("Result", extend);
		//cv::waitKey(0);

		return;
	}

	float len = t * std::max(w, h);					// ����ǰ������
	float len_ = std::min(dst_w, dst_h);		// ���ź����С��

	if (m > 1.f && mm > 1.f)						// ����ǰ�����ź��Ϊ��ͼ
	{
		float n = float(dst_w) / w;					// ȷ�����ź�Ĵ�С�ܹ�������������
		if (dst_h >= h * n)
		{
			len_ = std::max(dst_w, dst_h);		// ���ź������
		}
	}
	else 	if (m < 1.f && mm < 1.f)				// ����ǰ�����ź��Ϊ��ͼ
	{
		float n = float(dst_w) / w;					// ȷ�����ź�Ĵ�С�ܹ�������������
		if (dst_h >= h * n)
		{
			len = t * std::min(w, h);					// ����ǰ����С��
		}
	}

	float ratio = len_ / len;							// �õ����ű���
	//std::cout << "Scale ratio: " << ratio << std::endl;

	int w_ = ratio * w;									// ���ź��ʵ�ʿ��
	int h_ = ratio * h;
	int delta_x = (dst_w - w_) / (2 * ratio);	// �������ź�ĳߴ���Ŀ��ߴ�Ĳ�ֵ���ø�ֵ��һ�뻻��Ϊ����ǰ��ֵ
	int delta_y = (dst_h - h_) / (2 * ratio);	// ��ֵ����ƽ��ͼ��������

	for (int i = 0; i < w; i++)
	{
		for (int j = 0; j < h; j++)
		{
			float u = (i + delta_x) / len;
			float v = (j + delta_y) / len;

			int x = u * len_;
			int y = v * len_;

			dst.at<cv::Vec3b>(y, x)[0] = src.at<cv::Vec3b>(j, i)[0];
			dst.at<cv::Vec3b>(y, x)[1] = src.at<cv::Vec3b>(j, i)[1];
			dst.at<cv::Vec3b>(y, x)[2] = src.at<cv::Vec3b>(j, i)[2];
		}
	}

	//imshow("Result", dst);
	//cv::waitKey(0);
}

void  GlHelper::loadInitTexture(const char *file_path)
{
    int width = m_pixel_w, height = m_pixel_h;

    cv::Mat img = cv::imread(file_path, 1);

    cv::Mat scale, rgb;
    flip(img, img, 1);	// OpenGL ������ ��ҪY�ᷭת

    //resize(img, scale, cv::Size(width, height));
	ratioResize(img, scale, width, height);
	//imshow("Result", scale);
	//cv::waitKey(0);

    cvtColor(scale, rgb, CV_BGR2RGB);

    GLuint tex_id;
    glGenTextures(1, &tex_id);
    glBindTexture(GL_TEXTURE_2D, tex_id);
	// ��ʱ
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, rgb.data);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// ���� ���� id
	//if (!m_cur_pi)
	//{
	//	printf("m_cur_pi is NULL!\n");
	//	exit(-1);
	//}
	//m_cur_pi->addTexId(tex_id);

	for(int i=0; i<m_pis.size(); i++)
		m_pis[i].addTexId(tex_id);
}

// ���һ�ź͵�һ�ŶԵ�
void GlHelper::replaceTexture()
{
	if (!m_cur_pi)
	{
		printf("m_cur_pi is NULL!\n");
		exit(-1);
	}

	int size = m_cur_pi->getTexIdSize();

	//clock_t start = clock();
	m_cur_pi->swapTexId(0, size - 1);
	//clock_t end = clock();
	//std::cout << end - start << std::endl;
}

// ��ǰ�����滻/�Ӻ���ǰ�滻
void GlHelper::replaceTextureInTurns(bool type)
{
	if (!m_cur_pi)
	{
		printf("m_cur_pi is NULL!\n");
		exit(-1);
	}

	int size = m_cur_pi->getTexIdSize();

	//clock_t start = clock();
	if (type)
	{
		for (int i = 1; i < size; i++)
		{
			// i-1 <=> i
			m_cur_pi->swapTexId(i - 1, i);
		}
	}
	else
	{
		for (int i = size - 1; i > 0; i--)
		{
			// i-1 <=> i
			m_cur_pi->swapTexId(i - 1, i);
		}
	}
	//clock_t end = clock();
	//std::cout << end - start << std::endl;
}

// ����Ϊ��������
void GlHelper::badReplaceTexture(const char* file_path, int index)
{
	int width = m_pixel_w, height = m_pixel_h;

	cv::Mat img = cv::imread(file_path, 1);

	cv::Mat scale, rgb;
	flip(img, img, 1);	// OpenGL ������ ��ҪY�ᷭת

	//resize(img, scale, cv::Size(width, height));
	ratioResize(img, scale, width, height);
	//imshow("Result", scale);
	//cv::waitKey(0);

	cvtColor(scale, rgb, CV_BGR2RGB);

	if (!m_cur_pi)
	{
		printf("m_cur_pi is NULL!\n");
		exit(-1);
	}

	int size = m_cur_pi->getTexIdSize();
	if (index > size - 1 || index < 0)
	{
		printf("The index is invalid!\n");
		return;
	}
	
	GLuint tex_id = m_cur_pi->getTexId(index);

	glBindTexture(GL_TEXTURE_2D, tex_id);
	// ��ʱ
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, rgb.data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
}

// ��ʼ�� Shader Program������ Program ID
GLuint GlHelper::initShaders(const char *v_shader_path, const char *f_shader_path)
{
    GLint vertCompiled, fragCompiled, linked;

    // Shaders
    GLint v = glCreateShader(GL_VERTEX_SHADER);
    GLint f = glCreateShader(GL_FRAGMENT_SHADER);

    char *vs = textFileRead(v_shader_path);
    char *fs = textFileRead(f_shader_path);

    glShaderSource(v, 1, &vs, NULL);
    glShaderSource(f, 1, &fs, NULL);

    free(vs);
    free(fs);

    glCompileShader(v);
    glGetShaderiv(v, GL_COMPILE_STATUS, &vertCompiled);	// Debug
    if (vertCompiled != GL_TRUE)
    {
        printf("Vertex Shader compied error! \n");
        printShaderInfoLog(v);
    }

    glCompileShader(f);
    glGetShaderiv(f, GL_COMPILE_STATUS, &fragCompiled);
    if (fragCompiled != GL_TRUE)
    {
        printf("Fragment Shader compied error! \n");
        printShaderInfoLog(f);
    }

    //Program:
    GLuint p = glCreateProgram();
    glAttachShader(p, v);
    glAttachShader(p, f);

    glBindAttribLocation(p, ATTRIB_VERTEX, "vertexIn");
    glBindAttribLocation(p, ATTRIB_TEXTURE, "textureIn");

    glLinkProgram(p);

    glGetProgramiv(p, GL_LINK_STATUS, &linked); // Debug
    if (linked != GL_TRUE)
    {
        printf("Program linked error! \n");
        printProgramInfoLog(p);
    }

    // Debug
    //printOglError(__FILE__, __LINE__);
    //printProgramInfoLog(p);

    return p;
}

void GlHelper::display()
{
	if (!m_cur_pi)
	{
		printf("m_cur_pi is NULL!\n");
		exit(-1);
	}

	GLint global_time_ul(m_cur_pi->getGlobalTimeUniformLocation()); 
	GLint resolution_ul(m_cur_pi->getResolutionUniformLocation()); 
	// --------------------------------------------------
	// Clear
	glClearColor(0.0, 0.0, 0.0, 0.0);
	//glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	//int size = m_cur_pi.tex_ids.size();
	int size = 2; // ������ǰ��������
	for (int i = 0; i < size; i++)
	{
		activeTexture(
				GL_TEXTURE0 + i, m_cur_pi->getTexId(i), m_cur_pi->getTexUniformLocation(i), i);
	}

	// CLOCKS_PER_SEC �� Windows��Ϊ 1000����LinuxΪ 1000000
	GLdouble  time = GLdouble(clock()) / CLOCKS_PER_SEC;	// CLOCKS_PER_SEC
															//std::cout << time << std::endl;
															// Global Time
	glUniform1f(global_time_ul, time);
	// Resolution
	glUniform2f(resolution_ul, SCREEN_W, SCREEN_H);

	// Draw
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	// unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);

	//glutSwapBuffers();
}

void GlHelper::saveCVImage(const char *file_name)
{
	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	int width = viewport[2];
	int height = viewport[3];

	// �������صĶ����ʽ����Word����(4�ֽ�)
	glPixelStorei(GL_PACK_ALIGNMENT, 4);
	// �����������ʵ���
	int nAlignWidth = (width * 24 + 31) / 32;
	// ���仺����
	unsigned char *pdata = new unsigned char[nAlignWidth * height * 4];
	memset(pdata, 0, nAlignWidth * height * 4);

	// �ӵ�ǰ�󶨵� frame buffer ��ȡ pixels  
	glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, pdata);

	cv::Mat src = cv::Mat(height, width, CV_8UC3, pdata);
	cv::Mat bgr;
	cvtColor(src, bgr, CV_RGB2BGR);
	cv::Mat dst;
	flip(bgr, dst, 0);	// 0:x; 1:y, -u:x,y;
	imwrite(file_name, dst);
}