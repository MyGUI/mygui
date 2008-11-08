/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_XmlDocument.h"
#include "MyGUI_ResourcePath.h"

#include <OgreResourceGroupManager.h>

#include "MyGUI_LastHeader.h"

namespace MyGUI
{
	namespace xml
	{

		//----------------------------------------------------------------------//
		// class xmlNodeIterator
		//----------------------------------------------------------------------//
		xmlNodeIterator::xmlNodeIterator(VectorNode::iterator _start, VectorNode::iterator _end) :
			m_first(true),
			m_current(_start),
			m_end(_end)
		{
		}

		bool xmlNodeIterator::nextNode()
		{
			if (m_current == m_end) return false;
			else if (m_first) {
				m_first = false;
				return true;
			}
			++ m_current;
			if (m_current == m_end) return false;
			return true;
		}

		bool xmlNodeIterator::nextNode(const std::string & _name)
		{
			while (nextNode()) {
				if ((*m_current)->getName() == _name) return true;
			};
			return false;
		}

		//----------------------------------------------------------------------//
		// class xmlNode
		//----------------------------------------------------------------------//
		xmlNode::xmlNode(const std::string &_name, xmlNodePtr _parent, xmlNodeType _type, const std::string & _body) :
			mName(_name),
			mBody(_body),
			mParent(_parent),
			mType(_type)
		{
		}

		xmlNode::~xmlNode()
		{
			for (VectorNode::iterator iter=mChilds.begin(); iter!=mChilds.end(); ++iter) {
				delete *iter;
			}
			mChilds.clear();
		}

		void xmlNode::save(std::ofstream & _stream, size_t _level)
		{
			// сначала табуляции намутим
			for (size_t tab=0; tab<_level; ++tab) _stream  << "    ";

			// теперь заголовок тега
			if (mType == XML_NODE_TYPE_INFO) _stream << "<?";
			else _stream << "<";
			_stream << mName;

			for (VectorAttributes::iterator iter = mAttributes.begin(); iter != mAttributes.end(); ++iter) {
				_stream << " " << iter->first << "=\"" << utility::convert_to_xml(iter->second) << "\"";
			}

			bool empty = mChilds.empty();
			// если детей нет то закрываем
			if (empty && mBody.empty()) {
				if (mType == XML_NODE_TYPE_INFO) _stream << "?>\n";
				else _stream << "/>\n";
			}
			else {
				_stream << ">";
				if (!empty) _stream << "\n";
				// если есть тело то сначало оно
				if (!mBody.empty()) {
					if (!empty) {
						for (size_t tab=0; tab<=_level; ++tab) _stream  << "    ";
					}
					_stream << utility::convert_to_xml(mBody);

					if (!empty) _stream << "\n";
				}
				// если есть детишки путь сохранятся
				for (size_t child=0; child<mChilds.size(); child++) {
					mChilds[child]->save(_stream, _level + 1);
				}

				if (!empty) {for (size_t tab=0; tab<_level; ++tab) _stream  << "    ";}
				_stream << "</" << mName << ">\n";
			}

		}

		xmlNodePtr xmlNode::createChild(const std::string & _name, const std::string & _body)
		{
			xmlNodePtr node = new xmlNode(_name, this, XML_NODE_TYPE_NORMAL, _body);
			mChilds.push_back(node);
			return node;
		}

		void xmlNode::clear()
		{
			for (VectorNode::iterator iter = mChilds.begin(); iter != mChilds.end(); ++iter) delete *iter;
			mChilds.clear();
			mBody.clear();
			mAttributes.clear();
		}

		bool xmlNode::findAttribute(const std::string & _name, std::string & _value)
		{
			for (VectorAttributes::iterator iter=mAttributes.begin(); iter!=mAttributes.end(); ++iter) {
				if ( (*iter).first == _name) {
					_value = (*iter).second;
					return true;
				}
			}
			return false;
		}

