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
				long start; // начало блока в байтах
				long end; // конец блока в байтах
				std::string typeBlock; // тип блока
				std::string nameBlock; // имя блока
			};
		public:
			loadINI();
			~loadINI();

			bool open(const std::string & strFileName); // открытие файла
			void close(); // закрытие файла
			bool seekNextBlock(); // ищет следующий блок
			bool seekTypeBlock(const std::string & strType); // ищет блок по типу
			bool seekNameBlock(const std::string & strName); // ищет блок по имени
			const std::string & getBlockName(); // возвращает имя текущего блока
			const std::string & getBlockType(); // возвращает тип текущего блока
			bool jumpBlock(bool jumpEnter, bool seekNext = true); // прыгаем либо внутрь либо наружу блока
			bool LoadNextValue(); // загружает следующий параметр в блоке
			bool getValueName(std::string & strName); // берет название переменной
			bool getValue(std::string & strValue, unsigned char position = 0); // берет значение переменной
			bool getValue(unsigned int & uValue, unsigned char position = 0); // берет значение переменной
			bool getValue(int & iValue, unsigned char position = 0); // берет значение переменной
			bool getValue(float & fValue, unsigned char position = 0); // берет значение переменной
			
	protected:

			bool getLine(); // возвращает строку содержащую данные
			void breakLineValue(char * strValue); // разбивает строку на значения
			
			std::FILE * m_file; // дискриптор открытого файла
			std::vector <__tag_INI_BLOCK_INFO> m_blockInfo;

			char m_buf[__INI_BUFFER_SIZE]; // буфер для считывания файлов
			unsigned short m_lenBuff; // длинна строки в буфере

			std::deque <std::string> m_valueInfo; // текущая загруженная строка с разбитыми параметрами

			std::string m_strEmptyString; // пустая строка для возврата ошибок

	};
}