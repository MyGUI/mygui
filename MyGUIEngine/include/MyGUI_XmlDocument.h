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
#include <cassert>
#include <type_traits>
#include <memory>

namespace MyGUI::xml
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

		ElementType(Enum _value = MAX) :
			mValue(_value)
		{
		}
		friend bool operator==(ElementType const& a, ElementType const& b)
		{
			return a.mValue == b.mValue;
		}
		friend bool operator!=(ElementType const& a, ElementType const& b)
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

		ErrorType(Enum _value = MAX) :
			mValue(_value)
		{
		}

		std::string_view print() const
		{
			return getValueName(mValue);
		}

	private:
		std::string_view getValueName(int _index) const
		{
			if (_index < 0 || _index >= MAX)
				return {};
			static const std::string_view values[MAX] = {
				"Failed to open XML file",
				"Failed to create XML file",
				"XML file contain incorrect content",
				"XML file contain not closed elements",
				"XML file without declaration",
				"XML file contain closed but not opened element",
				"XML file contain inconsistent elements",
				"XML file contain more than one declaration",
				"XML file contain more than one root element",
				"XML file contain incorrect attribute"};
			return values[_index];
		}

	private:
		Enum mValue;
	};

	class Element;
	class Document;

	using ElementPtr = Element*;
	using PairAttribute = std::pair<std::string, std::string>;
	using VectorAttributes = std::vector<PairAttribute>;
	using VectorElement = std::vector<std::unique_ptr<Element>>;

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
		bool next(std::string_view _name);

		ElementPtr operator->() const;
		ElementPtr current();

		/*obsolete:*/