		std::string xmlNode::findAttribute(const std::string & _name)
		{
			for (VectorAttributes::iterator iter=mAttributes.begin(); iter!=mAttributes.end(); ++iter) {
				if ( (*iter).first == _name) return (*iter).second;
			}
			return "";
		}

		//----------------------------------------------------------------------//
		// class xmlDocument
		//----------------------------------------------------------------------//
		xmlDocument::xmlDocument():
			mRoot(0),
			mInfo(0),
			mLastError(xml::errors::XML_ERROR_NONE),
			mLastErrorFile(""),
			mLine(0),
			mCol(0)
		{
		}

		xmlDocument::~xmlDocument()
		{
			clear();
		}

		bool xmlDocument::open(const std::string & _name, const std::string & _group)
		{
			if (_group.empty()) return open(_name);

			if (!helper::isFileExist(_name, _group)) {
				mLastError = xml::errors::XML_ERROR_OPEN_FILE;
				return false;
			}

			Ogre::DataStreamPtr fileStream = Ogre::ResourceGroupManager::getSingleton().openResource(_name, _group);
			return open(fileStream);
		}

		bool xmlDocument::open(const Ogre::DataStreamPtr& stream)
		{
			clear();

			mLastErrorFile = stream->getName();
			// это текущая строка для разбора
			std::string line;
			// это строка из файла
			std::string read;
			// текущий узел для разбора
			xmlNodePtr currentNode = 0;

			while (false == stream->eof()) {
				// берем новую строку
				read = stream->getLine (false);
				mLine ++;
				mCol = 0; // потом проверить на многострочных тэгах

				if (read.empty()) continue;

				// текущая строка для разбора и то что еще прочитали
				line += read;

				// крутимся пока в строке есть теги
				while (true) {

					// сначала ищем по угловым скобкам
					size_t start = find(line, '<');
					if (start == line.npos) break;
					size_t end = line.npos;

					// пытаемся вырезать многострочный коментарий
					if ((start + 3 < line.size()) && (line[start + 1] == '!') && (line[start + 2] == '-') && (line[start + 3] == '-')) {
						end = line.find("-->", start + 4);
						if (end == line.npos) break;
						end += 2;

					}
					else {
						end = find(line, '>', start+1);
						if (end == line.npos) break;

					}

					// проверяем на наличее тела
					size_t body = line.find_first_not_of(" \t<");
					if (body < start) {

						std::string body_str = line.substr(0, start);
						// текущий символ
						mCol = 0;

						if (currentNode != 0) 	{
							bool ok = true;
							currentNode->setBody(utility::convert_from_xml(body_str, ok));
							if (!ok) {
								mLastError = xml::errors::XML_ERROR_BODY_NON_CORRECT;
								return false;
							}
						}

					}

					// вырезаем наш тэг и парсим
					if (false == parseTag(currentNode, line.substr(start+1, end-start-1))) {
						// ошибка установится внутри
						return false;
					}

					// и обрезаем текущую строку разбора
					line = line.substr(end+1);

				}; // while (true)

			}; // while (!stream.eof())

			if (currentNode) {
				mLastError = xml::errors::XML_ERROR_NON_CLOSE_ALL_TAGS;
				return false;
			}

			return true;
		}

