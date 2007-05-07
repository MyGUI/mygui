#pragma once

#include <fstream>
#include <vector>
#include <deque>

#define __INI_BUFFER_SIZE 256

namespace MyGUI
{
	class loadINI
	{
		public:
			typedef struct __tag_INI_BLOCK_INFO {
				long start; // ������ ����� � ������
				long end; // ����� ����� � ������
				std::string typeBlock; // ��� �����
				std::string nameBlock; // ��� �����
			};
		public:
			loadINI();
			~loadINI();

			bool open(const std::string & strFileName); // �������� �����
			void close(); // �������� �����
			bool seekNextBlock(); // ���� ��������� ����
			bool seekTypeBlock(const std::string & strType); // ���� ���� �� ����
			bool seekNameBlock(const std::string & strName); // ���� ���� �� �����
			const std::string & getBlockName(); // ���������� ��� �������� �����
			const std::string & getBlockType(); // ���������� ��� �������� �����
			bool jumpBlock(bool jumpEnter, bool seekNext = true); // ������� ���� ������ ���� ������ �����
			bool LoadNextValue(); // ��������� ��������� �������� � �����
			bool getValueName(std::string & strName); // ����� �������� ����������
			bool getValue(std::string & strValue, unsigned char position = 0); // ����� �������� ����������
			bool getValue(unsigned int & uValue, unsigned char position = 0); // ����� �������� ����������
			bool getValue(int & iValue, unsigned char position = 0); // ����� �������� ����������
			bool getValue(float & fValue, unsigned char position = 0); // ����� �������� ����������
			
	protected:

			bool getLine(); // ���������� ������ ���������� ������
			void breakLineValue(char * strValue); // ��������� ������ �� ��������
			
			std::FILE * m_file; // ���������� ��������� �����
			std::vector <__tag_INI_BLOCK_INFO> m_blockInfo;

			char m_buf[__INI_BUFFER_SIZE]; // ����� ��� ���������� ������
			unsigned short m_lenBuff; // ������ ������ � ������

			std::deque <std::string> m_valueInfo; // ������� ����������� ������ � ��������� �����������

			std::string m_strEmptyString; // ������ ������ ��� �������� ������

	};
}