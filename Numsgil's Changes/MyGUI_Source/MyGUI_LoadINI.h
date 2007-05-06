#pragma once

#include <OgreString.h>

//this shouldn't be necessary if the following
//parser could be implemented using strings or\
//vectors
const unsigned int __INI_BUFFER_SIZE = 256;

namespace MyGUI
{
    struct __tag_INI_BLOCK_INFO {
	    long start; // начало блока в байтах
	    long end; // конец блока в байтах
	    Ogre::String typeBlock; // тип блока
	    Ogre::String nameBlock; // имя блока
    };
		    
    class loadINI
    {
        public:
		    loadINI();
		    ~loadINI();
		    bool open(const char * strFileName); // открытие файла
		    void close();
    		
		    bool seekNextBlock(); // ищет следующий блок
		    bool seekTypeBlock(const char * strType); // ищет блок по типу
		    bool seekNameBlock(const char * strName); // ищет блок по имени
    		
		    Ogre::String getBlockName(); // возвращает имя текущего блока
		    Ogre::String getBlockType(); // возвращает тип текущего блока
		    bool jampBlock(bool jampEnter, bool seekNext = true); // прыгаем либо внутрь либо наружу блока
		    bool LoadNextValue(); // загружает следующий параметр в блоке
		    bool getValueName(Ogre::String & strName); // берет название переменной
		    bool getValue(Ogre::String & strValue,  size_t position = 0); // берет значение переменной
		    bool getValue(Ogre::uint32 & uValue,    size_t position = 0); // берет значение переменной
		    bool getValue(int & iValue,             size_t position = 0); // берет значение переменной
		    bool getValue(float & fValue,           size_t position = 0); // берет значение переменной
    		
    protected:

		    bool getLine(); // возвращает строку содержащую данные
		    void breakLineValue(char * strValue); // разбивает строку на значения
    		
		    FILE * m_file; // дискриптор открытого файла
		    std::vector <__tag_INI_BLOCK_INFO> m_blockInfo;

		    char m_buf[__INI_BUFFER_SIZE]; // буфер для считывания файлов
		    size_t m_lenBuff; // длинна строки в буфере

		    std::deque <Ogre::String> m_valueInfo; // текущая загруженная строка с разбитыми параметрами
    };
}