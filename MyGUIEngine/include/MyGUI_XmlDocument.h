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

#include <pugixml.hpp>

#include <string>
#include <iostream>

#ifndef MYGUI_DONT_USE_OBSOLETE
	#include "MyGUI_ObsoleteXmlTypes.h"
#endif

namespace MyGUI::xml
{

	class MYGUI_EXPORT Document
	{
	public:
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

		pugi::xml_node getRoot() const;

		pugi::xml_node createDeclaration(std::string_view _version = "1.0", std::string_view _encoding = "UTF-8");
		pugi::xml_node createRoot(std::string_view _name);

		/*obsolete:*/
#ifndef MYGUI_DONT_USE_OBSOLETE

		MYGUI_OBSOLETE(
			"use : pugi::xml_node Document::createDeclaration(std::string_view _version, std::string_view _encoding)")
		pugi::xml_node createInfo(std::string_view _version = "1.0", std::string_view _encoding = "UTF-8")
		{
			return createDeclaration(_version, _encoding);
		}

#endif // MYGUI_DONT_USE_OBSOLETE

	private:
		pugi::xml_document mDoc;
		pugi::xml_parse_result mResult;
		std::string mLastError;
		std::string mLastErrorFile;
	};

	using xmlDocument MYGUI_OBSOLETE("use : class MyGUI::xml::Document") = Document;

} // namespace MyGUI

#endif // MYGUI_XML_DOCUMENT_H_