#ifndef MYGUI_DONT_USE_OBSOLETE

		MYGUI_OBSOLETE("use : bool ElementEnumerator::next()")
		bool nextNode()
		{
			return next();
		}
		MYGUI_OBSOLETE("use : bool ElementEnumerator::next(std::string_view _name)")
		bool nextNode(std::string_view _name)
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
		bool m_first{true};
		VectorElement::iterator m_current, m_end;
	};


	//----------------------------------------------------------------------//
	// class Element
	//----------------------------------------------------------------------//
	class MYGUI_EXPORT Element
	{
		friend class Document;

	public:
		Element(
			std::string_view _name,
			ElementPtr _parent,
			ElementType _type = ElementType::Normal,
			std::string_view _content = {});
		Element(Element&&) = default;

	private:
		void save(std::ostream& _stream, size_t _level);

	public:
		ElementPtr createChild(
			std::string_view _name,
			std::string_view _content = {},
			ElementType _type = ElementType::Normal);
		void removeChild(ElementPtr _child);

		template<typename T, typename = std::enable_if_t<!std::is_convertible_v<T, std::string_view>>>
		void addAttribute(std::string_view _key, const T& _value)
		{
			addAttribute(_key, MyGUI::utility::toString(_value));
		}

		void addAttribute(std::string_view _key, std::string_view _value);

		void removeAttribute(std::string_view _key);

		void setAttribute(std::string_view _key, std::string_view _value);

		template<typename T, typename = std::enable_if_t<!std::is_convertible_v<T, std::string_view>>>
		void addContent(const T& _content)
		{
			addContent(MyGUI::utility::toString(_content));
		}

		void addContent(std::string_view _content);

		template<typename T, typename = std::enable_if_t<!std::is_convertible_v<T, std::string_view>>>
		void setContent(const T& _content)
		{
			setContent(MyGUI::utility::toString(_content));
		}

		void setContent(std::string_view _content);

		void clear();

		bool findAttribute(std::string_view _name, std::string& _value);
		std::string_view findAttribute(std::string_view _name);

		const std::string& getName() const;

		const std::string& getContent() const;

		const VectorAttributes& getAttributes() const;

		ElementPtr getParent() const;

		ElementEnumerator getElementEnumerator();

		ElementType getType() const;

		std::unique_ptr<Element> createCopy();

		/*obsolete:*/
#ifndef MYGUI_DONT_USE_OBSOLETE

		template<typename T>
		MYGUI_OBSOLETE(
			"use : template <typename T> void Element::addAttribute(const std::string &_key, const T& _value)")
		void addAttributes(std::string_view _key, const T& _value)
		{
			addAttribute<T>(_key, _value);
		}
		MYGUI_OBSOLETE("use : void Element::addAttribute(std::string_view _key, std::string_view _value)")
		void addAttributes(std::string_view _key, std::string_view _value)
		{
			addAttribute(_key, _value);
		}

		template<typename T>
		MYGUI_OBSOLETE("use : template <typename T> void Element::addContent(const T& _content)")
		void addBody(const T& _content)
		{
			addContent<T>(_content);
		}
		MYGUI_OBSOLETE("use : void Element::addContent(std::string_view _content)")
		void addBody(std::string_view _content)
		{
			addContent(_content);
		}
		template<typename T>
		MYGUI_OBSOLETE("use : template <typename T> void Element::setContent(const T& _content)")
		void setBody(const T& _content)
		{
			setContent<T>(_content);
		}
		MYGUI_OBSOLETE("use : void Element::setContent(std::string_view _content)")
		void setBody(std::string_view _content)
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
		VectorElement mChildren;
		ElementPtr mParent;
		ElementType mType;
	};

	//----------------------------------------------------------------------//
	// class Document
	//----------------------------------------------------------------------//
	class MYGUI_EXPORT Document
	{
	public:
		// открывает обычным файлом, имя файла в utf8
		bool open(const std::string& _filename);

		// открывает обычным файлом, имя файла в utf16 или utf32
		bool open(const std::wstring& _filename);

		// открывает обычным потоком
		bool open(std::istream& _stream);

		bool open(const UString& _filename);

		bool open(IDataStream* _stream);

		// сохраняет файл
		bool save(const std::string& _filename);

		// сохраняет файл
		bool save(const std::wstring& _filename);

		bool save(std::ostream& _stream);

		bool save(const UString& _filename);

		void clear();

		std::string getLastError() const;

		void clearLastError();

		ElementPtr createDeclaration(std::string_view _version = "1.0", std::string_view _encoding = "UTF-8");
		ElementPtr createRoot(std::string_view _name);

		ElementPtr getRoot() const;

		/*obsolete:*/
#ifndef MYGUI_DONT_USE_OBSOLETE

		MYGUI_OBSOLETE(
			"use : ElementPtr Document::createDeclaration(std::string_view _version, std::string_view _encoding)")
		ElementPtr createInfo(std::string_view _version = "1.0", std::string_view _encoding = "UTF-8")
		{
			return createDeclaration(_version, _encoding);
		}

#endif // MYGUI_DONT_USE_OBSOLETE

	private:
		void setLastFileError(std::string_view _filename);
		void setLastFileError(const std::wstring& _filename);

		bool parseTag(ElementPtr& _currentNode, std::string _content);

		bool checkPair(std::string& _key, std::string& _value);

		bool parseLine(std::string& _line, ElementPtr& _element);

		// ищет символ без учета ковычек
		size_t find(std::string_view _text, char _char, size_t _start = 0);

		void clearDeclaration();
		void clearRoot();

	private:
		std::unique_ptr<Element> mRoot;
		std::unique_ptr<Element> mDeclaration;
		ErrorType mLastError;
		std::string mLastErrorFile;
		size_t mLine{0};
		size_t mCol{0};

	}; // class Document

	using xmlNodeIterator MYGUI_OBSOLETE("use : class MyGUI::xml::ElementEnumerator") = ElementEnumerator;
	using xmlNodePtr MYGUI_OBSOLETE("use : class MyGUI::xml::ElementPtr") = ElementPtr;
	using xmlDocument MYGUI_OBSOLETE("use : class MyGUI::xml::Document") = Document;

} // namespace MyGUI

#endif // MYGUI_XML_DOCUMENT_H_
