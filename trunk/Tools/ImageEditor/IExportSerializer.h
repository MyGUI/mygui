/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _57e7c8c1_09ed_420f_8a28_e487e0554438_
#define _57e7c8c1_09ed_420f_8a28_e487e0554438_

#include "pugixml.hpp"
#include "IFactoryItem.h"

namespace tools
{
	class MYGUI_EXPORT_DLL IExportSerializer :
		public components::IFactoryItem
	{
	public:
		IExportSerializer() { }
		virtual ~IExportSerializer() { }

		virtual void serialization(pugi::xml_document& _doc) = 0;
		virtual bool deserialization(pugi::xml_document& _doc) = 0;
	};
}

#endif
