/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_DataManager.h"

namespace MyGUI::xml
{

	namespace utility
	{
		static std::string convert_from_xml(std::string_view _string, bool& _ok)
		{
			std::string ret;
			_ok = true;

			size_t pos = _string.find('&');
			if (pos == std::string::npos)
				return std::string{_string};

			ret.reserve(_string.size());
			size_t old = 0;
			while (pos != std::string::npos)
			{
				ret += _string.substr(old, pos - old);

				size_t end = _string.find(';', pos + 1);
				if (end == std::string::npos)
				{
					_ok = false;
					return ret;
				}

				std::string_view tag = _string.substr(pos, end - pos + 1);
				if (tag == "&amp;")
					ret += '&';
				else if (tag == "&lt;")
					ret += '<';
				else if (tag == "&gt;")
					ret += '>';
				else if (tag == "&apos;")
					ret += '\'';
				else if (tag == "&quot;")
					ret += '\"';
				else
				{
					_ok = false;
					return ret;
				}

				old = end + 1;
				pos = _string.find('&', old);
			}
			ret += _string.substr(old, std::string::npos);

			return ret;
		}

		static std::string convert_to_xml(std::string_view _string)
		{
			std::string ret;

			size_t pos = _string.find_first_of("&<>'\"");
			if (pos == std::string::npos)
				return std::string{_string};

			ret.reserve(_string.size() * 2);
			size_t old = 0;
			while (pos != std::string::npos)
			{
				ret += _string.substr(old, pos - old);

				if (_string[pos] == '&')
					ret += "&amp;";
				else if (_string[pos] == '<')
					ret += "&lt;";
				else if (_string[pos] == '>')
					ret += "&gt;";
				else if (_string[pos] == '\'')
					ret += "&apos;";
				else if (_string[pos] == '\"')
					ret += "&quot;";

				old = pos + 1;
				pos = _string.find_first_of("&<>'\"", old);
			}
			ret += _string.substr(old, std::string::npos);

			return ret;
		}

	}

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

