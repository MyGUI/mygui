/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_SINGLETON_H_
#define MYGUI_SINGLETON_H_

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
	public:
		using Base = Singleton<T>;

		#if defined(__clang__)
			// This constructor is called before the `T` object is fully constructed, and
			// pointers are not dereferenced anyway, so UBSan shouldn't check vptrs.
			__attribute__((no_sanitize("vptr")))
		#endif
		Singleton()
		{
			MYGUI_ASSERT(nullptr == msInstance, "Singleton instance " << getClassTypeName() << " already exsist");
			msInstance = static_cast<T*>(this);
		}

		virtual ~Singleton()
		{
			if (nullptr == msInstance)
				MYGUI_LOG(Critical, "Destroying Singleton instance " << getClassTypeName() << " before constructing it.");
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

#endif // MYGUI_SINGLETON_H_
