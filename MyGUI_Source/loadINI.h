//=========================================================================================
#ifndef __loadINI_H__
#define __loadINI_H__
//=========================================================================================
//using namespace Ogre;
using namespace std;
//=========================================================================================
#include <fstream>
//=========================================================================================
#define __INI_BUFFER_SIZE 256
//=========================================================================================
//typedef unsigned short uint16;
//=========================================================================================
class loadINI
{
	public:
		typedef struct __tag_INI_BLOCK_INFO {
			unsigned short start; // ������ ����� � ������
			unsigned short end; // ����� ����� � ������
			string typeBlock; // ��� �����
			string nameBlock; // ��� �����
		};
	public:
		loadINI() :
			m_file(0)
		{
		};

		~loadINI()
		{
			close();
		};

		bool open(const string & strFileName) // �������� �����
		{
			close();
			m_file = fopen(strFileName.c_str(), "r");
			if (m_file == 0) return false;

			__tag_INI_BLOCK_INFO data;
			m_blockInfo.push_back(data);
			m_blockInfo[0].start = 0;
			m_blockInfo[0].end = 0;

			return true;
		};

		void close() // �������� �����
		{
			if (m_file != 0) {
				fclose(m_file);
				m_file = 0;
			}
			m_blockInfo.clear();
		};

		bool seekNextBlock() // ���� ��������� ����
		{
			if (!m_file) return false;
			// ������������� ������� �� ����� �����
			fseek(m_file, m_blockInfo[m_blockInfo.size()-1].end, SEEK_SET);

			char strOld[__INI_BUFFER_SIZE];
			strOld[0] = 0;

			unsigned char CountSep = 1;
			while (getLine()) {

				if (m_buf[m_lenBuff-1] == '{') { // ������ �����
					CountSep++;
				} else if (m_buf[m_lenBuff-1] == '}') { // ����� �����
					CountSep--;
					if (CountSep == 0) return false; // �� ���� �������
					if (CountSep == 1) { // ���� ����������
						unsigned short index = (unsigned short)m_blockInfo.size()-1;
						m_blockInfo[index].start = m_blockInfo[m_blockInfo.size()-1].end;
						fpos_t pos;
						fgetpos(m_file, &pos);
						m_blockInfo[index].end = pos;
						m_blockInfo[index].typeBlock.clear();
						m_blockInfo[index].nameBlock.clear();

						pos = strlen(strOld);
						int start = -1;
						for (unsigned short i=0; i<pos; i++) {
							if (start == -1) {
								if (strOld[i] > 32) start = i; // ���������� ������ �������
							} else {
								if (strOld[i] <= 32) { // � ��������� ������� ��� ������ �������
									strOld[i] = 0;
									if ((i+1) < pos) {
										m_blockInfo[index].nameBlock = &strOld[i+1];
									}
								}
							}
						}
						if (start != -1) m_blockInfo[index].typeBlock = &strOld[start]; // ��� ������ ��������

						fseek(m_file, m_blockInfo[m_blockInfo.size()-1].start, SEEK_SET); // ��������� �� ������ �����
						while (getLine()) {// � �������� ������� ����� "{"
							if (m_buf[m_lenBuff-1] == '{') break;
						}

						return true;
					}
				} else if (CountSep == 1) { // ��� �� ������ �����
					strcpy(strOld, m_buf); // ��������� ������
				}
			};

			return false;
		};

		bool seekTypeBlock(const string & strType) // ���� ���� �� ����
		{
			while (seekNextBlock()) {
				if (m_blockInfo[m_blockInfo.size()-1].typeBlock == strType) return true;
			}
			return false;
		};

		bool seekNameBlock(const string & strName) // ���� ���� �� �����
		{
			while (seekNextBlock()) {
				if (m_blockInfo[m_blockInfo.size()-1].nameBlock == strName) return true;
			}
			return false;
		};

		const string & getBlockName() // ���������� ��� �������� �����
		{
			if (!m_file) return m_strEmptyString;
			return m_blockInfo[m_blockInfo.size()-1].nameBlock;
		};

		const string & getBlockType() // ���������� ��� �������� �����
		{
			if (!m_file) return m_strEmptyString;
			return m_blockInfo[m_blockInfo.size()-1].typeBlock;
		};

		bool jampBlock(bool jampEnter, bool seekNext = true) // ������� ���� ������ ���� ������ �����
		{
			if (!m_file) return false;

			if (jampEnter) { // ������� �� ������ �����
				fseek(m_file, m_blockInfo[m_blockInfo.size()-1].start, SEEK_SET);
				 // �������� ������� �� ���� ������ ������
				while (true) {
					if (!getLine()) return false;
					if (m_buf[m_lenBuff-1] == '{') break;
				}
				fpos_t pos;
				fgetpos(m_file, &pos);
				__tag_INI_BLOCK_INFO data;
				m_blockInfo.push_back(data);
				unsigned short size = (unsigned short)m_blockInfo.size()-1;
				m_blockInfo[size].start = pos;
				m_blockInfo[size].end = pos;

			} else { // ������� �� �����
				unsigned short size = (unsigned short)m_blockInfo.size();
				// ���� �� �� ��������� �����, �� ������� � ��� ����������
				if (size > 1) m_blockInfo.pop_back();
				else { // �� � �������� ������
					if (m_blockInfo[0].end == 0) return false; // � ������ �����
					else { // ��������� ������ � ������ �����
						m_blockInfo[0].start = 0;
						m_blockInfo[0].end = 0;
					}
				}
			}

			if (seekNext) seekNextBlock(); // ������ ������ �� ������ ����

			return true;
		};

