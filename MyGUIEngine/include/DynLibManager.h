/*!
	@file		DynlibManager.h
	@author		Denis Koronchik
	@date		08/2007
	@module		libEngine
*/
#ifndef _DYNLIBMANAGER_H_
#define _DYNLIBMANAGER_H_

#include "Prerequest.h"

#include "DynLib.h"

#include <string>
#include <map>

namespace MyGUI
{
	/*!	\brief Manager of dynamic libraries
	*/
	class _MyGUIExport DynLibManager
	{
	private:
		DynLibManager();

		~DynLibManager();

	public:
		//!	Get pointer to instance
		static DynLibManager* Instance();

		//!	Delete instance
		static void shutDown();

	private:
		//!	Pointer to instance
		static DynLibManager* m_instance;

	public:
		//!	Load library
		DynLib* load(const std::string &fileName);

		//!	Unload library
		void unload(DynLib *library);

		//!	Initialization
		void initialize();

	private:

		//! Dynamic libraries map
		typedef std::map <String, DynLib*> StringDynLibMap;
		
		//!	Loaded libraries
		StringDynLibMap mLibsMap;
	};

}

#endif