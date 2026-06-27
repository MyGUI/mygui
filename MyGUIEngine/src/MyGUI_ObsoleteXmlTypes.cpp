/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_ObsoleteXmlTypes.h"

namespace MyGUI::xml
{

#ifndef MYGUI_DONT_USE_OBSOLETE

	MYGUI_DISABLE_DEPRECATED_WARNINGS

	ElementEnumerator::ElementEnumerator(pugi::xml_object_range<pugi::xml_node_iterator> range)
	{
		for (auto child : range)
			mElements.emplace_back(child);
	}

	bool ElementEnumerator::next()
	{
		if (mFirst)
		{
			mFirst = false;
		}
		else
		{
			++mIndex;
		}
		return mIndex < mElements.size();
	}

	bool ElementEnumerator::next(std::string_view _name)
	{
		while (next())
		{
			if (mElements[mIndex].getName() == _name)
				return true;
		}
		return false;
	}

	ElementPtr ElementEnumerator::operator->() const
	{
		return mElements[mIndex];
	}

	ElementPtr ElementEnumerator::current()
	{
		return mElements[mIndex];
	}

	ElementPtr::ElementPtr(pugi::xml_node _node) :
		mNode(_node)
	{
	}

	ElementPtr* ElementPtr::operator->()
	{
		return this;
	}

	const ElementPtr* ElementPtr::operator->() const
	{
		return this;
	}

	ElementPtr ElementPtr::createChild(std::string_view _name, std::string_view _content, ElementType _type)
	{
		auto child = mNode.append_child(std::string(_name).c_str());
		if (!_content.empty())
			child.text().set(std::string(_content).c_str());
		return ElementPtr(child);
	}

	void ElementPtr::removeChild(ElementPtr _child)
	{
		mNode.remove_child(_child.mNode);
	}

	void ElementPtr::addAttribute(std::string_view _key, std::string_view _value)
	{
		mNode.append_attribute(std::string(_key).c_str()) = std::string(_value).c_str();
	}

	void ElementPtr::removeAttribute(std::string_view _key)
	{
		mNode.remove_attribute(std::string(_key).c_str());
	}

	void ElementPtr::setAttribute(std::string_view _key, std::string_view _value)
	{
		auto attr = mNode.attribute(std::string(_key).c_str());
		if (attr)
			attr.set_value(std::string(_value).c_str());
		else
			mNode.append_attribute(std::string(_key).c_str()) = std::string(_value).c_str();
	}

	void ElementPtr::addContent(std::string_view _content)
	{
		if (mNode.text().set(std::string(_content).c_str()))
		{
			mContentCached = false;
		}
	}

	void ElementPtr::setContent(std::string_view _content)
	{
		for (auto child = mNode.first_child(); child;)
		{
			auto next = child.next_sibling();
			mNode.remove_child(child);
			child = next;
		}
		mNode.text().set(std::string(_content).c_str());
		mContentCached = false;
	}

	void ElementPtr::clear()
	{
		for (auto child = mNode.first_child(); child;)
		{
			auto next = child.next_sibling();
			mNode.remove_child(child);
			child = next;
		}
		while (mNode.first_attribute())
			mNode.remove_attribute(mNode.first_attribute());
		mContentCached = false;
		mAttributesCached = false;
	}

	bool ElementPtr::findAttribute(std::string_view _name, std::string& _value)
	{
		auto attr = mNode.attribute(std::string(_name).c_str());
		if (attr)
		{
			_value = attr.as_string();
			return true;
		}
		return false;
	}

	std::string_view ElementPtr::findAttribute(std::string_view _name)
	{
		auto attr = mNode.attribute(std::string(_name).c_str());
		if (attr)
			return attr.as_string();
		return {};
	}

	const std::string& ElementPtr::getName() const
	{
		cacheName();
		return mName;
	}

	const std::string& ElementPtr::getContent() const
	{
		cacheContent();
		return mContent;
	}

	const VectorAttributes& ElementPtr::getAttributes() const
	{
		if (!mAttributesCached)
		{
			mAttributes.clear();
			for (auto attr : mNode.attributes())
				mAttributes.emplace_back(attr.name(), attr.as_string());
			mAttributesCached = true;
		}
		return mAttributes;
	}

	ElementPtr ElementPtr::getParent() const
	{
		return ElementPtr(mNode.parent());
	}

	ElementEnumerator ElementPtr::getElementEnumerator()
	{
		return ElementEnumerator(mNode.children());
	}

	ElementType ElementPtr::getType() const
	{
		switch (mNode.type())
		{
		case pugi::node_null: return ElementType::Normal;
		case pugi::node_document: return ElementType::Normal;
		case pugi::node_element: return ElementType::Normal;
		case pugi::node_pcdata: return ElementType::Normal;
		case pugi::node_cdata: return ElementType::Normal;
		case pugi::node_comment: return ElementType::Comment;
		case pugi::node_pi: return ElementType::Normal;
		case pugi::node_declaration: return ElementType::Declaration;
		case pugi::node_doctype: return ElementType::Normal;
		}
		return ElementType::Normal;
	}

	void ElementPtr::cacheName() const
	{
		if (!mNameCached)
		{
			mName = mNode.name();
			mNameCached = true;
		}
	}

	void ElementPtr::cacheContent() const
	{
		if (!mContentCached)
		{
			mContent = mNode.text().as_string();
			mContentCached = true;
		}
	}

	std::unique_ptr<Element> ElementPtr::createCopy()
	{
		auto doc = std::make_shared<pugi::xml_document>();
		auto copy = doc->append_copy(mNode);
		return std::make_unique<Element>(copy, std::move(doc));
	}

	MYGUI_RESTORE_WARNINGS

#endif // MYGUI_DONT_USE_OBSOLETE

} // namespace MyGUI