		bool xmlDocument::open(const std::string & _name)
		{
			clear();

			std::ifstream stream(_name.c_str());
			mLastErrorFile = _name;
			if (false == stream.is_open()) {
				mLastError = xml::errors::XML_ERROR_OPEN_FILE;
				return false;
			}
			// это текущая строка для разбора
			std::string line;
			// это строка из файла
			std::string read;
			// текущий узел для разбора
			xmlNodePtr currentNode = 0;

			while (false == stream.eof()) {
				// берем новую строку
				std::getline(stream, read);
				mLine ++;
				mCol = 0; // потом проверить на многострочных тэгах

				if (read.empty()) continue;

				// текущая строка для разбора и то что еще прочитали
				line += read;

				// крутимся пока в строке есть теги
				while (true) {

					// сначала ищем по угловым скобкам
					size_t start = find(line, '<');
					if (start == line.npos) break;
					size_t end = line.npos;

					// пытаемся вырезать многострочный коментарий
					if ((start + 3 < line.size()) && (line[start + 1] == '!') && (line[start + 2] == '-') && (line[start + 3] == '-')) {
						end = line.find("-->", start + 4);
						if (end == line.npos) break;
						end += 2;

					}
					else {
						end = find(line, '>', start+1);
						if (end == line.npos) break;

					}

					// проверяем на наличее тела
					size_t body = line.find_first_not_of(" \t<");
					if (body < start) {

						std::string body_str = line.substr(0, start);
						// текущий символ
						mCol = 0;

						if (currentNode != 0) 	{
							bool ok = true;
							currentNode->setBody(utility::convert_from_xml(body_str, ok));
							if (!ok) {
								mLastError = xml::errors::XML_ERROR_BODY_NON_CORRECT;
								return false;
							}
						}

					}

					// вырезаем наш тэг и парсим
					if (false == parseTag(currentNode, line.substr(start+1, end-start-1))) {
						// ошибка установится внутри
						stream.close();
						return false;
					}

					// и обрезаем текущую строку разбора
					line = line.substr(end+1);

				}; // while (true)

			}; // while (!stream.eof())

			if (currentNode) {
				mLastError = xml::errors::XML_ERROR_NON_CLOSE_ALL_TAGS;
				stream.close();
				return false;
			}

			stream.close();
			return true;
		}

		// сохраняет файл
		bool xmlDocument::save(const std::string & _name)
		{
			if (!mInfo) {
				mLastError = xml::errors::XML_ERROR_DOCUMENT_IS_EMPTY;
				mLastErrorFile = _name;
				return false;
			}

			std::ofstream stream(_name.c_str());
			if (!stream.is_open()) {
				mLastError = xml::errors::XML_ERROR_CREATE_FILE;
				mLastErrorFile = _name;
				return false;
			}

			// заголовок utf8
			stream << (char)0xEF;
			stream << (char)0xBB;
			stream << (char)0xBF;

			mInfo->save(stream, 0);
			if (mRoot) mRoot->save(stream, 0);

			stream.close();
			return true;
		}

		void xmlDocument::clear()
		{
			clearInfo();
			clearRoot();
			mLine = 0;
			mCol = 0;
		}

		const std::string xmlDocument::getLastError()
		{
			if (0 == mLastError) return "";
			// текстовое описание ошибок
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
			stream << "'" << errorNamesString[mLastError] << "' " << mLastErrorFile;
			if (xml::errors::XML_ERROR_OPEN_FILE != mLastError)
				stream << ",  "<< "line=" << (unsigned int)mLine << " , col=" << (unsigned int)mCol;
			return stream.str();
		}

