/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_XML_DOCUMENT_H_
#define MYGUI_XML_DOCUMENT_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_UString.h"
#include "MyGUI_Diagnostic.h"
#include "MyGUI_DataStream.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>

namespace MyGUI
{
	namespace xml
	{

		struct ElementType
		{
			enum Enum
			{
				Comment,
				Declaration,
				Normal,
				MAX
			};

			ElementType(Enum _value = MAX) : mValue(_value) { }
			friend bool operator == (ElementType const& a, ElementType const& b)
			{
				return a.mValue == b.mValue;
			}
			friend bool operator != (ElementType const& a, ElementType const& b)
			{
				return a.mValue != b.mValue;
			}

			int getValue() const
			{
				return mValue;
			}

		private:
			Enum mValue;
		};

		struct ErrorType
		{
			enum Enum
			{
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
				MAX
			};

			ErrorType(Enum _value = MAX) : mValue(_value) { }

			std::string print() const
			{
				return getValueName(mValue);
			}

		private:
			const char* getValueName(int _index) const
			{
				static const char* values[MAX + 1] =
				{
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
					""
				};
				return values[(_index < MAX && _index >= 0) ? _index : MAX];
			}
		private:
			Enum mValue;
		};

		class Element;
		class Document;

		typedef Element* ElementPtr;
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
			bool next(const std::string& _name);

			ElementPtr operator->() const;
			ElementPtr current();

			/*obsolete:*/
#ifndef MYGUI_DONT_USE_OBSOLETE

			MYGUI_OBSOLETE("use : bool ElementEnumerator::next()")
			bool nextNode()
			{
				return next();
			}
			MYGUI_OBSOLETE("use : bool ElementEnumerator::next(const std::string& _name)")
			bool nextNode(const std::string& _name)
			{
				return next(_name);
			}
			MYGUI_OBSOLETE("use : ElementPtr ElementEnumerator::current()")
			ElementPtr currentNode()
			{
				return current();
			}

#endif // MYGUI_DONT_USE_OBSOLETE

		private:
			bool m_first;
			VectorElement::iterator m_current, m_end;
		};


		//----------------------------------------------------------------------//
		// class Element
		//----------------------------------------------------------------------//
		class MYGUI_EXPORT Element
		{
			friend class Document;

		public:
			~Element();

		private:
			Element(const std::string& _name, ElementPtr _parent, ElementType _type = ElementType::Normal, const std::string& _content = "");
			void save(std::ostream& _stream, size_t _level);

		public:
			ElementPtr createChild(const std::string& _name, const std::string& _content = "", ElementType _type = ElementType::Normal);
			void removeChild(ElementPtr _child);

			template <typename T>
			void addAttribute(const std::string& _key, const T& _value)
			{
				addAttribute(_key, utility::toString(_value));
			}

			void addAttribute(const std::string& _key, const std::string& _value);

			void removeAttribute(const std::string& _key);

			void setAttribute(const std::string& _key, const std::string& _value);

			template <typename T>
			void addContent(const T& _content)
			{
				addContent(utility::toString(_content));
			}

			void addContent(const std::string& _content);

			template <typename T>
			void setContent(const T& _content)
			{
				setContent(utility::toString(_content));
			}

			void setContent(const std::string& _content);

			void clear();

			bool findAttribute(const std::string& _name, std::string& _value);
			std::string findAttribute(const std::string& _name);

			const std::string& getName() const;

			const std::string& getContent() const;

			const VectorAttributes& getAttributes() const;

			ElementPtr getParent() const;

			ElementEnumerator getElementEnumerator();

			ElementType getType() const;

			ElementPtr createCopy();

			/*obsolete:*/
#ifndef MYGUI_DONT_USE_OBSOLETE

			template <typename T>
			MYGUI_OBSOLETE("use : template <typename T> void Element::addAttribute(const std::string &_key, const T& _value)")
			void addAttributes(const std::string& _key, const T& _value)
			{
				addAttribute<T>(_key, _value);
			}
			MYGUI_OBSOLETE("use : void Element::addAttribute(const std::string& _key, const std::string& _value)")
			void addAttributes(const std::string& _key, const std::string& _value)
			{
				addAttribute(_key, _value);
			}

			template <typename T>
			MYGUI_OBSOLETE("use : template <typename T> void Element::addContent(const T& _content)")
			void addBody(const T& _content)
			{
				addContent<T>(_content);
			}
			MYGUI_OBSOLETE("use : void Element::addContent(const std::string& _content)")
			void addBody(const std::string& _content)
			{
				addContent(_content);
			}
			template <typename T>
			MYGUI_OBSOLETE("use : template <typename T> void Element::setContent(const T& _content)")
			void setBody(const T& _content)
			{
				setContent<T>(_content);
			}
			MYGUI_OBSOLETE("use : void Element::setContent(const std::string& _content)")
			void setBody(const std::string& _content)
			{
				setContent(_content);
			}

			MYGUI_OBSOLETE("use : const std::string& Element::getContent()")
			const std::string& getBody() const
			{
				return getContent();
			}
			MYGUI_OBSOLETE("use : ElementEnumerator Element::getElementEnumerator()")
			ElementEnumerator getNodeIterator()
			{
				return getElementEnumerator();
			}

#endif // MYGUI_DONT_USE_OBSOLETE

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
			bool open(const std::string& _filename);

			// открывает обычным файлом, имя файла в utf16 или utf32
			bool open(const std::wstring& _filename);

			// открывает обычным потоком
			bool open(std::istream& _stream);

			bool open(const UString& _filename);

			bool open(IDataStream* _data);

			// сохраняет файл
			bool save(const std::string& _filename);

			// сохраняет файл
			bool save(const std::wstring& _filename);

			bool save(std::ostream& _stream);

			bool save(const UString& _filename);

			void clear();

			std::string getLastError();

			void clearLastError();

			ElementPtr createDeclaration(const std::string& _version = "1.0", const std::string& _encoding = "UTF-8");
			ElementPtr createRoot(const std::string& _name);

			ElementPtr getRoot() const;

			/*obsolete:*/
#ifndef MYGUI_DONT_USE_OBSOLETE

			MYGUI_OBSOLETE("use : ElementPtr Document::createDeclaration(const std::string& _version, const std::string& _encoding)")
			ElementPtr createInfo(const std::string& _version = "1.0", const std::string& _encoding = "UTF-8")
			{
				return createDeclaration(_version, _encoding);
			}

#endif // MYGUI_DONT_USE_OBSOLETE

		private:
			void setLastFileError(const std::string& _filename);
			void setLastFileError(const std::wstring& _filename);

			bool parseTag(ElementPtr& _currentNode, std::string _content);

			bool checkPair(std::string& _key, std::string& _value);

			bool parseLine(std::string& _line, ElementPtr& _element);

			// ищет символ без учета ковычек
			size_t find(const std::string& _text, char _char, size_t _start = 0);

			void clearDeclaration();
			void clearRoot();

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

#endif // MYGUI_XML_DOCUMENT_H_
