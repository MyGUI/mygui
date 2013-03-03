/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef __MYGUI_SINGLETON_H__
#define __MYGUI_SINGLETON_H__

#include "MyGUI_Diagnostic.h"

namespace MyGUI
{

#if MYGUI_COMPILER == MYGUI_COMPILER_MSVC || MYGUI_PLATFORM == MYGUI_PLATFORM_APPLE
	template <class T>
	class Singleton
#else
	template <class T>
	class MYGUI_EXPORT Singleton
#endif
	{
	public:
		typedef Singleton<T> Base;

		Singleton()
		{
			MYGUI_ASSERT(nullptr == msInstance, "Singleton instance " << getClassTypeName() << " already exsist");
			msInstance = static_cast<T*>(this);
		}

		virtual ~Singleton()
		{
			MYGUI_ASSERT(nullptr != msInstance, "Destroying Singleton instance " << getClassTypeName() << " before constructing it.");
			msInstance = nullptr;
		}

		static T& getInstance()
		{
			MYGUI_ASSERT(nullptr != getInstancePtr(), "Singleton instance " << getClassTypeName() << " was not created");
			return (*getInstancePtr());
		}

		static T* getInstancePtr()
		{
			return msInstance;
		}

		static const char* getClassTypeName()
		{
			return mClassTypeName;
		}

	private:
		static T* msInstance;
		static const char* mClassTypeName;
	};

} // namespace MyGUI

#endif // __MYGUI_SINGLETON_H__
