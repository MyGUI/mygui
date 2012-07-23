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

	private:
		void parseSkin(pugi::xml_node _node);
		void writeSkin(pugi::xml_node _parent, Data* _data);

		void fillStateData(Data* _data, pugi::xml_node _node);
		void fillSeparatorData(Data* _data, pugi::xml_node _node);

		std::string convertEditorToExportStateName(const std::string& _value);
		std::string convertExportToEditorStateName(const std::string& _value);
		void registerMapName(const std::string& _value1, const std::string& _value2);

		Data* getChildData(Data* _data, const std::string& _dataType, const std::string& _name);

	private:
		typedef std::map<std::string, std::string> MapString;
		MapString mEditorToExportNames;
		MapString mExportToEditorNames;
	};

}

#endif
