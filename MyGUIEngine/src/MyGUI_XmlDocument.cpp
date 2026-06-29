/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_DataManager.h"

#include <pugixml.hpp>
#include <fstream>

namespace MyGUI::xml
{

	Document::Document() :
		mDoc(std::make_shared<pugi::xml_document>()),
		mResult(std::make_shared<pugi::xml_parse_result>())
	{
	}

	bool Document::open(const std::string& _filename)
	{
		clear();

		*mResult =
			mDoc->load_file(_filename.c_str(), pugi::parse_default | pugi::parse_declaration | pugi::parse_comments);

		if (!*mResult)
			mLastErrorFile = _filename;

		return *mResult;
	}

	bool Document::open(const std::wstring& _filename)
	{
		return open(UString(_filename).asUTF8());
	}

	bool Document::open(std::istream& _stream)
	{
		clear();

		*mResult = mDoc->load(_stream, pugi::parse_default | pugi::parse_declaration | pugi::parse_comments);

		if (!*mResult)
			mLastErrorFile = "<stream>";

		return *mResult;
	}

	bool Document::open(IDataStream* _stream)
	{
		clear();

		std::string data;
		size_t streamSize = _stream->size();
		if (streamSize > 0)
		{
			data.resize(streamSize);
			streamSize = _stream->read(data.data(), streamSize);
			data.resize(streamSize);
		}
		else
		{
			char buf[4096];
			while (!_stream->eof())
			{
				size_t readSize = _stream->read(buf, sizeof(buf));
				if (readSize == 0)
					break;
				data.append(buf, readSize);
			}
		}

		*mResult = mDoc->load_buffer(
			data.data(),
			data.size(),
			pugi::parse_default | pugi::parse_declaration | pugi::parse_comments);

		return *mResult;
	}

	bool Document::save(const std::string& _filename)
	{
		bool result = mDoc->save_file(_filename.c_str(), "    ", pugi::format_write_bom | pugi::format_default);

		if (!result)
		{
			mLastError = "Failed to save XML file";
			mLastErrorFile = _filename;
		}
		return result;
	}

	bool Document::save(const std::wstring& _filename)
	{
		return save(UString(_filename).asUTF8());
	}

	bool Document::save(std::ostream& _stream)
	{
		mDoc->save(_stream, "    ", pugi::format_write_bom | pugi::format_default);
		return true;
	}

	void Document::clear()
	{
		mDoc->reset();
		*mResult = {};
		mLastError.clear();
		mLastErrorFile.clear();
	}

	bool Document::open(const UString& _filename)
	{
		return open(_filename.asUTF8());
	}

	bool Document::save(const UString& _filename)
	{
		return save(_filename.asUTF8());
	}

	void Document::clearLastError()
	{
		*mResult = {};
		mLastError.clear();
		mLastErrorFile.clear();
	}

	ElementPtr Document::getRoot() const
	{
		return mDoc->document_element();
	}

	ElementPtr Document::createDeclaration(std::string_view _version, std::string_view _encoding)
	{
		// remove existing declaration
		for (auto child = mDoc->first_child(); child;)
		{
			auto next = child.next_sibling();
			if (child.type() == pugi::node_declaration)
				mDoc->remove_child(child);
			child = next;
		}

		auto decl = mDoc->prepend_child(pugi::node_declaration);
		decl.set_name("xml");
		decl.append_attribute("version") = _version;
		decl.append_attribute("encoding") = _encoding;
		return decl;
	}

	ElementPtr Document::createRoot(std::string_view _name)
	{
		// remove existing root elements
		for (auto child = mDoc->first_child(); child;)
		{
			auto next = child.next_sibling();
			if (child.type() == pugi::node_element)
				mDoc->remove_child(child);
			child = next;
		}

		return mDoc->append_child(_name);
	}

