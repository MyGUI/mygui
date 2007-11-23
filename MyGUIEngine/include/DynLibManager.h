/*!
	@file		DynlibManager.h
	@author		Denis Koronchik
	@date		08/2007
	@module		libEngine
*/
#ifndef __DYN_LIB_MANAGER_H__
#define __DYN_LIB_MANAGER_H__

#include "Prerequest.h"
#include "DynLib.h"

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
		static void shutdown();

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
		typedef std::map <std::string, DynLib*> StringDynLibMap;
		
		//!	Loaded libraries
		StringDynLibMap mLibsMap;
	};

}

#endif // __DYN_LIB_MANAGER_H__