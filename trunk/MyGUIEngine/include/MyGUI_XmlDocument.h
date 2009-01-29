/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_XML_DOCUMENT_H__
#define __MYGUI_XML_DOCUMENT_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Utility.h"
#include "MyGUI_Convert.h"
#include "MyGUI_Common.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>

#include <OgreDataStream.h>
#include <OgreUTFString.h>

#include "MyGUI_LastHeader.h"

namespace MyGUI
{
	namespace xml
	{

		struct ElementType
		{
			enum Enum {
				Comment,
				Declaration,
				Normal,
				MAX };
			ElementType(Enum _value = MAX) : value(_value) { }
			friend bool operator == (ElementType const & a, ElementType const & b) { return a.value == b.value; }
			friend bool operator != (ElementType const & a, ElementType const & b) { return a.value != b.value; }
		private:
			Enum value;
		};

		struct ErrorType
		{
			enum Enum {
				OpenFileFail,
				CreateFileFail,
				IncorrectContent,
				NotClosedElements,
				NoXMLDeclaration,
				CloseNotOpenedElement,
				InconsistentOpenCloseElements,
				MoreThanOneXMLDeclaration,
				MoreThanOneRootElement,
				IncorrectAttribute,
				MAX };
			ErrorType(Enum _value = MAX) : value(_value) { }

			std::string print() const { return getValueName(value); }

		private:
			const char * getValueName(int _index) const
			{
				static const char * values[MAX + 1] = {
					"Failed to open XML file",
					"Failed to ceate XML file",
					"XML file contain incorrect content",
					"XML file contain not closed elements",
					"XML file without declaration",
					"XML file contain closed but not opened element",
					"XML file contain inconsistent elements",
					"XML file contain more than one declaration",
					"XML file contain more than one root element",
					"XML file contain incorrect attribute",
					"" };
				return values[(_index < MAX && _index >= 0) ? _index : MAX];
			}
		private:
			Enum value;
		};

		class Element;
		class Document;

		typedef Element * ElementPtr;
		typedef std::pair<std::string, std::string> PairAttribute;
		typedef std::vector<PairAttribute> VectorAttributes;
		typedef std::vector<ElementPtr> VectorElement;

		//----------------------------------------------------------------------//
		// class ElementEnumerator
		//----------------------------------------------------------------------//
		class MYGUI_EXPORT ElementEnumerator
		{
			friend class Element;

		private:
			ElementEnumerator(VectorElement::iterator _begin, VectorElement::iterator _end);

		public:
			bool next();
			bool next(const std::string & _name);

			ElementPtr operator->() const { assert(m_current != m_end); return (*m_current); }
			ElementPtr current() { assert(m_current != m_end); return (*m_current); }

			MYGUI_OBSOLETE("use : bool ElementEnumerator::next()")
			bool nextNode() { return next(); }
			MYGUI_OBSOLETE("use : bool ElementEnumerator::next(const std::string & _name)")
			bool nextNode(const std::string & _name) { return next(_name); }
			MYGUI_OBSOLETE("use : ElementPtr ElementEnumerator::current()")
			ElementPtr currentNode() { return current(); }

		private:
			bool m_first;
			VectorElement::iterator m_current, m_end;
		};


		//----------------------------------------------------------------------//
		// class Element
		//----------------------------------------------------------------------//
		class MYGUI_EXPORT Element
		{
			friend class  Document;

		public:
			~Element();

		private:
			Element(const std::string &_name, ElementPtr _parent, ElementType _type = ElementType::Normal, const std::string & _content = "");
			void save(std::ofstream & _stream, size_t _level);

		public:
			ElementPtr createChild(const std::string & _name, const std::string & _content = "");

			template <typename T>
			MYGUI_OBSOLETE("use : template <typename T> void Element::addAttribute(const std::string &_key, const T& _value)")
			void addAttributes(const std::string &_key, const T& _value) { addAttribute<T>(_key, _value); }
			MYGUI_OBSOLETE("use : void Element::addAttribute(const std::string & _key, const std::string & _value)")
			void addAttributes(const std::string & _key, const std::string & _value) { addAttribute(_key, _value); }

			template <typename T>
			void addAttribute(const std::string &_key, const T& _value)
			{
				mAttributes.push_back(PairAttribute(_key, utility::toString(_value)));
			}

			void addAttribute(const std::string & _key, const std::string & _value)
			{
				mAttributes.push_back(PairAttribute(_key, _value));
			}

			void removeAttribute(const std::string & _key)
			{
				for (size_t index=0; index<mAttributes.size(); ++index) {
					if (mAttributes[index].first == _key) {
						mAttributes.erase(mAttributes.begin() + index);
						return;
					}
				}
			}

			void setAttribute(const std::string & _key, const std::string & _value)
			{
				for (size_t index=0; index<mAttributes.size(); ++index) {
					if (mAttributes[index].first == _key) {
						mAttributes[index].second = _value;
						return;
					}
				}
				mAttributes.push_back(PairAttribute(_key, _value));
			}

			template <typename T>
			void addContent(const T& _content)
			{
				mContent.empty() ? mContent = utility::toString(_content) : mContent += utility::toString(" ", _content);
			}

