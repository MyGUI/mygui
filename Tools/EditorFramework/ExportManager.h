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
#include "IExportSerializer.h"

namespace tools
{

	class MYGUI_EXPORT_DLL ExportManager :
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
		IExportSerializer* mExportSerializer;
	};

}

#endif
