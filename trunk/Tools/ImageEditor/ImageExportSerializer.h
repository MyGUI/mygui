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
		void parseGroup(pugi::xml_node _node, Data* _parent);
		void parseIndex(pugi::xml_node _node, Data* _parent);
		void parseFrame(pugi::xml_node _node, Data* _parent);

		void writeImage(pugi::xml_node _parent, Data* _data);
		void writeGroup(pugi::xml_node _parent, Data* _data);
		void writeIndex(pugi::xml_node _parent, Data* _data);
		void writeFrame(pugi::xml_node _parent, Data* _data);

		void updateImageProperty(Data* _data);
		void updateGroupProperty(Data* _data);
		void updateIndexProperty(Data* _data);

		MyGUI::IntPoint getFirstFramePoint(Data* _data);
	};

}

#endif
