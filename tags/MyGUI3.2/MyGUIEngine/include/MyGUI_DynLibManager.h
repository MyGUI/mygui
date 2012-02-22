/*!
	@file
	@author		Denis Koronchik
	@date		08/2007
*/
/*
	This file is part of MyGUI.

	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __MYGUI_DYN_LIB_MANAGER_H__
#define __MYGUI_DYN_LIB_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Singleton.h"
#include "MyGUI_DynLib.h"
#include <map>

namespace MyGUI
{

	/*!	\brief Manager of dynamic libraries
	*/
	class MYGUI_EXPORT DynLibManager :
		public Singleton<DynLibManager>
	{
	public:
		DynLibManager();

		void initialise();
		void shutdown();

		//!	Load library
		DynLib* load(const std::string& fileName);
		//!	Unload library
		void unload(DynLib* library);

		void unloadAll();

		/*internal:*/
		void _unloadDelayDynLibs();

	private:
		void notifyEventFrameStart(float _time);

	private:
		//! Dynamic libraries map
		typedef std::map <std::string, DynLib*> StringDynLibMap;
		//!	Loaded libraries
		StringDynLibMap mLibsMap;

		bool mIsInitialise;

		typedef std::vector<DynLib*> VectorDynLib;
		VectorDynLib mDelayDynLib;
	};

} // namespace MyGUI

#endif // __MYGUI_DYN_LIB_MANAGER_H__