		bool xmlDocument::parseTag(xmlNodePtr &_currentNode, std::string _body)
		{

			// убераем лишнее
			utility::trim(_body);

			if (_body.empty()) {
				// создаем пустой тег
				if (_currentNode) _currentNode = _currentNode->createChild("");
				else {
					_currentNode = new xmlNode("", 0);
					// если это первый то запоминаем
					if (!mRoot) mRoot = _currentNode;
				}
				return true;
			}

			char simbol = _body[0];
			bool tag_info = false;

			if (simbol == '!') return true; // проверяем на коментарии

			if (simbol == '?') { // проверяем на информационный тег
				tag_info = true;
				_body.erase(0, 1); // удаляем первый символ
			}

			size_t start, end;
			// проверяем на закрытие тега
			if (simbol == '/') {
				if (_currentNode == 0) {
					// чета мы закрывам а ниче даже и не открыто
					if (!mRoot) {
						mLastError = xml::errors::XML_ERROR_CLOSE_TAG_NOT_FOUND_START_TAG;
						return false;
					}
				}
				// обрезаем имя тэга
				start = _body.find_first_not_of(" \t", 1);
				if (start == _body.npos) {
					// тег пустой
					_body.clear();
				} else {
					end = _body.find_last_not_of(" \t");
					_body = _body.substr(start, end - start+1);
				}
				// проверяем соответствие открывающего и закрывающего тегов
				if (_currentNode->getName() != _body) {
					mLastError = xml::errors::XML_ERROR_OPEN_CLOSE_NOT_EQVIVALENT;
					return false;
				}
				// а теперь снижаем текущий узел вниз
				_currentNode = _currentNode->getParent();

			} else {
				// выделяем имя до первого пробела или закрывающего тега
				std::string cut = _body;
				start = _body.find_first_of(" \t/?", 1); // << превед
				if (start != _body.npos) {
					cut = _body.substr(0, start);
					_body = _body.substr(start);
				} else _body.clear();

				if (_currentNode) _currentNode = _currentNode->createChild(cut);
				else {
					if (tag_info) {
						// информационный тег
						if (mInfo) {
							mLastError = xml::errors::XML_ERROR_INFO_IS_EXIST;
							return false;
						}
						_currentNode = new xmlNode(cut, 0, XML_NODE_TYPE_INFO);
						mInfo = _currentNode;
					} else {
						// рутовый тег
						if (mRoot) {
							mLastError = xml::errors::XML_ERROR_ROOT_IS_EXIST;
							return false;
						}
						_currentNode = new xmlNode(cut, 0, XML_NODE_TYPE_NORMAL);
						mRoot = _currentNode;
					}
				}

				// проверим на пустоту
				start = _body.find_last_not_of(" \t");
				if (start == _body.npos) return true;

				// сразу отделим закрывающийся тэг
				bool close = false;
				if ((_body[start] == '/') || (_body[start] == '?')) {
					close = true;
					// не будем резать строку, просто поставим пробел
					_body[start] = ' ';
					// проверим на пустоту
					start = _body.find_last_not_of(" \t");
					if (start == _body.npos) {
						// возвращаем все назад и уходим
						_currentNode = _currentNode->getParent();
						return true;
					}
				}

				// а вот здесь уже в цикле разбиваем на атрибуты
				while (true) {

					// ищем равно
					start = _body.find('=');
					if (start == _body.npos) {
						mLastError = xml::errors::XML_ERROR_ATTRIBUTE_NON_CORRECT;
						return false;
					}
					// ищем вторые ковычки
					end = _body.find('\"', start+1);
					if (end == _body.npos) {
						mLastError = xml::errors::XML_ERROR_ATTRIBUTE_NON_CORRECT;
						return false;
					}
					end = _body.find('\"', end+1);
					if (end == _body.npos) {
						mLastError = xml::errors::XML_ERROR_ATTRIBUTE_NON_CORRECT;
						return false;
					}

					std::string key = _body.substr(0, start);
					std::string value = _body.substr(start+1, end-start);

					// проверка на валидность
					if (! checkPair(key, value)) {
						mLastError = xml::errors::XML_ERROR_ATTRIBUTE_NON_CORRECT;
						return false;
					}

					// добавляем пару в узел
					_currentNode->addAttributes(key, value);

					// следующий кусок
					_body = _body.substr(end+1);

					// в строке не осталось символов
					start = _body.find_first_not_of(" \t");
					if (start == _body.npos) break;

					mCol += start;

				};

				// был закрывающий тег для текущего тега
				if (close) {
					// не проверяем имена, потому что это наш тэг
					_currentNode = _currentNode->getParent();
				}

			}
			return true;
		}