	std::string Document::getLastError() const
	{
		if (!*mResult)
		{
			size_t line = 0, col = 0;
			std::string lineText;
			if (mResult->offset >= 0 && !mLastErrorFile.empty())
			{
				std::ifstream file(mLastErrorFile, std::ios::binary);
				if (file)
				{
					line = 1;
					std::string data((std::istreambuf_iterator<char>(file)), {});
					for (ptrdiff_t i = 0; i < mResult->offset && i < (ptrdiff_t)data.size(); ++i)
					{
						if (data[i] == '\n')
							++line;
					}
					ptrdiff_t start = mResult->offset;
					while (start > 0 && data[start - 1] != '\n')
						--start;
					col = mResult->offset - start + 1;
					ptrdiff_t end = data.size();
					for (ptrdiff_t i = start; i < (ptrdiff_t)data.size(); ++i)
					{
						if (data[i] == '\n')
						{
							end = i;
							break;
						}
					}
					lineText = data.substr(start, end - start);
				}
			}
			return utility::
				toString("'", mResult->description(), "', ", mLastErrorFile, "(", line, ",", col, "): ", lineText);
		}
		if (!mLastError.empty())
		{
			return utility::toString("'", mLastError, "', file='", mLastErrorFile, "'");
		}
		return {};
	}

	ElementEnumerator::ElementEnumerator(std::shared_ptr<pugi::xml_node> parent)
	{
		for (auto child : parent->children())
			mElements.emplace_back(child);
	}

	bool ElementEnumerator::next()
	{
		return ++mIndex < mElements.size();
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

	ElementPtr::ElementPtr(const pugi::xml_node& _node) :
		mNode(std::make_shared<pugi::xml_node>(_node))
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

	ElementPtr ElementPtr::createChild(std::string_view _name, std::string_view _content)
	{
		auto child = mNode->append_child(_name);
		if (!_content.empty())
			child.text().set(_content);
		return ElementPtr(child);
	}

	void ElementPtr::removeChild(ElementPtr _child)
	{
		mNode->remove_child(*_child.mNode);
	}

	void ElementPtr::addAttribute(std::string_view _key, std::string_view _value)
	{
		mNode->append_attribute(_key) = _value;
	}

	void ElementPtr::removeAttribute(std::string_view _key)
	{
		mNode->remove_attribute(_key);
	}

	void ElementPtr::setAttribute(std::string_view _key, std::string_view _value)
	{
		auto attr = mNode->attribute(_key);
		if (attr)
			attr.set_value(_value);
		else
			mNode->append_attribute(_key) = _value;
	}

	void ElementPtr::setContent(std::string_view _content)
	{
		mNode->remove_children();
		mNode->text().set(_content);
	}

	void ElementPtr::clear()
	{
		mNode->remove_children();
		mNode->remove_attributes();
	}

	bool ElementPtr::findAttribute(std::string_view _name, std::string& _value)
	{
		auto attr = mNode->attribute(_name);
		if (attr)
		{
			_value = attr.as_string();
			return true;
		}
		return false;
	}

	std::string_view ElementPtr::findAttribute(std::string_view _name)
	{
		auto attr = mNode->attribute(_name);
		if (attr)
			return attr.as_string();
		return {};
	}

	std::string ElementPtr::getName() const
	{
		return mNode->name();
	}

	std::string ElementPtr::getContent() const
	{
		return mNode->text().as_string();
	}

	VectorAttributes ElementPtr::getAttributes() const
	{
		VectorAttributes attrs;
		for (auto attr : mNode->attributes())
			attrs.emplace_back(attr.name(), attr.as_string());
		return attrs;
	}

	ElementPtr ElementPtr::getParent() const
	{
		return ElementPtr(mNode->parent());
	}

	ElementEnumerator ElementPtr::getElementEnumerator()
	{
		return ElementEnumerator(mNode);
	}

	ElementPtr::operator pugi::xml_node() const
	{
		return *mNode;
	}

	const pugi::xml_node& ElementPtr::node() const
	{
		return *mNode;
	}

	Element::Element(std::shared_ptr<pugi::xml_node> _node, std::shared_ptr<pugi::xml_document> _doc) :
		ElementPtr(*_node),
		mDocument(std::move(_doc))
	{
	}

	std::unique_ptr<Element> ElementPtr::createCopy()
	{
		auto doc = std::make_shared<pugi::xml_document>();
		auto copy = doc->append_copy(*mNode);
		auto nodeCopy = std::make_shared<pugi::xml_node>(copy);
		return std::make_unique<Element>(nodeCopy, doc);
	}

} // namespace MyGUI
