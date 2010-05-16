/*!
	@file
	@author		George Evmenov
	@date		02/2010
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
#ifndef __MYGUI_SINGLETON_H__
#define __MYGUI_SINGLETON_H__

#include "MyGUI_Diagnostic.h"
namespace MyGUI
{
#if MYGUI_COMPILER == MYGUI_COMPILER_MSVC
	template <class T>
	class Singleton
#else
	template <class T>
	class MYGUI_EXPORT Singleton
#endif
	{
	private:
		static T* msInstance;
	protected:
		static const char* INSTANCE_TYPE_NAME;
		bool mIsInitialise;
	public:
		Singleton() :
			mIsInitialise(false)
		{
			MYGUI_ASSERT(nullptr == msInstance, "Singleton instance " << INSTANCE_TYPE_NAME << " already exsist");
			msInstance = static_cast<T*>(this);
		}
		virtual ~Singleton()
		{
			MYGUI_ASSERT(nullptr != msInstance, "Destroying Singleton instance " << INSTANCE_TYPE_NAME << " before constructing it.");
			msInstance = nullptr;
		}
		static T& getInstance()
		{
			MYGUI_ASSERT(nullptr != getInstancePtr(), "Singleton instance " << INSTANCE_TYPE_NAME << " was not created");
			return (*getInstancePtr());
		}
		static T* getInstancePtr() { return msInstance; }
	};

	template <class T>
	T* Singleton<T>::msInstance = nullptr;

} // namespace MyGUI

#endif // __MYGUI_SINGLETON_H__