	void Element::save(std::ostream& _stream, size_t _level)
	{
		// сначала табуляции намутим
		for (size_t tab = 0; tab < _level; ++tab)
			_stream << "    ";

		// теперь заголовок тега
		if (mType == ElementType::Declaration)
			_stream << "<?";
		else if (mType == ElementType::Comment)
			_stream << "<!--";
		else
			_stream << "<";

		_stream << mName;

		for (auto& attribute : mAttributes)
		{
			_stream << " " << attribute.first << "=\"" << utility::convert_to_xml(attribute.second) << "\"";
		}

		bool empty = mChildren.empty();
		// если детей нет то закрываем
		if (empty && mContent.empty())
		{
			if (mType == ElementType::Declaration)
				_stream << "?>\n";
			else if (mType == ElementType::Comment)
				_stream << "-->\n";
			else
				_stream << "/>\n";
		}
		else
		{
			_stream << ">";
			if (!empty)
				_stream << "\n";
			// если есть тело то сначало оно
			if (!mContent.empty())
			{
				if (!empty)
				{
					for (size_t tab = 0; tab <= _level; ++tab)
						_stream << "    ";
				}
				_stream << utility::convert_to_xml(mContent);

				if (!empty)
					_stream << "\n";
			}
			// save child items
			for (const auto& child : mChildren)
			{
				child->save(_stream, _level + 1);
			}

			if (!empty)
			{
				for (size_t tab = 0; tab < _level; ++tab)
					_stream << "    ";
			}
			_stream << "</" << mName << ">\n";
		}
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

#if MYGUI_COMPILER == MYGUI_COMPILER_MSVC && !defined(STLPORT)
	inline void open_stream(std::ofstream& _stream, const std::wstring& _wide)
	{
		_stream.open(_wide.c_str());
	}
	inline void open_stream(std::ifstream& _stream, const std::wstring& _wide)
	{
		_stream.open(_wide.c_str());
	}
#else
	inline void open_stream(std::ofstream& _stream, const std::wstring& _wide)
	{
		_stream.open(UString(_wide).asUTF8_c_str());
	}
	inline void open_stream(std::ifstream& _stream, const std::wstring& _wide)
	{
		_stream.open(UString(_wide).asUTF8_c_str());
	}
#endif

	//----------------------------------------------------------------------//
	// class Document
	//----------------------------------------------------------------------//

	// открывает обычным файлом, имя файла в utf8
	bool Document::open(const std::string& _filename)
	{
		std::ifstream stream;
		stream.open(_filename.c_str());

		if (!stream.is_open())
		{
			mLastError = ErrorType::OpenFileFail;
			setLastFileError(_filename);
			return false;
		}

		bool result = open(stream);

		stream.close();
		return result;
	}

	// открывает обычным файлом, имя файла в utf16 или utf32
	bool Document::open(const std::wstring& _filename)
	{
		std::ifstream stream;
		open_stream(stream, _filename);

		if (!stream.is_open())
		{
			mLastError = ErrorType::OpenFileFail;
			setLastFileError(_filename);
			return false;
		}

		bool result = open(stream);

		stream.close();
		return result;
	}

	bool Document::open(std::istream& _stream)
	{
		auto data = std::make_unique<DataStream>(&_stream);

		bool result = open(data.get());

		return result;
	}

	// сохраняет файл, имя файла в кодировке utf8
	bool Document::save(const std::string& _filename)
	{
		std::ofstream stream;
		stream.open(_filename.c_str());

		if (!stream.is_open())
		{
			mLastError = ErrorType::CreateFileFail;
			setLastFileError(_filename);
			return false;
		}

		bool result = save(stream);

		if (!result)
		{
			setLastFileError(_filename);
		}

		stream.close();
		return result;
	}

	// сохраняет файл, имя файла в кодировке utf16 или utf32
	bool Document::save(const std::wstring& _filename)
	{
		std::ofstream stream;
		open_stream(stream, _filename);

		if (!stream.is_open())
		{
			mLastError = ErrorType::CreateFileFail;
			setLastFileError(_filename);
			return false;
		}

		bool result = save(stream);

		if (!result)
		{
			setLastFileError(_filename);
		}

		stream.close();
		return result;
	}

	// открывает обычным потоком
	bool Document::open(IDataStream* _stream)
	{
		clear();

		// это текущая строка для разбора
		std::string line;
		// это строка из файла
		std::string read;
		// текущий узел для разбора
		ElementPtr currentNode = nullptr;

		while (!_stream->eof())
		{
			// берем новую строку
			_stream->readline(read, '\n');
			if (read.empty())
				continue;
			if (read[read.size() - 1] == '\r')
				read.erase(read.size() - 1, 1);
			if (read.empty())
				continue;

			mLine++;
			mCol = 0; // потом проверить на многострочных тэгах

			// текущая строка для разбора и то что еще прочитали
			line += read;

			if (!parseLine(line, currentNode))
			{
				return false;
			}

		} // while (!stream.eof())

		if (currentNode)
		{
			mLastError = ErrorType::NotClosedElements;
			return false;
		}

		return true;
	}

	bool Document::save(std::ostream& _stream)
	{
		if (!mDeclaration)
		{
			mLastError = ErrorType::NoXMLDeclaration;
			return false;
		}

		// заголовок utf8
		_stream << (char)0xEFu;
		_stream << (char)0xBBu;
		_stream << (char)0xBFu;

		mDeclaration->save(_stream, 0);
		if (mRoot)
			mRoot->save(_stream, 0);

		return true;
	}

	void Document::clear()
	{
		clearDeclaration();
		clearRoot();
		mLine = 0;
		mCol = 0;
	}

	bool Document::parseTag(ElementPtr& _currentNode, std::string _content)
	{
		// убераем лишнее
		MyGUI::utility::trim(_content);

		if (_content.empty())
		{
			// создаем пустой тег
			if (_currentNode)
			{
				_currentNode = _currentNode->createChild(std::string_view{});
			}
			else if (!mRoot)
			{
				mRoot = std::make_unique<Element>(std::string_view{}, nullptr);
				_currentNode = mRoot.get();
			}
			return true;
		}

		char symbol = _content[0];
		bool tagDeclaration = false;

		// проверяем на коментарии
		if (symbol == '!')
		{
			if (_currentNode != nullptr)
			{
				//_currentNode->createChild(std::string_view{}, _content, ElementType::Comment);
			}
			return true;
		}
		// проверяем на информационный тег
		if (symbol == '?')
		{
			tagDeclaration = true;
			_content.erase(0, 1); // удаляем первый символ
		}

		size_t start = 0;
		// проверяем на закрытие тега
		if (symbol == '/')
		{
			if (_currentNode == nullptr)
			{
				mLastError = ErrorType::CloseNotOpenedElement;
				return false;
			}
			// обрезаем имя тэга
			start = _content.find_first_not_of(" \t", 1);
			if (start == std::string::npos)
			{
				// тег пустой
				_content.clear();
			}
			else
			{
				size_t end = _content.find_last_not_of(" \t");
				_content = _content.substr(start, end - start + 1);
			}
			// проверяем соответствие открывающего и закрывающего тегов
			if (_currentNode->getName() != _content)
			{
				mLastError = ErrorType::InconsistentOpenCloseElements;
				return false;
			}
			// а теперь снижаем текущий узел вниз
			_currentNode = _currentNode->getParent();
		}
		else
		{
			// выделяем имя до первого пробела или закрывающего тега
			std::string cut = _content;
			start = _content.find_first_of(" \t/?", 1); // << превед
			if (start != std::string::npos)
			{
				cut = _content.substr(0, start);
				_content = _content.substr(start);
			}
			else
			{
				_content.clear();
			}

			if (_currentNode)
			{
				_currentNode = _currentNode->createChild(cut);
			}
			else
			{
				if (tagDeclaration)
				{
					// информационный тег
					if (mDeclaration)
					{
						mLastError = ErrorType::MoreThanOneXMLDeclaration;
						return false;
					}
					mDeclaration = std::make_unique<Element>(cut, nullptr, ElementType::Declaration);
					_currentNode = mDeclaration.get();
				}
				else
				{
					// рутовый тег
					if (mRoot)
					{
						mLastError = ErrorType::MoreThanOneRootElement;
						return false;
					}
					mRoot = std::make_unique<Element>(cut, nullptr, ElementType::Normal);
					_currentNode = mRoot.get();
				}
			}

			// проверим на пустоту
			start = _content.find_last_not_of(" \t");
			if (start == std::string::npos)
				return true;

			// сразу отделим закрывающийся тэг
			bool close = false;
			if ((_content[start] == '/') || (_content[start] == '?'))
			{
				close = true;
				// не будем резать строку, просто поставим пробел
				_content[start] = ' ';
				// проверим на пустоту
				start = _content.find_last_not_of(" \t");
				if (start == std::string::npos)
				{
					// возвращаем все назад и уходим
					_currentNode = _currentNode->getParent();
					return true;
				}
			}

			// а вот здесь уже в цикле разбиваем на атрибуты
			while (true)
			{
				// ищем равно
				start = _content.find('=');
				if (start == std::string::npos)
				{
					mLastError = ErrorType::IncorrectAttribute;
					return false;
				}
				// ищем вторые ковычки
				size_t end = _content.find_first_of("\"\'", start + 1);
				if (end == std::string::npos)
				{
					mLastError = ErrorType::IncorrectAttribute;
					return false;
				}
				end = _content.find_first_of("\"\'", end + 1);
				if (end == std::string::npos)
				{
					mLastError = ErrorType::IncorrectAttribute;
					return false;
				}

				std::string key = _content.substr(0, start);
				std::string value = _content.substr(start + 1, end - start);

				// проверка на валидность
				if (!checkPair(key, value))
				{
					mLastError = ErrorType::IncorrectAttribute;
					return false;
				}

				// добавляем пару в узел
				_currentNode->addAttribute(key, value);

				// следующий кусок
				_content = _content.substr(end + 1);

				// в строке не осталось символов
				start = _content.find_first_not_of(" \t");
				if (start == std::string::npos)
					break;

				mCol += start;
			}

			// был закрывающий тег для текущего тега
			if (close)
			{
				// не проверяем имена, потому что это наш тэг
				_currentNode = _currentNode->getParent();
			}
		}
		return true;
	}

	bool Document::checkPair(std::string& _key, std::string& _value)
	{
		// в ключе не должно быть ковычек и пробелов
		MyGUI::utility::trim(_key);
		if (_key.empty())
			return false;
		size_t start = _key.find_first_of(" \t\"\'&");
		if (start != std::string::npos)
			return false;

		// в значении, ковычки по бокам
		MyGUI::utility::trim(_value);
		if (_value.size() < 2)
			return false;
		if (((_value[0] != '"') || (_value[_value.length() - 1] != '"')) &&
			((_value[0] != '\'') || (_value[_value.length() - 1] != '\'')))
			return false;
		bool ok = true;
		_value = utility::convert_from_xml(_value.substr(1, _value.length() - 2), ok);
		return ok;
	}

	// ищет символ без учета ковычек
	size_t Document::find(std::string_view _text, char _char, size_t _start)
	{
		// ковычки
		bool kov = false;

		// буфер для поиска
		char buff[16] = "\"_\0";
		buff[1] = _char;

		size_t pos = _start;

		while (true)
		{
			pos = _text.find_first_of(buff, pos);

			// если уже конец, то досвидания
			if (pos == std::string::npos)
			{
				break;
			}
			// нашли ковычку
			if (_text[pos] == '"')
			{
				kov = !kov;
				pos++;
			}
			// если мы в ковычках, то идем дальше
			else if (kov)
			{
				pos++;
			}
			// мы не в ковычках
			else
			{
				break;
			}
		}

		return pos;
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

	bool Document::parseLine(std::string& _line, ElementPtr& _element)
	{
		// крутимся пока в строке есть теги
		while (true)
		{
			// сначала ищем по угловым скобкам
			size_t start = find(_line, '<');
			if (start == std::string::npos)
				break;
			size_t end;

			// пытаемся вырезать многострочный коментарий
			if ((start + 3 < _line.size()) && (_line[start + 1] == '!') && (_line[start + 2] == '-') &&
				(_line[start + 3] == '-'))
			{
				end = _line.find("-->", start + 4);
				if (end == std::string::npos)
					break;
				end += 2;
			}
			else
			{
				end = find(_line, '>', start + 1);
				if (end == std::string::npos)
					break;
			}
			// проверяем на наличее тела
			size_t body = _line.find_first_not_of(" \t<");
			if (body < start)
			{
				std::string_view body_str = std::string_view{_line}.substr(0, start);
				// текущий символ
				mCol = 0;

				if (_element != nullptr)
				{
					bool ok = true;
					_element->setContent(utility::convert_from_xml(body_str, ok));
					if (!ok)
					{
						mLastError = ErrorType::IncorrectContent;
						return false;
					}
				}
			}
			// вырезаем наш тэг и парсим
			if (!parseTag(_element, _line.substr(start + 1, end - start - 1)))
			{
				return false;
			}
			// и обрезаем текущую строку разбора
			_line = _line.substr(end + 1);
		}
		return true;
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

	void Document::setLastFileError(std::string_view _filename)
	{
		mLastErrorFile = _filename;
	}

	void Document::setLastFileError(const std::wstring& _filename)
	{
		mLastErrorFile = UString(_filename).asUTF8();
	}

} // namespace MyGUI
