#include "stdafx.h"
#include "ProgramInfo.h"

ProgramInfo::ProgramInfo() //:ref_count(NULL)
{
	// ��ʼ�����ü���
	ref_count = new int;
	*ref_count = 1;
}

// �������캯��
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
	// �������ü���
	ref_count = pi.ref_count;
	if (ref_count)
		(*ref_count)++;
}

// ��ֵ����
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
	// ������ü���
	if (ref_count)
	{
		// ֮ǰ�����ü�����1
		(*ref_count)--;
		// ������ü���<=0�����ͷ�֮ǰ������
		if (*ref_count <= 0)
		{
			delete ref_count;
		}
	}

	// �����µ����ü���
	ref_count = pi.ref_count;
	if (ref_count)
		(*ref_count)++;

	return *this;
}

// ��������
ProgramInfo::~ProgramInfo()
{
	// ������ü���
	if (!ref_count)
	{
		return;
	}
	// ���ü�����1 
	(*ref_count)--;

	// ����Ƿ��ͷ�����
	if (*ref_count <= 0)
	{
		int size = getTexIdSize();
		for (int i = 0; i < size; i++)
		{
			glDeleteTextures(1, &(tex_ids[i]));
		}

		// -------------------------------------------
		// vector�������ͷŷ�ʽ��clear�����������ͷ�
		std::vector<GLuint>().swap(tex_ids);
		std::vector<GLint>().swap(tex_uls);
		// -------------------------------------------

		// ɾ�� program object
		glDeleteProgram(m_program_id);
	}
}