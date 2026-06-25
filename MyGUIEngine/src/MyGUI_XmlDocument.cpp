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

	namespace
	{

		// Map pugixml parse status to MyGUI ErrorType
		ErrorType mapPugiError(pugi::xml_parse_status _status)
		{
			switch (_status)
			{
				case pugi::status_file_not_found:
					return ErrorType::OpenFileFail;
				case pugi::status_end_element_mismatch:
					return ErrorType::InconsistentOpenCloseElements;
				case pugi::status_bad_attribute:
					return ErrorType::IncorrectAttribute;
				default:
					return ErrorType::IncorrectContent;
			}
		}

		// Recursively build MyGUI Element tree from pugixml node tree
		void buildElementTreeChildren(const pugi::xml_node& _pugiNode, Element* _myguiParent)
		{
			for (const auto& child : _pugiNode)
			{
				if (child.type() == pugi::node_element)
				{
					Element* newChild = _myguiParent->createChild(child.name());
					for (const auto& attr : child.attributes())
						newChild->addAttribute(attr.name(), attr.value());
					buildElementTreeChildren(child, newChild);
				}
				else if (
					(child.type() == pugi::node_pcdata || child.type() == pugi::node_cdata) &&
					_myguiParent->getContent().empty())
				{
					std::string_view text = child.value();
					// skip whitespace-only text (likely indentation between elements)
					if (text.find_first_not_of(" \t\r\n") != std::string::npos)
						_myguiParent->setContent(text);
				}
			}
		}

		// Build MyGUI Element tree from a pugi::xml_document
		bool convertPugiToMyGUI(
			const pugi::xml_document& _doc,
			std::unique_ptr<Element>& _root,
			std::unique_ptr<Element>& _declaration)
		{
			for (const auto& node : _doc)
			{
				if (node.type() == pugi::node_declaration)
				{
					_declaration = std::make_unique<Element>("xml", nullptr, ElementType::Declaration);
					for (const auto& attr : node.attributes())
						_declaration->addAttribute(attr.name(), attr.value());
				}
				else if (node.type() == pugi::node_element)
				{
					if (_root)
					{
						// multiple root elements — should not happen with valid XML
						return false;
					}
					_root = std::make_unique<Element>(node.name(), nullptr, ElementType::Normal);
					for (const auto& attr : node.attributes())
						_root->addAttribute(attr.name(), attr.value());
					buildElementTreeChildren(node, _root.get());
				}
			}
			return true;
		}

		// Recursively build pugixml tree from MyGUI Element tree
		void elementToPugiNode(Element* _myguiElem, pugi::xml_node& _pugiNode)
		{
			// set attributes
			for (const auto& attr : _myguiElem->getAttributes())
				_pugiNode.append_attribute(attr.first.c_str()) = attr.second.c_str();

			// set text content
			if (!_myguiElem->getContent().empty())
				_pugiNode.text().set(_myguiElem->getContent().c_str());

			// recurse children
			ElementEnumerator children = _myguiElem->getElementEnumerator();
			while (children.next())
			{
				ElementPtr child = children.current();
				if (child->getType() == ElementType::Comment)
				{
					pugi::xml_node commentNode = _pugiNode.append_child(pugi::node_comment);
					commentNode.set_value(child->getContent().c_str());
					continue;
				}
				pugi::xml_node childNode = _pugiNode.append_child(child->getName().c_str());
				elementToPugiNode(child, childNode);
			}
		}

		// Convert MyGUI Element tree to pugi::xml_document and save
		bool saveToPugiStream(
			Element* _declaration,
			Element* _root,
			std::ostream& _stream)
		{
			pugi::xml_document doc;

			// add declaration
			if (_declaration)
			{
				pugi::xml_node decl = doc.prepend_child(pugi::node_declaration);
				decl.set_name("xml");
				for (const auto& attr : _declaration->getAttributes())
					decl.append_attribute(attr.first.c_str()) = attr.second.c_str();
			}

			// add root element and its children
			if (_root)
			{
				pugi::xml_node rootNode = doc.append_child(_root->getName().c_str());
				elementToPugiNode(_root, rootNode);
			}

			doc.save(_stream, "    ", pugi::format_write_bom | pugi::format_default);
			return true;
		}

		// compute line (1-indexed) from offset by counting newlines
		size_t computeLine(const char* _data, ptrdiff_t _offset)
		{
			if (_offset <= 0)
				return 1;
			size_t line = 1;
			for (ptrdiff_t i = 0; i < _offset; ++i)
			{
				if (_data[i] == '\n')
					++line;
			}
			return line;
		}

		// compute column (1-indexed) from offset by scanning back to last newline
		size_t computeColumn(const char* _data, ptrdiff_t _offset)
		{
			if (_offset <= 0)
				return 0;
			const char* pos = _data + _offset;
			const char* start = pos;
			while (start > _data && *(start - 1) != '\n')
				--start;
			return static_cast<size_t>(pos - start + 1);
		}

		// compute line and column from offset
		void computeLineCol(const char* _data, ptrdiff_t _offset, size_t& _line, size_t& _col)
		{
			_line = computeLine(_data, _offset);
			_col = computeColumn(_data, _offset);
		}

	} // anonymous namespace

	//----------------------------------------------------------------------//
	// class ElementEnumerator
	//----------------------------------------------------------------------//
	ElementEnumerator::ElementEnumerator(VectorElement::iterator _begin, VectorElement::iterator _end) :
		m_current(_begin),
		m_end(_end)
	{
	}

	bool ElementEnumerator::next()
	{
		if (m_current == m_end)
			return false;
		if (m_first)
		{
			m_first = false;
			return true;
		}
		++m_current;
		return m_current != m_end;
	}

	bool ElementEnumerator::next(std::string_view _name)
	{
		while (next())
		{
			if ((*m_current)->getName() == _name)
				return true;
		}
		return false;
	}

	ElementPtr ElementEnumerator::operator->() const
	{
		assert(m_current != m_end);
		return m_current->get();
	}

	ElementPtr ElementEnumerator::current()
	{
		assert(m_current != m_end);
		return m_current->get();
	}

	//----------------------------------------------------------------------//
	// class Element
	//----------------------------------------------------------------------//
	Element::Element(std::string_view _name, ElementPtr _parent, ElementType _type, std::string_view _content) :
		mName(_name),
		mContent(_content),
		mParent(_parent),
		mType(_type)
	{
	}

	ElementPtr Element::createChild(std::string_view _name, std::string_view _content, ElementType _type)
	{
		return mChildren.emplace_back(std::make_unique<Element>(_name, this, _type, _content)).get();
	}

	void Element::removeChild(ElementPtr _child)
	{
		for (auto iter = mChildren.begin(); iter != mChildren.end(); ++iter)
		{
			if (iter->get() == _child)
			{
				mChildren.erase(iter);
				break;
			}
		}
	}

	void Element::clear()
	{
		mChildren.clear();
		mContent.clear();
		mAttributes.clear();
	}

	bool Element::findAttribute(std::string_view _name, std::string& _value)
	{
		for (const auto& attribute : mAttributes)
		{
			if (attribute.first == _name)
			{
				_value = attribute.second;
				return true;
			}
		}
		return false;
	}

	std::string_view Element::findAttribute(std::string_view _name)
	{
		for (const auto& attribute : mAttributes)
		{
			if (attribute.first == _name)
				return attribute.second;
		}
		return {};
	}

	void Element::addAttribute(std::string_view _key, std::string_view _value)
	{
		mAttributes.emplace_back(_key, _value);
	}

	void Element::removeAttribute(std::string_view _key)
	{
		for (size_t index = 0; index < mAttributes.size(); ++index)
		{
			if (mAttributes[index].first == _key)
			{
				mAttributes.erase(mAttributes.begin() + index);
				return;
			}
		}
	}

	std::unique_ptr<Element> Element::createCopy()
	{
		auto elem = std::make_unique<Element>(mName, nullptr, mType, mContent);
		elem->mAttributes = mAttributes;

		for (const auto& oldChild : mChildren)
		{
			auto child = oldChild->createCopy();
			child->mParent = elem.get();
			elem->mChildren.emplace_back(std::move(child));
		}

		return elem;
	}

	void Element::setAttribute(std::string_view _key, std::string_view _value)
	{
		for (auto& attribute : mAttributes)
		{
			if (attribute.first == _key)
			{
				attribute.second = _value;
				return;
			}
		}
		mAttributes.emplace_back(_key, _value);
	}

	void Element::addContent(std::string_view _content)
	{
		if (mContent.empty())
		{
			mContent = _content;
		}
		else
		{
			mContent += " ";
			mContent += _content;
		}
	}

	void Element::setContent(std::string_view _content)
	{
		mContent = _content;
	}

	const std::string& Element::getName() const
	{
		return mName;
	}

	const std::string& Element::getContent() const
	{
		return mContent;
	}

	const VectorAttributes& Element::getAttributes() const
	{
		return mAttributes;
	}

	ElementPtr Element::getParent() const
	{
		return mParent;
	}

	ElementEnumerator Element::getElementEnumerator()
	{
		return {mChildren.begin(), mChildren.end()};
	}

	ElementType Element::getType() const
	{
		return mType;
	}

	//----------------------------------------------------------------------//
	// class Document
	//----------------------------------------------------------------------//

	bool Document::open(const std::string& _filename)
	{
		clear();

		// read file into buffer for line/col computation
		std::ifstream file(_filename.c_str(), std::ios::binary | std::ios::ate);
		if (!file.is_open())
		{
			mLastError = ErrorType::OpenFileFail;
			mLastErrorFile = _filename;
			return false;
		}
		std::streamsize size = file.tellg();
		file.seekg(0, std::ios::beg);
		std::string data(static_cast<size_t>(size > 0 ? size : 0), '\0');
		if (size > 0)
			file.read(data.data(), size);
		file.close();

		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_buffer(
			data.data(),
			data.size(),
			pugi::parse_default | pugi::parse_declaration | pugi::parse_comments);

		if (!result)
		{
			mLastError = mapPugiError(result.status);
			mLastErrorFile = _filename;
			computeLineCol(data.data(), result.offset, mLine, mCol);
			return false;
		}

		if (!convertPugiToMyGUI(doc, mRoot, mDeclaration))
		{
			mLastError = ErrorType::MoreThanOneRootElement;
			mLastErrorFile = _filename;
			return false;
		}

		return true;
	}

	bool Document::open(const std::wstring& _filename)
	{
		return open(UString(_filename).asUTF8());
	}

	bool Document::open(std::istream& _stream)
	{
		clear();

		// read all data into buffer for line/col computation
		std::string data;
		char buf[4096];
		while (_stream)
		{
			_stream.read(buf, sizeof(buf));
			std::streamsize readSize = _stream.gcount();
			if (readSize > 0)
				data.append(buf, static_cast<size_t>(readSize));
		}

		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_buffer(
			data.data(),
			data.size(),
			pugi::parse_default | pugi::parse_declaration | pugi::parse_comments);

		if (!result)
		{
			mLastError = mapPugiError(result.status);
			computeLineCol(data.data(), result.offset, mLine, mCol);
			return false;
		}

		if (!convertPugiToMyGUI(doc, mRoot, mDeclaration))
		{
			mLastError = ErrorType::MoreThanOneRootElement;
			return false;
		}

		return true;
	}

	bool Document::open(IDataStream* _stream)
	{
		clear();

		// read all data from stream
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

		pugi::xml_document doc;
		pugi::xml_parse_result result = doc.load_buffer(
			data.data(),
			data.size(),
			pugi::parse_default | pugi::parse_declaration | pugi::parse_comments);

		if (!result)
		{
			mLastError = mapPugiError(result.status);
			computeLineCol(data.data(), result.offset, mLine, mCol);
			return false;
		}

		if (!convertPugiToMyGUI(doc, mRoot, mDeclaration))
		{
			mLastError = ErrorType::MoreThanOneRootElement;
			return false;
		}

		return true;
	}

	bool Document::save(const std::string& _filename)
	{
		if (!mDeclaration)
		{
			mLastError = ErrorType::NoXMLDeclaration;
			mLastErrorFile = _filename;
			return false;
		}

		std::ofstream stream;
		stream.open(_filename.c_str());

		if (!stream.is_open())
		{
			mLastError = ErrorType::CreateFileFail;
			mLastErrorFile = _filename;
			return false;
		}

		bool result = save(stream);

		stream.close();
		return result;
	}

	bool Document::save(const std::wstring& _filename)
	{
		return save(UString(_filename).asUTF8());
	}

	bool Document::save(std::ostream& _stream)
	{
		if (!mDeclaration)
		{
			mLastError = ErrorType::NoXMLDeclaration;
			return false;
		}

		if (!saveToPugiStream(mDeclaration.get(), mRoot.get(), _stream))
		{
			mLastError = ErrorType::CreateFileFail;
			return false;
		}

		return true;
	}

	void Document::clear()
	{
		clearDeclaration();
		clearRoot();
		mLine = 0;
		mCol = 0;
	}

	bool Document::open(const UString& _filename)
	{
		return open(_filename.asWStr());
	}

	bool Document::save(const UString& _filename)
	{
		return save(_filename.asWStr());
	}

	void Document::clearLastError()
	{
		mLastError = ErrorType::MAX;
	}

	ElementPtr Document::getRoot() const
	{
		return mRoot.get();
	}

	void Document::clearDeclaration()
	{
		mDeclaration.reset();
	}

	void Document::clearRoot()
	{
		mRoot.reset();
	}

	ElementPtr Document::createDeclaration(std::string_view _version, std::string_view _encoding)
	{
		clearDeclaration();
		mDeclaration = std::make_unique<Element>("xml", nullptr, ElementType::Declaration);
		mDeclaration->addAttribute("version", _version);
		mDeclaration->addAttribute("encoding", _encoding);
		return mDeclaration.get();
	}

	ElementPtr Document::createRoot(std::string_view _name)
	{
		clearRoot();
		mRoot = std::make_unique<Element>(_name, nullptr, ElementType::Normal);
		return mRoot.get();
	}

	std::string Document::getLastError() const
	{
		std::string_view error = mLastError.print();
		if (error.empty())
			return {};
		return MyGUI::utility::toString(
			"'",
			error,
			"' ,  file='",
			mLastErrorFile,
			"' ,  line=",
			mLine,
			" ,  col=",
			mCol);
	}

} // namespace MyGUI
