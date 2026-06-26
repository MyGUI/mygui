/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_DataManager.h"

#include <fstream>

namespace MyGUI::xml
{

	bool Document::open(const std::string& _filename)
	{
		clear();

		mResult =
			mDoc.load_file(_filename.c_str(), pugi::parse_default | pugi::parse_declaration | pugi::parse_comments);

		if (!mResult)
			mLastErrorFile = _filename;

		return mResult;
	}

	bool Document::open(const std::wstring& _filename)
	{
		return open(UString(_filename).asUTF8());
	}

	bool Document::open(std::istream& _stream)
	{
		clear();

		mResult = mDoc.load(_stream, pugi::parse_default | pugi::parse_declaration | pugi::parse_comments);

		if (!mResult)
			mLastErrorFile = "<stream>";

		return mResult;
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

		mResult = mDoc.load_buffer(
			data.data(),
			data.size(),
			pugi::parse_default | pugi::parse_declaration | pugi::parse_comments);

		return mResult;
	}

	bool Document::save(const std::string& _filename)
	{
		bool result = mDoc.save_file(_filename.c_str(), "    ", pugi::format_write_bom | pugi::format_default);

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
		mDoc.save(_stream, "    ", pugi::format_write_bom | pugi::format_default);
		return true;
	}

	void Document::clear()
	{
		mDoc.reset();
		mResult = {};
		mLastError.clear();
		mLastErrorFile.clear();
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
		mResult = {};
		mLastError.clear();
		mLastErrorFile.clear();
	}

	pugi::xml_node Document::getRoot() const
	{
		for (auto child : mDoc)
		{
			if (child.type() == pugi::node_element)
				return child;
		}
		return {};
	}

	pugi::xml_node Document::createDeclaration(std::string_view _version, std::string_view _encoding)
	{
		// remove existing declaration
		for (auto child = mDoc.first_child(); child;)
		{
			auto next = child.next_sibling();
			if (child.type() == pugi::node_declaration)
				mDoc.remove_child(child);
			child = next;
		}

		auto decl = mDoc.prepend_child(pugi::node_declaration);
		decl.set_name("xml");
		decl.append_attribute("version") = _version.data();
		decl.append_attribute("encoding") = _encoding.data();
		return decl;
	}

	pugi::xml_node Document::createRoot(std::string_view _name)
	{
		// remove existing root elements
		for (auto child = mDoc.first_child(); child;)
		{
			auto next = child.next_sibling();
			if (child.type() == pugi::node_element)
				mDoc.remove_child(child);
			child = next;
		}

		return mDoc.append_child(_name.data());
	}

	std::string Document::getLastError() const
	{
		if (!mResult)
		{
			size_t line = 0, col = 0;
			std::string lineText;
			if (mResult.offset >= 0 && !mLastErrorFile.empty())
			{
				std::ifstream file(mLastErrorFile, std::ios::binary);
				if (file)
				{
					line = 1;
					std::string data((std::istreambuf_iterator<char>(file)), {});
					for (ptrdiff_t i = 0; i < mResult.offset && i < (ptrdiff_t)data.size(); ++i)
					{
						if (data[i] == '\n')
							++line;
					}
					ptrdiff_t start = mResult.offset;
					while (start > 0 && data[start - 1] != '\n')
						--start;
					col = mResult.offset - start + 1;
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
				toString("'", mResult.description(), "', ", mLastErrorFile, "(", line, ",", col, "): ", lineText);
		}
		if (!mLastError.empty())
		{
			return utility::toString("'", mLastError, "', file='", mLastErrorFile, "'");
		}
		return {};
	}

} // namespace MyGUI
