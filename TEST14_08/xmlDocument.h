#pragma once

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>

#include "xmlNode.h"

namespace xml
{

	class xmlDocument
	{
	public:
		xmlDocument() : m_root(0), m_info(0), m_lastError(xml::errors::XML_ERROR_NONE), m_line(0), m_col(0) {}
		~xmlDocument() {clear();}

		bool load(std::string _name)
		{
			clear();

			std::ifstream stream;
			stream.open(_name.c_str());
			if (!stream) {
				m_lastError = xml::errors::XML_ERROR_OPEN_FILE;
				return false;
			}

			// ��� ������� ������ ��� �������
			std::string line;
			// ��� ������ �� �����
			std::string read;
			// ������� ���� ��� �������
			xmlNodePtr currentNode = 0;

			while (!stream.eof()) {
				// ����� ����� ������
				std::getline(stream, read);
				m_line ++;
				m_col = 0; // ����� ��������� �� ������������� �����

				if (read.empty()) continue;

				// ������� ������ ��� ������� � �� ��� ��� ���������
				line += read;

				// ������� ���� �� ������� �������
				size_t start = line.find('<');
				if (start == line.npos) continue;

				size_t end = line.find('>', start+1); // ����� ��������� �� ����� ��� ����� �������
				if (end == line.npos) continue;

				// ��������� �� ������� ���� ???
				size_t body = line.find_first_not_of(" \t<");
				if (body < start) {
					if (currentNode == 0) {
						m_lastError = xml::errors::XML_ERROR_BODY_NON_CORRECT;
						stream.close();
						return false;
					}
					std::string body_str = line.substr(0, start);

					// ������� ������
					m_col = body_str.find_first_not_of(" \t");

					trim(body_str);
					currentNode->addBody(body_str);
				}

				// �������� ��� ��� � ������
				if (!parseTag(currentNode, line.substr(start+1, end-start-1))) {
					// ������ ����������� ������
					stream.close();
					return false;
				}

				// � �������� ������� ������ �������
				line = line.substr(end+1);

			}; // while (!stream.eof()) {

			if (currentNode) {
				m_lastError = xml::errors::XML_ERROR_NON_CLOSE_ALL_TAGS;
				stream.close();
				return false;
			}

			if (!m_info) {
				m_lastError = xml::errors::XML_ERROR_DOCUMENT_IS_EMPTY;
				stream.close();
				return false;
			}

			stream.close();
			return true;
		}

		// ��������� ����
		bool save(const std::string & _name)
		{
			if (!m_info) {
				m_lastError = xml::errors::XML_ERROR_DOCUMENT_IS_EMPTY;
				return false;
			}

			std::ofstream stream;
			stream.open(_name.c_str());
			if (!stream.is_open()) {
				m_lastError = xml::errors::XML_ERROR_CREATE_FILE;
				return false;
			}

			// ��������� utf8
			stream << (char)0xEF;
			stream << (char)0xBB;
			stream << (char)0xBF;

			m_info->save(stream, 0);
			if (m_root) m_root->save(stream, 0);

			stream.close();
			return true;
		}

		// ��������� ����
		bool save(const std::wstring & _name)
		{
			if (!m_info) {
				m_lastError = xml::errors::XML_ERROR_DOCUMENT_IS_EMPTY;
				return false;
			}

			std::ofstream stream;
			stream.open(_name.c_str());
			if (!stream.is_open()) {
				m_lastError = xml::errors::XML_ERROR_CREATE_FILE;
				return false;
			}

			// ��������� utf8
			stream << (char)0xEF;
			stream << (char)0xBB;
			stream << (char)0xBF;

			m_info->save(stream, 0);
			if (m_root) m_root->save(stream, 0);

			stream.close();
			return true;
		}

		void clear()
		{
			clearInfo();
			clearRoot();
			m_line = 0;
			m_col = 0;
		}