			void addContent(const std::string & _content)
			{
				if (mContent.empty()) mContent = _content;
				else {
					mContent += " ";
					mContent += _content;
				}
			}

			template <typename T>
			void setContent(const T& _content)
			{
				mContent = utility::toString(_content);
			}

			void setContent(const std::string & _content)
			{
				mContent = _content;
			}

			template <typename T>
			MYGUI_OBSOLETE("use : template <typename T> void Element::addContent(const T& _content)")
			void addBody(const T& _content) { addContent<T>(_content); }
			MYGUI_OBSOLETE("use : void Element::addContent(const std::string & _content)")
			void addBody(const std::string & _content) { addContent(_content); }
			template <typename T>
			MYGUI_OBSOLETE("use : template <typename T> void Element::setContent(const T& _content)")
			void setBody(const T& _content) { setContent<T>(_content); }
			MYGUI_OBSOLETE("use : void Element::setContent(const std::string & _content)")
			void setBody(const std::string & _content) { setContent(_content); }

			void clear();

			bool findAttribute(const std::string & _name, std::string & _value);
			std::string findAttribute(const std::string & _name);

			const std::string & getName() { return mName; }
			const std::string & getContent() { return mContent; }
			const VectorAttributes & getAttributes() { return mAttributes; }
			ElementPtr getParent() { return mParent; }

			ElementEnumerator getElementEnumerator() { return ElementEnumerator(mChilds.begin(), mChilds.end()); }

			ElementType getType() { return mType; }

			MYGUI_OBSOLETE("use : const std::string & Element::getContent()")
			const std::string & getBody() { return getContent(); }
			MYGUI_OBSOLETE("use : ElementEnumerator Element::getElementEnumerator()")
			ElementEnumerator getNodeIterator() { return getElementEnumerator(); }

		private:
			std::string mName;
			std::string mContent;
			VectorAttributes mAttributes;
			VectorElement mChilds;
			ElementPtr mParent;
			ElementType mType;
		};

		//----------------------------------------------------------------------//
		// class Document
		//----------------------------------------------------------------------//
		class MYGUI_EXPORT Document
		{
		public:
			Document();
			~Document();

			// открывает обычным файлом, имя файла в utf8
			bool open(const std::string & _filename);

			// открывает обычным файлом, имя файла в utf16 или utf32
			bool open(const std::wstring & _filename);

			// открывает обычным потоком
			bool open(std::ifstream & _stream);

			// сохраняет файл, имя файла в кодировке utf8
			bool save(const std::string & _filename);

			// сохраняет файл, имя файла в кодировке utf16 или utf32
			bool save(const std::wstring & _filename);

			bool save(std::ofstream & _stream);


			// если группа есть, то открывается огровским потоком, если нет, то просто как файл
			bool open(const std::string & _filename, const std::string & _group);

			// открывает по потоку огра
			bool open(const Ogre::DataStreamPtr& _stream);

			bool save(const Ogre::UTFString & _filename)
			{
				return save(_filename.asWStr());
			}

			bool open(const Ogre::UTFString & _filename)
			{
				return open(_filename.asWStr());
			}

			void clear();

			std::string getLastError()
			{
				const std::string& error = mLastError.print();
				if (error.empty()) return error;
				return MyGUI::utility::toString("'", error, "' ,  file='", mLastErrorFile, "' ,  line=", mLine, " ,  col=", mCol);
			}

			void clearLastError() { mLastError = ErrorType::MAX; }

		private:

			void setLastFileError(const std::string & _filename) { mLastErrorFile = _filename; }

			void setLastFileError(const std::wstring & _filename) { mLastErrorFile = Ogre::UTFString(_filename).asUTF8(); }

			bool parseTag(ElementPtr &_currentNode, std::string _content);

			bool checkPair(std::string &_key, std::string &_value);

			bool parseLine(std::string & _line, ElementPtr & _element);

			// ищет символ без учета ковычек
			size_t find(const std::string & _text, char _char, size_t _start = 0);

			void clearDeclaration();
			void clearRoot();

		public:
			ElementPtr createDeclaration(const std::string & _version = "1.0", const std::string & _encoding = "UTF-8");
			ElementPtr createRoot(const std::string & _name);

			ElementPtr getRoot() { return mRoot; }

			MYGUI_OBSOLETE("use : ElementPtr Document::createDeclaration(const std::string & _version, const std::string & _encoding)")
			ElementPtr createInfo(const std::string & _version = "1.0", const std::string & _encoding = "UTF-8") { return createDeclaration(_version, _encoding); }

		private:
			ElementPtr mRoot;
			ElementPtr mDeclaration;
			ErrorType mLastError;
			std::string mLastErrorFile;
			size_t mLine;
			size_t mCol;

		}; // class Document

		MYGUI_OBSOLETE("use : class MyGUI::xml::ElementEnumerator")
		typedef ElementEnumerator xmlNodeIterator;
		MYGUI_OBSOLETE("use : class MyGUI::xml::ElementPtr")
		typedef ElementPtr xmlNodePtr;
		MYGUI_OBSOLETE("use : class MyGUI::xml::Document")
		typedef Document xmlDocument;

	} // namespace xml

} // namespace MyGUI

#endif // __MYGUI_XML_DOCUMENT_H__
