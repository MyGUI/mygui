/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#include "MyGUI_XmlDocument.h"

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
			// ������� ��������� �������
			for (size_t tab=0; tab<_level; ++tab) _stream  << "    ";

			// ������ ��������� ����
			if (mType == XML_NODE_TYPE_INFO) _stream << "<?";
			else _stream << "<";
			_stream << mName;

			for (VectorAttributes::iterator iter = mAttributes.begin(); iter != mAttributes.end(); ++iter) {
				_stream << " " << iter->first << "=\"" << utility::convert_to_xml(iter->second) << "\"";
			}

			bool empty = mChilds.empty();
			// ���� ����� ��� �� ���������
			if (empty && mBody.empty()) {
				if (mType == XML_NODE_TYPE_INFO) _stream << "?>\n";
				else _stream << "/>\n";
			}
			else {
				_stream << ">";
				if (!empty) _stream << "\n";
				// ���� ���� ���� �� ������� ���
				if (!mBody.empty()) {
					if (!empty) {
						for (size_t tab=0; tab<=_level; ++tab) _stream  << "    ";
					}
					_stream << utility::convert_to_xml(mBody);

					if (!empty) _stream << "\n";
				}
				// ���� ���� ������� ���� ����������
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

		bool xmlDocument::open(const Ogre::DataStreamPtr& stream)
		{
			clear();

			mLastErrorFile = stream->getName();
			// ��� ������� ������ ��� �������
			std::string line;
			// ��� ������ �� �����
			std::string read;
			// ������� ���� ��� �������
			xmlNodePtr currentNode = 0;

			while (false == stream->eof()) {
				// ����� ����� ������
				read = stream->getLine (false);
				mLine ++;
				mCol = 0; // ����� ��������� �� ������������� �����

				if (read.empty()) continue;

				// ������� ������ ��� ������� � �� ��� ��� ���������
				line += read;

				// �������� ���� � ������ ���� ����
				while (true) {

					// ������� ���� �� ������� �������
					size_t start = find(line, '<');
					if (start == line.npos) break;
					size_t end = line.npos;

					// �������� �������� ������������� ����������
					if ((start + 3 < line.size()) && (line[start + 1] == '!') && (line[start + 2] == '-') && (line[start + 3] == '-')) {
						end = line.find("-->", start + 4);
						if (end == line.npos) break;
						end += 2;

					}
					else {
						end = find(line, '>', start+1);
						if (end == line.npos) break;

					}

					// ��������� �� ������� ����
					size_t body = line.find_first_not_of(" \t<");
					if (body < start) {

						std::string body_str = line.substr(0, start);
						// ������� ������
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

					// �������� ��� ��� � ������
					if (false == parseTag(currentNode, line.substr(start+1, end-start-1))) {
						// ������ ����������� ������
						return false;
					}

					// � �������� ������� ������ �������
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
			// ��� ������� ������ ��� �������
			std::string line;
			// ��� ������ �� �����
			std::string read;
			// ������� ���� ��� �������
			xmlNodePtr currentNode = 0;

			while (false == stream.eof()) {
				// ����� ����� ������
				std::getline(stream, read);
				mLine ++;
				mCol = 0; // ����� ��������� �� ������������� �����

				if (read.empty()) continue;

				// ������� ������ ��� ������� � �� ��� ��� ���������
				line += read;

				// �������� ���� � ������ ���� ����
				while (true) {

					// ������� ���� �� ������� �������
					size_t start = find(line, '<');
					if (start == line.npos) break;
					size_t end = line.npos;

					// �������� �������� ������������� ����������
					if ((start + 3 < line.size()) && (line[start + 1] == '!') && (line[start + 2] == '-') && (line[start + 3] == '-')) {
						end = line.find("-->", start + 4);
						if (end == line.npos) break;
						end += 2;

					}
					else {
						end = find(line, '>', start+1);
						if (end == line.npos) break;

					}

					// ��������� �� ������� ����
					size_t body = line.find_first_not_of(" \t<");
					if (body < start) {

						std::string body_str = line.substr(0, start);
						// ������� ������
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

					// �������� ��� ��� � ������
					if (false == parseTag(currentNode, line.substr(start+1, end-start-1))) {
						// ������ ����������� ������
						stream.close();
						return false;
					}

					// � �������� ������� ������ �������
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

		// ��������� ����
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

			// ��������� utf8
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
			stream << "'" << errorNamesString[mLastError] << "' " << mLastErrorFile;
			if (xml::errors::XML_ERROR_OPEN_FILE != mLastError)
				stream << ",  "<< "line=" << (unsigned int)mLine << " , col=" << (unsigned int)mCol;
			return stream.str();
		}

		bool xmlDocument::parseTag(xmlNodePtr &_currentNode, std::string _body)
		{

			// ������� ������
			utility::trim(_body);

			if (_body.empty()) {
				// ������� ������ ���
				if (_currentNode) _currentNode = _currentNode->createChild("");
				else {
					_currentNode = new xmlNode("", 0);
					// ���� ��� ������ �� ����������
					if (!mRoot) mRoot = _currentNode;
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
					if (!mRoot) {
						mLastError = xml::errors::XML_ERROR_CLOSE_TAG_NOT_FOUND_START_TAG;
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
					mLastError = xml::errors::XML_ERROR_OPEN_CLOSE_NOT_EQVIVALENT;
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
						if (mInfo) {
							mLastError = xml::errors::XML_ERROR_INFO_IS_EXIST;
							return false;
						}
						_currentNode = new xmlNode(cut, 0, XML_NODE_TYPE_INFO);
						mInfo = _currentNode;
					} else {
						// ������� ���
						if (mRoot) {
							mLastError = xml::errors::XML_ERROR_ROOT_IS_EXIST;
							return false;
						}
						_currentNode = new xmlNode(cut, 0, XML_NODE_TYPE_NORMAL);
						mRoot = _currentNode;
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
						mLastError = xml::errors::XML_ERROR_ATTRIBUTE_NON_CORRECT;
						return false;
					}
					// ���� ������ �������
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

					// �������� �� ����������
					if (! checkPair(key, value)) {
						mLastError = xml::errors::XML_ERROR_ATTRIBUTE_NON_CORRECT;
						return false;
					}

					// ��������� ���� � ����
					_currentNode->addAttributes(key, value);

					// ��������� �����
					_body = _body.substr(end+1);

					// � ������ �� �������� ��������
					start = _body.find_first_not_of(" \t");
					if (start == _body.npos) break;

					mCol += start;

				};

				// ��� ����������� ��� ��� �������� ����
				if (close) {
					// �� ��������� �����, ������ ��� ��� ��� ���
					_currentNode = _currentNode->getParent();
				}

			}
			return true;
		}

		bool xmlDocument::checkPair(std::string &_key, std::string &_value)
		{
			// � ����� �� ������ ���� ������� � ��������
			utility::trim(_key);
			if (_key.empty()) return false;
			size_t start = _key.find_first_of(" \t\"\'&");
			if (start != _key.npos) return false;

			// � ��������, ������� �� �����
			utility::trim(_value);
			if (_value.size() < 2) return false;
			if ((_value[0] != '"') || (_value[_value.length()-1] != '"')) return false;
			bool ok = true;
			_value = utility::convert_from_xml(_value.substr(1, _value.length() - 2), ok);
			return ok;
		}

		// ���� ������ ��� ����� �������
		size_t xmlDocument::find(const std::string & _text, char _char, size_t _start)
		{
			// �������
			bool kov = false;

			// ����� ��� ������
			char buff[16] = "\"_\0";
			buff[1] = _char;

			size_t pos = _start;

			while (true) {

				pos = _text.find_first_of(buff, pos);

				// ���� ��� �����, �� ����������
				if (pos == _text.npos) break;

				// ����� �������
				else if (_text[pos] == '"') {
					kov = !kov;
					pos ++;
				}
				// ���� �� � ��������, �� ���� ������
				else if (kov) pos ++;

				// �� �� � ��������
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

				int pos = _string.find("&");
				if (pos == std::string::npos) return _string;

				ret.reserve(_string.size());
				int old = 0;
				while (pos != std::string::npos) {
					ret += _string.substr(old, pos - old);

					int end = _string.find(";", pos + 1);
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
		}

	} // namespace xml

} // namespace MyGUI
