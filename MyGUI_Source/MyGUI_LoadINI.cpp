#pragma warning(disable : 4996)
#include "MyGUI_LoadINI.h"

using namespace std;

namespace MyGUI
{
	
	loadINI::loadINI() :
		m_file(0)
	{
	}

	loadINI::~loadINI()
	{
		close();
	}

	bool loadINI::open(const string & strFileName) // открытие файла
	{
		close();
		m_file = fopen(strFileName.c_str(), "r");
		if (m_file == 0) return false;

		__tag_INI_BLOCK_INFO data;
		m_blockInfo.push_back(data);
		m_blockInfo[0].start = 0;
		m_blockInfo[0].end = 0;

		return true;
	}

	void loadINI::close() // закрытие файла
	{
		if (m_file != 0) {
			fclose(m_file);
			m_file = 0;
		}
		m_blockInfo.clear();
	}

	bool loadINI::seekNextBlock() // ищет следующий блок
	{
		if (!m_file) return false;
		// устанавливаем позицию на конец блока
		fseek(m_file, m_blockInfo[m_blockInfo.size()-1].end, SEEK_SET);

		char strOld[__INI_BUFFER_SIZE];
		strOld[0] = 0;

		unsigned char CountSep = 1;
		while (getLine()) {

			if (m_buf[m_lenBuff-1] == '{') { // начало блока
				CountSep++;
			} else if (m_buf[m_lenBuff-1] == '}') { // конец блока
				CountSep--;
				if (CountSep == 0) return false; // мы были дочками
				if (CountSep == 1) { // блок закончился
					unsigned short index = (unsigned short)m_blockInfo.size()-1;
					m_blockInfo[index].start = m_blockInfo[m_blockInfo.size()-1].end;
					fpos_t pos;
					fgetpos(m_file, &pos);
					m_blockInfo[index].end = long(pos);
					m_blockInfo[index].typeBlock.clear();
					m_blockInfo[index].nameBlock.clear();

					pos = strlen(strOld);
					int start = -1;
					for (unsigned short i=0; i<pos; i++) {
						if (start == -1) {
							if (strOld[i] > 32) start = i; // пропускаем первые пробелы
						} else {
							if (strOld[i] <= 32) { // и разделяем строчки при первом пробеле
								strOld[i] = 0;
								if ((i+1) < pos) {
									m_blockInfo[index].nameBlock = &strOld[i+1];
								}
							}
						}
					}
					if (start != -1) m_blockInfo[index].typeBlock = &strOld[start]; // без первых пробелов

					fseek(m_file, m_blockInfo[m_blockInfo.size()-1].start, SEEK_SET); // указатель на начало блока
					while (getLine()) {// и сдвигаем позицию после "{"
						if (m_buf[m_lenBuff-1] == '{') break;
					}

					return true;
				}
			} else if (CountSep == 1) { // еще до начала блока
				strcpy(strOld, m_buf); // сохраняем строку
			}
		}

		return false;
	}

	bool loadINI::seekTypeBlock(const string & strType) // ищет блок по типу
	{
		while (seekNextBlock()) {
			if (m_blockInfo[m_blockInfo.size()-1].typeBlock.compare(strType) == 0)
			    return true;
		}
		return false;
	}

	bool loadINI::seekNameBlock(const string & strName) // ищет блок по имени
	{
		while (seekNextBlock()) {
			if (m_blockInfo[m_blockInfo.size()-1].nameBlock.compare(strName) == 0)
			    return true;
		}
		return false;
	}

	const string & loadINI::getBlockName() // возвращает имя текущего блока
	{
		if (!m_file) return m_strEmptyString;
		return m_blockInfo[m_blockInfo.size()-1].nameBlock;
	}

	const string & loadINI::getBlockType() // возвращает тип текущего блока
	{
		if (!m_file) return m_strEmptyString;
		return m_blockInfo[m_blockInfo.size()-1].typeBlock;
	}

	bool loadINI::jumpBlock(bool jumpEnter, bool seekNext) // прыгаем либо внутрь либо наружу блока
	{
		if (!m_file) return false;

		if (jumpEnter) { // прыгаем во внутрь блока
			fseek(m_file, m_blockInfo[m_blockInfo.size()-1].start, SEEK_SET);
			 // сдвигаем позицию на одну строку внутрь
			while (true) {
				if (!getLine()) return false;
				if (m_buf[m_lenBuff-1] == '{') break;
			}
			fpos_t pos;
			fgetpos(m_file, &pos);
			__tag_INI_BLOCK_INFO data;
			m_blockInfo.push_back(data);
			unsigned short size = (unsigned short)m_blockInfo.size()-1;
			m_blockInfo[size].start = (long)pos;
			m_blockInfo[size].end = (long)pos;

		} else { // выходим из блока
			unsigned short size = (unsigned short)m_blockInfo.size();
			// если мы во вложенном блоке, то удаляем о нем информацию
			if (size > 1) m_blockInfo.pop_back();
			else { // мы в корневых блоках
				if (m_blockInfo[0].end == 0) return false; // в начале файла
				else { // переводим курсор в начало файла
					m_blockInfo[0].start = 0;
					m_blockInfo[0].end = 0;
				}
			}
		}

		if (seekNext) seekNextBlock(); // ставим курсор на первый блок

		return true;
	}