		bool LoadNextValue() // ��������� ��������� �������� � �����
		{
			if (!m_file) return false;
			m_valueInfo.clear();
			if (!getLine()) {
				return false;
			}

			// �������� �� ������� �����
			fpos_t pos;
			fgetpos(m_file, &pos);
			if (pos >= m_blockInfo[m_blockInfo.size()-1].end) { // ���� ����������
				fseek(m_file, m_blockInfo[m_blockInfo.size()-1].start, SEEK_SET); // �� ������ �����
				return false;
			}

			char strOld[__INI_BUFFER_SIZE];
			strcpy(strOld, m_buf); // ��������� �������

			if (getLine()) { // ������� �� ������� ������
				fseek(m_file, pos, SEEK_SET); // ���������� �������
				if (m_buf[m_lenBuff-1] == '{') return false; // ������ ��� �������� �����
			}

			breakLineValue(strOld);

			// ���� �������� ���� ��� ����, �� ��������
			if (m_valueInfo.size() < 2) return LoadNextValue();

			return true;
		}

		bool getValueName(string & strName) // ����� �������� ����������
		{
			if (m_valueInfo.size() > 1) {
				strName = m_valueInfo[0]; // ������ ��������
				return true;
			}
			return false;
		};

		bool getValue(string & strValue, unsigned char position = 0) // ����� �������� ����������
		{
			position ++;
			if (m_valueInfo.size() > position) {
				strValue = m_valueInfo[position]; // ������ ��������
				return true;
			}
			return false;
		};

		bool getValue(unsigned int & uValue, unsigned char position = 0) // ����� �������� ����������
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

		bool getValue(int & iValue, unsigned char position = 0) // ����� �������� ����������
		{
			position ++;
			if (m_valueInfo.size() > position) {
				long value;
				if (!sscanf(m_valueInfo[position].c_str(), "%I", &value)) return false;
				iValue = value;
				return true;
			}
			return false;
		};

		bool getValue(float & fValue, unsigned char position = 0) // ����� �������� ����������
		{
			position ++;
			if (m_valueInfo.size() > position) {
				float value;
				if (!sscanf(m_valueInfo[position].c_str(), "%f", &value)) return false;
				fValue = value;
				return true;
			}
			return false;
		};

protected:

		bool getLine() // ���������� ������ ���������� ������
		{
			while (true) {
				// ����� ������
				if (!fgets(m_buf, __INI_BUFFER_SIZE, m_file)) return false;
				m_lenBuff = (unsigned short)strlen(m_buf);

				while (m_lenBuff != 0) {
					if (m_buf[m_lenBuff-1] > 32) { // ���� ������ ������
						unsigned short pos=0;
						while (pos < m_lenBuff) { // ���� ������ �����
							if (m_buf[pos] > 32) {
								if ((m_buf[pos] != '/') || (m_buf[pos] != '#')) return true; // ����������
								else pos = m_lenBuff;
							}
							pos ++;
						}
						
					}
					m_lenBuff --;
					m_buf[m_lenBuff] = 0;
				};
			};
		};


		void breakLineValue(char * strValue) // ��������� ������ �� ��������
		{
			unsigned short size = (unsigned short)strlen(strValue);
			unsigned short pos=0;
			while (pos < size) { // �������� ������ �������
				if (strValue[pos] > 32) { // �� ������
					strValue = &strValue[pos];
					size -= pos;
					pos = size; // ������� �� �����
				}
				pos ++;
			}

			pos = 0;
			bool cov = false; // �������
			char * strOld = strValue;
			while (pos < size) { // ������ �� ������� �������
				if (strValue[pos] < 33) { // ������
					if (!cov) { // �� �� � ��������
						strValue[pos] = 0;
						if ((pos+1) < size) {
							strValue = &strValue[pos+1]; // ��������� ��������
							breakLineValue(strValue); // ��������
						}
						pos = size;
					}
				} else if (strValue[pos] == '"') {
					cov = !cov; // �������
				} else if ((strValue[pos] == '/') && (!cov)) { // ���������� �� � ������
					strValue[pos] = 0;
					pos = size;
				}
				pos ++;
			}

			if (strOld[0] == '"') {
				strOld[strlen(strOld)-1] = 0; // ������ �������
				strOld = &strOld[1]; // ������ �������
			}
			m_valueInfo.push_front(strOld);
		}

		FILE * m_file; // ���������� ��������� �����
		vector <__tag_INI_BLOCK_INFO> m_blockInfo;

		char m_buf[__INI_BUFFER_SIZE]; // ����� ��� ���������� ������
		unsigned short m_lenBuff; // ������ ������ � ������

		deque <string> m_valueInfo; // ������� ����������� ������ � ��������� �����������

		string m_strEmptyString; // ������ ������ ��� �������� ������

};

//=========================================================================================
#endif
//=========================================================================================
