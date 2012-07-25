/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _ec693983_b8f8_4345_8dfd_c9858fe58df9_
#define _ec693983_b8f8_4345_8dfd_c9858fe58df9_

#include "IExportSerializer.h"
#include "pugixml.hpp"
#include "Data.h"

namespace tools
{

	class ImageExportSerializer :
		public IExportSerializer
	{
	public:
		ImageExportSerializer();
		virtual ~ImageExportSerializer();

		virtual void serialization(pugi::xml_document& _doc);
		virtual bool deserialization(pugi::xml_document& _doc);

	private:
		void parseImage(pugi::xml_node _node);
		void parseGroup(pugi::xml_node _node, DataPtr _parent);
		void parseIndex(pugi::xml_node _node, DataPtr _parent);
		void parseFrame(pugi::xml_node _node, DataPtr _parent);

		void writeImage(pugi::xml_node _parent, DataPtr _data);
		void writeGroup(pugi::xml_node _parent, DataPtr _data);
		void writeIndex(pugi::xml_node _parent, DataPtr _data);
		void writeFrame(pugi::xml_node _parent, DataPtr _data);

		void updateImageProperty(DataPtr _data);
		void updateGroupProperty(DataPtr _data);
		void updateIndexProperty(DataPtr _data);

		MyGUI::IntPoint getFirstFramePoint(DataPtr _data);
	};

}

#endif
