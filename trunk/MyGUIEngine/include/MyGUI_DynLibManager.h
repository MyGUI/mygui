/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
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