		bool xmlDocument::checkPair(std::string &_key, std::string &_value)
		{
			// в ключе не должно быть ковычек и пробелов
			utility::trim(_key);
			if (_key.empty()) return false;
			size_t start = _key.find_first_of(" \t\"\'&");
			if (start != _key.npos) return false;

			// в значении, ковычки по бокам
			utility::trim(_value);
			if (_value.size() < 2) return false;
			if ((_value[0] != '"') || (_value[_value.length()-1] != '"')) return false;
			bool ok = true;
			_value = utility::convert_from_xml(_value.substr(1, _value.length() - 2), ok);
			return ok;
		}

		// ищет символ без учета ковычек
		size_t xmlDocument::find(const std::string & _text, char _char, size_t _start)
		{
			// ковычки
			bool kov = false;

			// буфер для поиска
			char buff[16] = "\"_\0";
			buff[1] = _char;

			size_t pos = _start;

			while (true) {

				pos = _text.find_first_of(buff, pos);

				// если уже конец, то досвидания
				if (pos == _text.npos) break;

				// нашли ковычку
				else if (_text[pos] == '"') {
					kov = !kov;
					pos ++;
				}
				// если мы в ковычках, то идем дальше
				else if (kov) pos ++;

				// мы не в ковычках
				else break;

			}; //	while (true) {

			return pos;
		}

		void xmlDocument::clearInfo()
		{
			if (mInfo) {
				delete mInfo;
				mInfo = 0;
			}
		}

		void xmlDocument::clearRoot()
		{
			if (mRoot) {
				delete mRoot;
				mRoot = 0;
			}
		}

		xmlNodePtr xmlDocument::createInfo(const std::string & _version, const std::string & _encoding)
		{
			clearInfo();
			mInfo = new xmlNode("xml", 0, XML_NODE_TYPE_INFO);
			mInfo->addAttributes("version", _version);
			mInfo->addAttributes("encoding", _encoding);
			return mInfo;
		}

		xmlNodePtr xmlDocument::createRoot(const std::string & _name)
		{
			clearRoot();
			mRoot = new xmlNode(_name, 0, XML_NODE_TYPE_NORMAL);
			return mRoot;
		}

		namespace utility
		{
			std::string convert_from_xml(const std::string & _string, bool & _ok)
			{
				std::string ret;
				_ok = true;

				size_t pos = _string.find("&");
				if (pos == std::string::npos) return _string;

				ret.reserve(_string.size());
				size_t old = 0;
				while (pos != std::string::npos) {
					ret += _string.substr(old, pos - old);

					size_t end = _string.find(";", pos + 1);
					if (end == std::string::npos) {
						_ok = false;
						return ret;
					}
					else {
						std::string tag = _string.substr(pos, end - pos + 1);
						if (tag == "&amp;") ret += '&';
						else if (tag == "&lt;") ret += '<';
						else if (tag == "&gt;") ret += '>';
						else if (tag == "&apos;") ret += '\'';
						else if (tag == "&quot;") ret += '\"';
						else {
							_ok = false;
							return ret;
						}
					}

					old = end + 1;
					pos = _string.find("&", old);
				};
				ret += _string.substr(old, std::string::npos);

				return ret;
			}

			std::string convert_to_xml(const std::string & _string)
			{
				std::string ret;

				size_t pos = _string.find_first_of("&<>'\"");
				if (pos == std::string::npos) return _string;

				ret.reserve(_string.size() * 2);
				size_t old = 0;
				while (pos != std::string::npos) {
					ret += _string.substr(old, pos - old);

					if (_string[pos] == '&') ret += "&amp;";
					else if (_string[pos] == '<') ret += "&lt;";
					else if (_string[pos] == '>') ret += "&gt;";
					else if (_string[pos] == '\'') ret += "&apos;";
					else if (_string[pos] == '\"') ret += "&quot;";

					old = pos + 1;
					pos = _string.find_first_of("&<>'\"", old);
				};
				ret += _string.substr(old, std::string::npos);

				return ret;
			}

		}

	} // namespace xml

} // namespace MyGUI
