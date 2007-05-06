#pragma once

#include <OgreString.h>

//this shouldn't be necessary if the following
//parser could be implemented using strings or\
//vectors
const unsigned int __INI_BUFFER_SIZE = 256;

namespace MyGUI
{
    struct __tag_INI_BLOCK_INFO {
	    long start; // ������ ����� � ������
	    long end; // ����� ����� � ������
	    Ogre::String typeBlock; // ��� �����
	    Ogre::String nameBlock; // ��� �����
    };
		    
    class loadINI
    {
        public:
		    loadINI();
		    ~loadINI();
		    bool open(const char * strFileName); // �������� �����
		    void close();
    		
		    bool seekNextBlock(); // ���� ��������� ����
		    bool seekTypeBlock(const char * strType); // ���� ���� �� ����
		    bool seekNameBlock(const char * strName); // ���� ���� �� �����
    		
		    Ogre::String getBlockName(); // ���������� ��� �������� �����
		    Ogre::String getBlockType(); // ���������� ��� �������� �����
		    bool jampBlock(bool jampEnter, bool seekNext = true); // ������� ���� ������ ���� ������ �����
		    bool LoadNextValue(); // ��������� ��������� �������� � �����
		    bool getValueName(Ogre::String & strName); // ����� �������� ����������
		    bool getValue(Ogre::String & strValue,  size_t position = 0); // ����� �������� ����������
		    bool getValue(Ogre::uint32 & uValue,    size_t position = 0); // ����� �������� ����������
		    bool getValue(int & iValue,             size_t position = 0); // ����� �������� ����������
		    bool getValue(float & fValue,           size_t position = 0); // ����� �������� ����������
    		
    protected:

		    bool getLine(); // ���������� ������ ���������� ������
		    void breakLineValue(char * strValue); // ��������� ������ �� ��������
    		
		    FILE * m_file; // ���������� ��������� �����
		    std::vector <__tag_INI_BLOCK_INFO> m_blockInfo;

		    char m_buf[__INI_BUFFER_SIZE]; // ����� ��� ���������� ������
		    size_t m_lenBuff; // ������ ������ � ������

		    std::deque <Ogre::String> m_valueInfo; // ������� ����������� ������ � ��������� �����������
    };
}