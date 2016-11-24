#pragma once
#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <GL/freeglut.h>

// ��Ȼ�������ü������������ʹ�� ProgramInfo* ���ͣ�������Ҫ�Լ������ڴ�
class ProgramInfo
{
public:
	ProgramInfo();
	ProgramInfo(const ProgramInfo& pi);
	ProgramInfo& operator=(const ProgramInfo& pi);
	virtual ~ProgramInfo();

	// -------------------------------------

	void setProgramId(GLuint id)
	{
		m_program_id = id;
	}

	GLuint getProgramId()
	{
		return m_program_id;
	}

	// -------------------------------------

	void addTexId(GLuint id)
	{
		tex_ids.push_back(id);
	}

	GLuint  getTexId(unsigned int index)
	{
		return tex_ids[index];
	}

	int getTexIdSize()
	{
		return tex_ids.size();
	}

	void addTexUniformLocation(GLint ul)
	{
		tex_uls.push_back(ul);
	}

	GLint getTexUniformLocation(unsigned int index)
	{
		return tex_uls[index];
	}

	int getTexUniformLocationSize()
	{
		return tex_uls.size();
	}

	// -------------------------------------------

	void setGlobalTimeUniformLocation(GLint ul)
	{
		global_time_ul = ul;
	}

	GLint getGlobalTimeUniformLocation()
	{
		return global_time_ul;
	}

	void setResolutionUniformLocation(GLint ul)
	{
		resolution_ul = ul;
	}

	GLint getResolutionUniformLocation()
	{
		return resolution_ul;
	}

	// -------------------------------------------

	static void swap(GLuint* a, GLuint* b)
	{
		*a = *a^*b;
		*b = *b^*a;
		*a = *a^*b;
	}

	void swapTexId(int i, int j)
	{
		swap(&tex_ids[i], &tex_ids[j]);
	}

	void printRefCount()
	{
		std::cout << "Reference count = " << *ref_count << std::endl;
	}

private:
	GLuint m_program_id;

	// Texture ID
	std::vector<GLuint> tex_ids;
	// Uniform Location
	std::vector<GLint> tex_uls;

	GLint global_time_ul;
	GLint resolution_ul;

	// ���ü�����ע�������ָ�����ͣ�����ǰһ����������ʱ����ǰ��������ü����Ż��1��������������ֵ�Ķ�����������ü�����
	int *ref_count;
};