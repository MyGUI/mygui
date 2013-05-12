/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _299713c3_291b_4d03_904d_4864ed846024_
#define _299713c3_291b_4d03_904d_4864ed846024_

#include "IExportSerializer.h"
#include "pugixml.hpp"
#include "Data.h"

namespace tools
{

	class SkinExportSerializer :
		public IExportSerializer
	{
	public:
		SkinExportSerializer();
		virtual ~SkinExportSerializer();

		virtual void serialization(pugi::xml_document& _doc);
		virtual bool deserialization(pugi::xml_document& _doc);

		void writeSkin(pugi::xml_node _parent, DataPtr _data);

	private:
		void parseSkin(pugi::xml_node _node);

		void writeRegion(pugi::xml_node _parent, DataPtr _parentData, DataPtr _data, bool _text);
		pugi::xml_node writeState(pugi::xml_node _parent, DataPtr _data, const MyGUI::IntCoord& _value);
		void writeStateText(pugi::xml_node _parent, DataPtr _data, const MyGUI::IntCoord& _value);

		void fillStateData(DataPtr _data, pugi::xml_node _node);
		void fillSeparatorData(DataPtr _data, pugi::xml_node _node);
		void fillRegionData(DataPtr _data, pugi::xml_node _node);

		std::string convertEditorToExportStateName(const std::string& _value);
		std::string convertExportToEditorStateName(const std::string& _value);
		void registerMapName(const std::string& _value1, const std::string& _value2);

		DataPtr getChildData(DataPtr _data, const std::string& _dataType, const std::string& _name);

		std::string convertEditorToExportAlign(const std::string& _value);

		void sortByAlign(Data::VectorData& childs);
		size_t findIndex(Data::VectorData& childs, const std::string& _name);
		void moveToEnd(Data::VectorData& childs, size_t _index);

	private:
		MyGUI::MapString mEditorToExportNames;
		MyGUI::MapString mExportToEditorNames;
	};

}

#endif
