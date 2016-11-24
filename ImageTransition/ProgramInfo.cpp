#include "stdafx.h"
#include "ProgramInfo.h"

ProgramInfo::ProgramInfo() //:ref_count(NULL)
{
	// 初始化引用计数
	ref_count = new int;
	*ref_count = 1;
}

// 拷贝构造函数
ProgramInfo::ProgramInfo(const ProgramInfo& pi)
{
	m_program_id = pi.m_program_id;

	// Texture ID
	tex_ids = pi.tex_ids;
	// Uniform Location
	tex_uls = pi.tex_uls;

	global_time_ul = pi.global_time_ul;
	resolution_ul = pi.resolution_ul;

	// ************************************************
	// 更新引用计数
	ref_count = pi.ref_count;
	if (ref_count)
		(*ref_count)++;
}

// 赋值函数
ProgramInfo& ProgramInfo::operator=(const ProgramInfo& pi)
{
	m_program_id = pi.m_program_id;

	// Texture ID
	tex_ids = pi.tex_ids;
	// Uniform Location
	tex_uls = pi.tex_uls;

	global_time_ul = pi.global_time_ul;
	resolution_ul = pi.resolution_ul;

	// ************************************************
	// 检查引用计数
	if (ref_count)
	{
		// 之前的引用计数减1
		(*ref_count)--;
		// 如果引用计数<=0，则释放之前的数据
		if (*ref_count <= 0)
		{
			delete ref_count;
		}
	}

	// 更新新的引用计数
	ref_count = pi.ref_count;
	if (ref_count)
		(*ref_count)++;

	return *this;
}

// 析构函数
ProgramInfo::~ProgramInfo()
{
	// 检查引用计数
	if (!ref_count)
	{
		return;
	}
	// 引用计数减1 
	(*ref_count)--;

	// 检查是否释放数据
	if (*ref_count <= 0)
	{
		int size = getTexIdSize();
		for (int i = 0; i < size; i++)
		{
			glDeleteTextures(1, &(tex_ids[i]));
		}

		// -------------------------------------------
		// vector的真正释放方式，clear并不能真正释放
		std::vector<GLuint>().swap(tex_ids);
		std::vector<GLint>().swap(tex_uls);
		// -------------------------------------------

		// 删除 program object
		glDeleteProgram(m_program_id);
	}
}