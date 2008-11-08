/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_XML_DOCUMENT_H__
#define __MYGUI_XML_DOCUMENT_H__

#include "MyGUI_Prerequest.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>

#include <OgreDataStream.h>

#include "MyGUI_LastHeader.h"

namespace MyGUI
{
	namespace xml
	{

		namespace utility
		{
			template< typename T >
			inline std::string toString (T p)
			{
				std::ostringstream stream;
				stream << p;
				return stream.str();
			}

			template< typename T1, typename T2 >
			inline std::string toString (T1 p1, T2 p2)
			{
				std::ostringstream stream;
				stream << p1 << p2;
				return stream.str();
			}

			inline void trim(std::string& _str, bool _left = true, bool _right = true)
			{
				if (_right) _str.erase(_str.find_last_not_of(" \t\r")+1);
				if (_left) _str.erase(0, _str.find_first_not_of(" \t\r"));
			}

			std::string convert_to_xml(const std::string & _string);
			std::string convert_from_xml(const std::string & _string, bool & _ok);

		}

		enum xmlNodeType {
			XML_NODE_TYPE_REMARK, // коментарий
			XML_NODE_TYPE_INFO, // информационный блок
			XML_NODE_TYPE_NORMAL, // обыкновенный блок
		};

		namespace errors
		{

			enum ErrorTypes {
				XML_ERROR_NONE,
				XML_ERROR_OPEN_FILE,
				XML_ERROR_CREATE_FILE,
				XML_ERROR_BODY_NON_CORRECT,
				XML_ERROR_NON_CLOSE_ALL_TAGS,
				XML_ERROR_DOCUMENT_IS_EMPTY,
				XML_ERROR_CLOSE_TAG_NOT_FOUND_START_TAG,
				XML_ERROR_OPEN_CLOSE_NOT_EQVIVALENT,
				XML_ERROR_INFO_IS_EXIST,
				XML_ERROR_ROOT_IS_EXIST,
				XML_ERROR_ATTRIBUTE_NON_CORRECT,
				XML_ERROR_COUNT
			};

		} // namespace errors

		class xmlNode;
		class xmlDocument;

		typedef xmlNode * xmlNodePtr;
		typedef std::pair<std::string, std::string> PairAttributes;
		typedef std::vector<PairAttributes> VectorAttributes;
		typedef std::vector<xmlNodePtr> VectorNode;

		//----------------------------------------------------------------------//
		// class xmlNodeIterator
		//----------------------------------------------------------------------//
		class _MyGUIExport xmlNodeIterator
		{
			friend class xmlNode;

		private:
			xmlNodeIterator(VectorNode::iterator _start, VectorNode::iterator _end);

		public:
			bool nextNode();
			bool nextNode(const std::string & _name);

			xmlNodePtr operator->() const { assert(m_current != m_end); return (*m_current); }
			xmlNodePtr currentNode() { assert(m_current != m_end); return (*m_current); }

		private:
			bool m_first;
			VectorNode::iterator m_current, m_end;
		};


		//----------------------------------------------------------------------//
		// class xmlNode
		//----------------------------------------------------------------------//
		class _MyGUIExport xmlNode
		{
			friend class  xmlDocument;

		public:
			~xmlNode();

		private:
			xmlNode(const std::string &_name, xmlNodePtr _parent, xmlNodeType _type = XML_NODE_TYPE_NORMAL, const std::string & _body = "");
			void save(std::ofstream & _stream, size_t _level);

		public:
			xmlNodePtr createChild(const std::string & _name, const std::string & _body = "");

			template <typename T>
			void addAttributes(const std::string &_key, const T& _value)
			{
				mAttributes.push_back(PairAttributes(_key, utility::toString(_value)));
			}

			void addAttributes(const std::string & _key, const std::string & _value)
			{
				mAttributes.push_back(PairAttributes(_key, _value));
			}

			template <typename T>
			void addBody(const T& _body)
			{
				mBody.empty() ? mBody = utility::toString(_body) : mBody += utility::toString(" ", _body);
			}

			void addBody(const std::string & _body)
			{
				if (mBody.empty()) mBody = _body;
				else {
					mBody += " ";
					mBody += _body;
				}
			}

			template <typename T>
			void setBody(const T& _body)
			{
				mBody = utility::toString(_body);
			}

			void setBody(const std::string & _body)
			{
				mBody = _body;
			}

			void clear();

			bool findAttribute(const std::string & _name, std::string & _value);
			std::string findAttribute(const std::string & _name);

			const std::string & getName() { return mName; }
			const std::string & getBody() { return mBody; }
			const VectorAttributes & getAttributes() { return mAttributes; }
			xmlNodePtr getParent() { return mParent; }

			xmlNodeIterator getNodeIterator() { return xmlNodeIterator(mChilds.begin(), mChilds.end()); }

		private:
			std::string mName;
			std::string mBody;
			VectorAttributes mAttributes;
			VectorNode mChilds;
			xmlNodePtr mParent;
			xmlNodeType mType;
		};

		//----------------------------------------------------------------------//
		// class xmlDocument
		//----------------------------------------------------------------------//
		class _MyGUIExport xmlDocument
		{
		public:
			xmlDocument();
			~xmlDocument();

			// если группа есть, то открывается огровским потоком, если нет, то просто как файл
			bool open(const std::string & _name, const std::string & _group);

			// открывает обычным файлом
			bool open(const std::string & _name);

			// открывает по потоку огра
			bool open(const Ogre::DataStreamPtr& stream);

			// сохраняет файл
			bool save(const std::string & _name);

			void clear();
			const std::string getLastError();

		private:

			bool parseTag(xmlNodePtr &_currentNode, std::string _body);

			bool checkPair(std::string &_key, std::string &_value);

			// ищет символ без учета ковычек
			size_t find(const std::string & _text, char _char, size_t _start = 0);

			void clearInfo();
			void clearRoot();

		public:
			xmlNodePtr createInfo(const std::string & _version = "1.0", const std::string & _encoding = "UTF-8");
			xmlNodePtr createRoot(const std::string & _name);

			xmlNodePtr getRoot() {return mRoot;}

		private:
			xmlNodePtr mRoot;
			xmlNodePtr mInfo;
			xml::errors::ErrorTypes mLastError;
			std::string mLastErrorFile;
			size_t mLine;
			size_t mCol;

		}; // class xmlDocument : public xmlNode

	} // namespace xml

} // namespace MyGUI

#endif // __MYGUI_XML_DOCUMENT_H__
