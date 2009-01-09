/*!
	@file
	@author		Denis Koronchik
	@date		08/2007
	@module		libEngine
*/
#ifndef __MYGUI_DYN_LIB_MANAGER_H__
#define __MYGUI_DYN_LIB_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Instance.h"
#include "MyGUI_DynLib.h"

namespace MyGUI
{
	/*!	\brief Manager of dynamic libraries
	*/
	class MYGUI_EXPORT DynLibManager
	{
		MYGUI_INSTANCE_HEADER(DynLibManager);

	public:
		void initialise();
		void shutdown();

		//!	Load library
		DynLib* load(const std::string &fileName);
		//!	Unload library
		void unload(DynLib *library);
	private:
		//! Dynamic libraries map
		typedef std::map <std::string, DynLib*> StringDynLibMap;
		//!	Loaded libraries
		StringDynLibMap mLibsMap;
	};

}

#endif // __MYGUI_DYN_LIB_MANAGER_H__