		const std::string getLastError()
		{
			// ��������� �������� ������
			static const char * errorNamesString[xml::errors::XML_ERROR_COUNT] = {
				"XML_ERROR_NONE",
				"XML_ERROR_OPEN_FILE",
				"XML_ERROR_CREATE_FILE",
				"XML_ERROR_BODY_NON_CORRECT",
				"XML_ERROR_NON_CLOSE_ALL_TAGS",
				"XML_ERROR_DOCUMENT_IS_EMPTY",
				"XML_ERROR_CLOSE_TAG_NOT_FOUND_START_TAG",
				"XML_ERROR_OPEN_CLOSE_NOT_EQVIVALENT",
				"XML_ERROR_INFO_IS_EXIST",
				"XML_ERROR_ROOT_IS_EXIST",
				"XML_ERROR_ATTRIBUTE_NON_CORRECT",
			};

			std::ostringstream stream;
			stream << "'" << errorNamesString[m_lastError] << "'\n";
			stream << "line = " << m_line << " , col = " << m_col;
			return stream.str();
		}

	private:
		bool parseTag(xmlNodePtr &_currentNode, std::string _body)
		{

			// ������� ������
			trim(_body);

			if (_body.empty()) {
				// ������� ������ ���
				if (_currentNode) _currentNode = _currentNode->createChild("");
				else {
					_currentNode = new xmlNode("", 0);
					// ���� ��� ������ �� ����������
					if (!m_root) m_root = _currentNode;
				}
				return true;
			}

			char simbol = _body[0];
			bool tag_info = false;
			
			if (simbol == '!') return true; // ��������� �� ����������

			if (simbol == '?') { // ��������� �� �������������� ���
				tag_info = true;
				_body.erase(0, 1); // ������� ������ ������
			}

			size_t start, end;
			// ��������� �� �������� ����
			if (simbol == '/') {
				if (_currentNode == 0) {
					// ���� �� �������� � ���� ���� � �� �������
					if (!m_root) {
						m_lastError = xml::errors::XML_ERROR_CLOSE_TAG_NOT_FOUND_START_TAG;
						return false;
					}
				}
				// �������� ��� ����
				start = _body.find_first_not_of(" \t", 1);
				if (start == _body.npos) {
					// ��� ������
					_body.clear();
				} else {
					end = _body.find_last_not_of(" \t");
					_body = _body.substr(start, end - start+1);
				}
				// ��������� ������������ ������������ � ������������ �����
				if (_currentNode->getName() != _body) {
					m_lastError = xml::errors::XML_ERROR_OPEN_CLOSE_NOT_EQVIVALENT;
					return false;
				}
				// � ������ ������� ������� ���� ����
				_currentNode = _currentNode->getParent();

			} else {
				// �������� ��� �� ������� ������� ��� ������������ ����
				std::string cut = _body;
				start = _body.find_first_of(" \t/?", 1); // << ������
				if (start != _body.npos) {
					cut = _body.substr(0, start);
					_body = _body.substr(start);
				} else _body.clear();

				if (_currentNode) _currentNode = _currentNode->createChild(cut);
				else {
					if (tag_info) {
						// �������������� ���
						if (m_info) {
							m_lastError = xml::errors::XML_ERROR_INFO_IS_EXIST;
							return false;
						}
						_currentNode = new xmlNode(cut, 0, true);
						m_info = _currentNode;
					} else {
						// ������� ���
						if (m_root) {
							m_lastError = xml::errors::XML_ERROR_ROOT_IS_EXIST;
							return false;
						}
						_currentNode = new xmlNode(cut, 0, false);
						m_root = _currentNode;
					}
				}

				// �������� �� �������
				start = _body.find_last_not_of(" \t");
				if (start == _body.npos) return true;

				// ����� ������� ������������� ���
				bool close = false;
				if ((_body[start] == '/') || (_body[start] == '?')) {
					close = true;
					// �� ����� ������ ������, ������ �������� ������
					_body[start] = ' ';
					// �������� �� �������
					start = _body.find_last_not_of(" \t");
					if (start == _body.npos) {
						// ���������� ��� ����� � ������
						_currentNode = _currentNode->getParent();
						return true;
					}
				}
					
				// � ��� ����� ��� � ����� ��������� �� ��������
				while (true) {
					
					// ���� �����
					start = _body.find('=');
					if (start == _body.npos) {
						m_lastError = xml::errors::XML_ERROR_ATTRIBUTE_NON_CORRECT;
						return false;
					}
					// ���� ������ �������
					end = _body.find('\"', start+1);
					if (end == _body.npos) {
						m_lastError = xml::errors::XML_ERROR_ATTRIBUTE_NON_CORRECT;
						return false;
					}
					end = _body.find('\"', end+1);
					if (end == _body.npos) {
						m_lastError = xml::errors::XML_ERROR_ATTRIBUTE_NON_CORRECT;
						return false;
					}

					std::string key = _body.substr(0, start);
					std::string value = _body.substr(start+1, end-start);

					// �������� �� ����������
					if (! checkPair(key, value)) {
						m_lastError = xml::errors::XML_ERROR_ATTRIBUTE_NON_CORRECT;
						return false;
					}

					// ��������� ���� � ����
					_currentNode->addAttributes(key, value);

					// ��������� �����
					_body = _body.substr(end+1);

					// � ������ �� �������� ��������
					start = _body.find_first_not_of(" \t");
					if (start == _body.npos) break;

					m_col += start;

				};

				// ��� ����������� ��� ��� �������� ����
				if (close) {
					// �� ��������� �����, ������ ��� ��� ��� ���
					_currentNode = _currentNode->getParent();
				}

			}
			return true;
		}

