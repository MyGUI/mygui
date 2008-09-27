/*!
	@file
	@author		Albert Semenov
	@date		09/2008
	@module
*/
#ifndef __MYGUI_RESOURCE_H__
#define __MYGUI_RESOURCE_H__

#include "MyGUI_Prerequest.h"
//#include "MyGUI_Guid.h"

namespace MyGUI
{

	class Resource;
	typedef Resource * ResourcePtr;

	class _MyGUIExport Resource
	{
	public:
		virtual ~Resource() { }
		virtual const std::string & getType() = 0;

		template <typename T>
		inline T * castType() { return static_cast<T*>(this); }
	};

} // namespace MyGUI

#endif // __MYGUI_RESOURCE_H__
