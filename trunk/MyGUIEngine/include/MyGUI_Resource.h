/*!
	@file
	@author		Albert Semenov
	@date		09/2008
	@module
*/
#ifndef __MYGUI_RESOURCE_H__
#define __MYGUI_RESOURCE_H__

#include "MyGUI_Prerequest.h"

namespace MyGUI
{

	class Resource;
	typedef Resource * ResourcePtr;

	class ResourceManager;

	class _MyGUIExport Resource
	{
		// для удаления
		friend class ResourceManager;

	protected:
		Resource() { }
		virtual ~Resource() { }

	public:
		virtual const std::string & getTypeName() = 0;

		template <typename T>
		inline T * castType() { return static_cast<T*>(this); }
	};

} // namespace MyGUI

#endif // __MYGUI_RESOURCE_H__