		bool checkPair(std::string &_key, std::string &_value)
		{
			// � ����� �� ������ ���� ������� � ��������
			trim(_key);
			if (_key.empty()) return false;
			size_t start = _key.find_first_of(" \t\"\'");
			if (start != _key.npos) return false;
			// � ��������, ������� �� �����
			trim(_value);
			if (_value.size() < 2) return false;
			if ((_value[0] != '"') || (_value[_value.length()-1] != '"')) return false;
			_value.erase(0, 1);
			_value.erase(_value.length() - 1);
			return true;
		}

		// ������� ������� �����
		inline void trimLeft(std::string &_str)
		{
			// ���� �����, �� ������
			if (_str.empty()) return;
			// ���� ������ �� ������ � �� ���������
			size_t start = _str.find_first_not_of(" \t");
			// ���� ��� ������, �� ��������
			if (start == _str.npos) {_str.clear(); return;}
			else if (start == 0) return;
			// ������� �����
			_str.erase(0, start);
		}

		// ������� ������� �����
		inline void trimRight(std::string &_str)
		{
			// ���� �����, �� ������
			if (_str.empty()) return;
			// ���� ��������� �� ������ � �� ���������
			size_t end = _str.find_last_not_of(" \t");
			// ���� ��� ������, �� ��������
			if (end == _str.npos) {_str.clear(); return;}
			else if (end == (_str.length()-1) ) return;
			// ������� �����
			_str.erase(end+1);
		}

		// ������� ������� �������
		inline void trim(std::string &_str)
		{
			// ���� �����, �� ������
			if (_str.empty()) return;
			// ���� ������ �� ������ � �� ���������
			size_t start = _str.find_first_not_of(" \t");
			// ���� ��� ������, �� ��������
			if (start == _str.npos) {_str.clear(); return;}
			else if (start == 0) return;
			// ������� �����
			_str.erase(0, start);
			// ���� ��������� �� ������ � �� ���������
			size_t end = _str.find_last_not_of(" \t");
			if (end == (_str.length()-1) ) return;
			// ������� �����
			_str.erase(end+1);
		}

		inline void clearInfo() {if (m_info) {delete m_info;m_info = 0;}}
		inline void clearRoot() {if (m_root) {delete m_root;m_root = 0;}}

	public:
		xmlNodePtr createInfo()
		{
			clearInfo();
			m_info = new xmlNode("xml", 0, true);
			return m_info;
		}

		xmlNodePtr createRoot(const std::string & _name)
		{
			clearRoot();
			m_root = new xmlNode(_name, 0, false);
			return m_root;
		}

		inline xmlNodePtr getRoot() {return m_root;}

	private:
		xmlNodePtr m_root;
		xmlNodePtr m_info;
		xml::errors::ErrorTypes m_lastError;
		size_t m_line;
		size_t m_col;

	}; // class xmlDocument : public xmlNode

} // namespace xml