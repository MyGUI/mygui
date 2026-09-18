/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _97897e95_12da_4017_9946_9422c79f905f_
#define _97897e95_12da_4017_9946_9422c79f905f_

#include "IExportSerializer.h"
#include <pugixml.hpp>
#include "Data.h"
#include <filesystem>

namespace tools
{

	class FontExportSerializer : public IExportSerializer
	{
	public:
		void serialization(pugi::xml_document& _doc) override;
		bool deserialization(pugi::xml_document& _doc) override;

		static bool exportData(const std::filesystem::path& _folderName, const MyGUI::UString& _fileName);
		static void generateFont(DataPtr _data, std::string_view _fontName);

	private:
		void parseFont(pugi::xml_node _node);
		void writeFont(pugi::xml_node _parent, DataPtr _data);

		static void generateFontManualXml(
			MyGUI::xml::ElementPtr _root,
			const std::filesystem::path& _folderName,
			DataPtr _data,
			std::string_view _fontName);
		static void removeFont(std::string_view _fontName);
		//static void generateFontTTFXml(MyGUI::xml::ElementPtr _root, std::string_view _fontName, DataPtr _data);
	};

}

#endif
