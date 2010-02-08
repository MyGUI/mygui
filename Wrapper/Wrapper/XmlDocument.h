/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __XML_DOCUMENT_H__
#define __XML_DOCUMENT_H__

#include "Enumerator.h"
#include "StringUtility.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>

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
	class ElementEnumerator
	{
		friend class Element;

	private:
		ElementEnumerator(VectorElement::iterator _begin, VectorElement::iterator _end);

	public:
		bool next();
		bool next(const std::string& _name);

		ElementPtr operator->() const { assert(m_current != m_end); return (*m_current); }
		ElementPtr current() { assert(m_current != m_end); return (*m_current); }

	private:
		bool m_first;
		VectorElement::iterator m_current, m_end;
	};


	//----------------------------------------------------------------------//
	// class Element
	//----------------------------------------------------------------------//
	class Element
	{
		friend class  Document;

	public:
		~Element();

	private:
		Element(const std::string &_name, ElementPtr _parent, ElementType _type = ElementType::Normal, const std::string& _content = "");
		void save(std::ofstream & _stream, size_t _level);

	public:
		ElementPtr createChild(const std::string& _name, const std::string& _content = "");

		template <typename T>
		void addAttribute(const std::string &_key, const T& _value)
		{
			mAttributes.push_back(PairAttribute(_key, utility::toString(_value)));
		}

		void addAttribute(const std::string& _key, const std::string& _value)
		{
			mAttributes.push_back(PairAttribute(_key, _value));
		}

		void removeAttribute(const std::string& _key)
		{
			for (size_t index=0; index<mAttributes.size(); ++index) {
				if (mAttributes[index].first == _key) {
					mAttributes.erase(mAttributes.begin() + index);
					return;
				}
			}
		}

		void setAttribute(const std::string& _key, const std::string& _value)
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

		void addContent(const std::string& _content)
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

		void setContent(const std::string& _content)
		{
			mContent = _content;
		}

		void setContent2(const std::string& _content)
		{
			mContent2 = _content;
		}

		void clear();

		bool findAttribute(const std::string& _name, std::string & _value);
		std::string findAttribute(const std::string& _name);

		const std::string& getName() const { return mName; }
		const std::string& getContent() const { return mContent; }
		const std::string& getContent2() const { return mContent2; }
		const VectorAttributes & getAttributes() const { return mAttributes; }
		ElementPtr getParent() const { return mParent; }

		ElementEnumerator getElementEnumerator() { return ElementEnumerator(mChilds.begin(), mChilds.end()); }

		ElementType getType() const { return mType; }

	private:
		std::string mName;
		std::string mContent;
		std::string mContent2;
		VectorAttributes mAttributes;
		VectorElement mChilds;
		ElementPtr mParent;
		ElementType mType;
	};

	//----------------------------------------------------------------------//
	// class Document
	//----------------------------------------------------------------------//
	class Document
	{
	public:
		Document();
		~Document();

		// открывает обычным файлом, имя файла в utf8
		bool open(const std::string& _filename);

		// открывает обычным файлом, имя файла в utf16 или utf32
		bool open(const std::wstring& _filename);

		// открывает обычным потоком
		bool open(std::ifstream & _stream);

		// сохраняет файл, имя файла в кодировке utf8
		bool save(const std::string& _filename);

		// сохраняет файл, имя файла в кодировке utf16 или utf32
		bool save(const std::wstring& _filename);

		bool save(std::ofstream & _stream);


		// если группа есть, то открывается огровским потоком, если нет, то просто как файл
		bool open(const std::string& _filename, const std::string& _group);

		void clear();

		std::string getLastError()
		{
			const std::string& error = mLastError.print();
			if (error.empty()) return error;
			return utility::toString("'", error, "' ,  file='", mLastErrorFile, "' ,  line=", mLine, " ,  col=", mCol);
		}

		void clearLastError() { mLastError = ErrorType::MAX; }

	private:

		void setLastFileError(const std::string& _filename) { mLastErrorFile = _filename; }

		bool parseTag(ElementPtr &_currentNode, std::string _content);

		bool checkPair(std::string &_key, std::string &_value);

		bool parseLine(std::string & _line, ElementPtr & _element);

		// ищет символ без учета ковычек
		size_t find(const std::string& _text, char _char, size_t _start = 0);

		void clearDeclaration();
		void clearRoot();

	public:
		ElementPtr createDeclaration(const std::string& _version = "1.0", const std::string& _encoding = "UTF-8");
		ElementPtr createRoot(const std::string& _name);

		ElementPtr getRoot() const { return mRoot; }

	private:
		ElementPtr mRoot;
		ElementPtr mDeclaration;
		ErrorType mLastError;
		std::string mLastErrorFile;
		size_t mLine;
		size_t mCol;

	}; // class Document

} // namespace xml

#endif // __XML_DOCUMENT_H__