	bool loadINI::LoadNextValue() // загружает следующий параметр в блоке
	{
		if (!m_file) return false;
		m_valueInfo.clear();
		if (!getLine()) {
			return false;
		}

		// проверка на границы блока
		fpos_t pos;
		fgetpos(m_file, &pos);
		if (pos >= m_blockInfo[m_blockInfo.size()-1].end) { // блок закончился
			fseek(m_file, m_blockInfo[m_blockInfo.size()-1].start, SEEK_SET); // на начало блока
			return false;
		}

		char strOld[__INI_BUFFER_SIZE];
		strcpy(strOld, m_buf); // сохраняем строчку

		if (getLine()) { // смотрим на строчку вперед
			fseek(m_file, (long)pos, SEEK_SET); // возвращаем позицию
			if (m_buf[m_lenBuff-1] == '{') return false; // значит это названия блока
		}

		breakLineValue(strOld);

		// если значений одно или ноль, то рекурсия
		if (m_valueInfo.size() < 2) return LoadNextValue();

		return true;
	}

	bool loadINI::getValueName(string & strName) // берет название переменной
	{
		if (m_valueInfo.size() > 1) {
			strName = m_valueInfo[0]; // первое значение
			return true;
		}
		return false;
	}

	bool loadINI::getValue(string & strValue, unsigned char position) // берет значение переменной
	{
		position ++;
		if (m_valueInfo.size() > position) {
			strValue = m_valueInfo[position]; // первое значение
			return true;
		}
		return false;
	}

	bool loadINI::getValue(unsigned int & uValue, unsigned char position) // берет значение переменной
	{
		position ++;
		if (m_valueInfo.size() > position) {
			unsigned long value;
			if (!sscanf(m_valueInfo[position].c_str(), "%U", &value)) return false;
			uValue = value;
			return true;
		}
		return false;
	};

	bool loadINI::getValue(int & iValue, unsigned char position) // берет значение переменной
	{
		position ++;
		if (m_valueInfo.size() > position) {
			long value;
			if (!sscanf(m_valueInfo[position].c_str(), "%I", &value)) return false;
			iValue = value;
			return true;
		}
		return false;
	}

	bool loadINI::getValue(float & fValue, unsigned char position) // берет значение переменной
	{
		position ++;
		if (m_valueInfo.size() > position) {
			float value;
			if (!sscanf(m_valueInfo[position].c_str(), "%f", &value)) return false;
			fValue = value;
			return true;
		}
		return false;
	}

	bool loadINI::getLine() // возвращает строку содержащую данные
	{
		while (true) {
			// берем строку
			if (!fgets(m_buf, __INI_BUFFER_SIZE, m_file)) return false;
			m_lenBuff = (unsigned short)strlen(m_buf);

			while (m_lenBuff != 0) {
				if (m_buf[m_lenBuff-1] > 32) { // есть символ справа
					unsigned short pos=0;
					while (pos < m_lenBuff) { // есть символ слева
						if (m_buf[pos] > 32) {
							if ((m_buf[pos] != '/') || (m_buf[pos] != '#')) return true; // коментарий
							else pos = m_lenBuff;
						}
						pos ++;
					}
					
				}
				m_lenBuff --;
				m_buf[m_lenBuff] = 0;
			}
		}
	}


	void loadINI::breakLineValue(char * strValue) // разбивает строку на значения
	{
		unsigned short size = (unsigned short)strlen(strValue);
		unsigned short pos=0;
		while (pos < size) { // обрезаем первые пробелы
			if (strValue[pos] > 32) { // не пробел
				strValue = &strValue[pos];
				size -= pos;
				pos = size; // выходим из цикла
			}
			pos ++;
		}

		pos = 0;
		bool cov = false; // ковычки
		char * strOld = strValue;
		while (pos < size) { // теперь до первого пробела
			if (strValue[pos] < 33) { // пробел
				if (!cov) { // мы не в ковычках
					strValue[pos] = 0;
					if ((pos+1) < size) {
						strValue = &strValue[pos+1]; // следующий параметр
						breakLineValue(strValue); // рекурсия
					}
					pos = size;
				}
			} else if (strValue[pos] == '"') {
				cov = !cov; // ковычка
			} else if ((strValue[pos] == '/') && (!cov)) { // коментарий не в строке
				strValue[pos] = 0;
				pos = size;
			}
			pos ++;
		}

		if (strOld[0] == '"') {
			strOld[strlen(strOld)-1] = 0; // вторая ковычка
			strOld = &strOld[1]; // первая ковычка
		}
		m_valueInfo.push_front(strOld);
	}
}