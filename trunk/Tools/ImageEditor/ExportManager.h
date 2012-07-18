/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _fde958f2_c1bc_4976_8cff_8a45a828b009_
#define _fde958f2_c1bc_4976_8cff_8a45a828b009_

#include <MyGUI.h>
#include "pugixml.hpp"
#include "Data.h"

namespace tools
{

	class ExportManager :
		public MyGUI::Singleton<ExportManager>
	{
	public:
		ExportManager();
		virtual ~ExportManager();

		void initialise();
		void shutdown();

		void serialization(pugi::xml_document& _doc);
		bool deserialization(pugi::xml_document& _doc);

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
