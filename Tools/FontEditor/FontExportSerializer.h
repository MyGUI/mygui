/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _97897e95_12da_4017_9946_9422c79f905f_
#define _97897e95_12da_4017_9946_9422c79f905f_

#include "IExportSerializer.h"
#include "pugixml.hpp"
#include "Data.h"

namespace tools
{

	class FontExportSerializer :
		public IExportSerializer
	{
	public:
		FontExportSerializer();
		virtual ~FontExportSerializer();

		virtual void serialization(pugi::xml_document& _doc);
		virtual bool deserialization(pugi::xml_document& _doc);

		static bool exportData(const MyGUI::UString& _folderName, const MyGUI::UString& _fileName);
		static void generateFont(DataPtr _font);

	private:
		void parseFont(pugi::xml_node _node);
		void writeFont(pugi::xml_node _parent, DataPtr _data);

		static void generateFontManualXml(MyGUI::xml::ElementPtr _root, const MyGUI::UString& _folderName, DataPtr _data);
		static void removeFont(const std::string& _fontName);
		//static void generateFontTTFXml(MyGUI::xml::ElementPtr _root, const std::string& _fontName, DataPtr _data);
	};

}

#endif
