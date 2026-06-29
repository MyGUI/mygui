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

#include <string>
#include <string_view>
#include <iostream>
#include <vector>
#include <utility>
#include <memory>

namespace pugi
{
	class xml_node;
	class xml_document;
	struct xml_parse_result;
}

namespace MyGUI::xml
{
	using PairAttribute = std::pair<std::string, std::string>;
	using VectorAttributes = std::vector<PairAttribute>;

	class ElementPtr;
	class Element;

	class MYGUI_EXPORT Document
	{
	public:
		Document();

		// Open as regular file, filename in UTF-8
		bool open(const std::string& _filename);

		// Open as regular file, filename in UTF-16 or UTF-32
		bool open(const std::wstring& _filename);

		// Open as regular stream
		bool open(std::istream& _stream);

		bool open(const UString& _filename);

		bool open(IDataStream* _stream);

		bool save(const std::string& _filename);

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
		ElementPtr createInfo(std::string_view _version = "1.0", std::string_view _encoding = "UTF-8");

#endif // MYGUI_DONT_USE_OBSOLETE

	private:
		std::shared_ptr<pugi::xml_document> mDoc;
		std::shared_ptr<pugi::xml_parse_result> mResult;
		std::string mLastError;
		std::string mLastErrorFile;
	};

	class MYGUI_EXPORT ElementEnumerator
	{
	public:
		ElementEnumerator(std::shared_ptr<pugi::xml_node> parent);

	public:
		bool next();
		bool next(std::string_view _name);

		ElementPtr operator->() const;
		ElementPtr current();

	private:
		std::vector<ElementPtr> mElements;
		size_t mIndex{size_t(-1)};
	};

	class MYGUI_EXPORT ElementPtr
	{
	public:
		ElementPtr() = default;
		ElementPtr(std::nullptr_t)
		{
		}
		ElementPtr(const pugi::xml_node& _node);

		operator pugi::xml_node() const;

		ElementPtr* operator->();
		const ElementPtr* operator->() const;

		friend bool operator==(const ElementPtr& lhs, std::nullptr_t)
		{
			return lhs.mNode == nullptr;
		}
		friend bool operator==(std::nullptr_t, const ElementPtr& rhs)
		{
			return nullptr == rhs.mNode;
		}
		friend bool operator!=(const ElementPtr& lhs, std::nullptr_t)
		{
			return lhs.mNode != nullptr;
		}
		friend bool operator!=(std::nullptr_t, const ElementPtr& rhs)
		{
			return nullptr != rhs.mNode;
		}

		ElementPtr& operator=(std::nullptr_t)
		{
			mNode.reset();
			return *this;
		}

		ElementPtr createChild(std::string_view _name, std::string_view _content = {});
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
		void setContent(const T& _content)
		{
			setContent(MyGUI::utility::toString(_content));
		}

		void setContent(std::string_view _content);

		void clear();

		bool findAttribute(std::string_view _name, std::string& _value);
		std::string_view findAttribute(std::string_view _name);

		std::string getName() const;
		std::string getContent() const;

		VectorAttributes getAttributes() const;

		ElementPtr getParent() const;

		ElementEnumerator getElementEnumerator();

		const pugi::xml_node& node() const;

		std::unique_ptr<Element> createCopy();

	private:
		std::shared_ptr<pugi::xml_node> mNode;
	};

	class MYGUI_EXPORT Element : public ElementPtr
	{
		friend class ElementPtr;

	public:
		Element(std::shared_ptr<pugi::xml_node> _node, std::shared_ptr<pugi::xml_document> _doc);

	private:
		std::shared_ptr<pugi::xml_document> mDocument;
	};

} // namespace MyGUI

#endif // MYGUI_XML_DOCUMENT_H_
