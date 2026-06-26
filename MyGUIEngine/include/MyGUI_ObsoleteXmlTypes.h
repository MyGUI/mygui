/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_OBSOLETE_XML_TYPES_H_
#define MYGUI_OBSOLETE_XML_TYPES_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_Diagnostic.h"

#include <pugixml.hpp>

#include <string>
#include <string_view>
#include <vector>
#include <utility>
#include <memory>

namespace MyGUI::xml
{
#if defined(__clang__)
	#define MYGUI_DISABLE_DEPRECATED_WARNINGS \
		_Pragma("clang diagnostic push") _Pragma("clang diagnostic ignored \"-Wdeprecated-declarations\"")
	#define MYGUI_RESTORE_WARNINGS _Pragma("clang diagnostic pop")
#elif defined(__GNUC__)
	#define MYGUI_DISABLE_DEPRECATED_WARNINGS \
		_Pragma("GCC diagnostic push") _Pragma("GCC diagnostic ignored \"-Wdeprecated-declarations\"")
	#define MYGUI_RESTORE_WARNINGS _Pragma("GCC diagnostic pop")
#else
	#define MYGUI_DISABLE_DEPRECATED_WARNINGS
	#define MYGUI_RESTORE_WARNINGS
#endif

#ifndef MYGUI_DONT_USE_OBSOLETE

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

	using PairAttribute = std::pair<std::string, std::string>;
	using VectorAttributes = std::vector<PairAttribute>;

	class ElementPtr;
	class Element;

	class MYGUI_EXPORT ElementEnumerator
	{
	public:
		ElementEnumerator(pugi::xml_object_range<pugi::xml_node_iterator> range);

	public:
		MYGUI_OBSOLETE("use pugixml classes and void deserialize(pugi::xml_node _node, Version _version) interface")
		bool next();
		MYGUI_OBSOLETE("use pugixml classes and void deserialize(pugi::xml_node _node, Version _version) interface")
		bool next(std::string_view _name);

		ElementPtr operator->() const;
		ElementPtr current();

	private:
		std::vector<ElementPtr> mElements;
		size_t mIndex{0};
		bool mFirst{true};
	};

	class MYGUI_EXPORT ElementPtr
	{
	public:
		ElementPtr(pugi::xml_node _node);

		operator pugi::xml_node() const
		{
			return mNode;
		}

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

		MYGUI_OBSOLETE("use pugixml classes and void deserialize(pugi::xml_node _node, Version _version) interface")
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

		MYGUI_OBSOLETE("use pugixml classes and void deserialize(pugi::xml_node _node, Version _version) interface")
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

		MYGUI_OBSOLETE("use pugixml classes and void deserialize(pugi::xml_node _node, Version _version) interface")
		ElementEnumerator getElementEnumerator();

		ElementType getType() const;

		pugi::xml_node node() const
		{
			return mNode;
		}

		std::unique_ptr<Element> createCopy();

	private:
		void cacheName() const;
		void cacheContent() const;

	private:
		pugi::xml_node mNode;
		mutable std::string mName;
		mutable std::string mContent;
		mutable bool mNameCached{false};
		mutable bool mContentCached{false};
		mutable VectorAttributes mAttributes;
		mutable bool mAttributesCached{false};
	};

	class MYGUI_EXPORT Element : public ElementPtr
	{
		friend class ElementPtr;

	public:
		MYGUI_DISABLE_DEPRECATED_WARNINGS
		Element(pugi::xml_node _node, std::shared_ptr<pugi::xml_document> _doc) :
			ElementPtr(_node),
			mDocument(std::move(_doc))
		{
		}
		MYGUI_RESTORE_WARNINGS

	private:
		std::shared_ptr<pugi::xml_document> mDocument;
	};

#endif // MYGUI_DONT_USE_OBSOLETE

} // namespace MyGUI

#endif // MYGUI_OBSOLETE_XML_TYPES_H_
