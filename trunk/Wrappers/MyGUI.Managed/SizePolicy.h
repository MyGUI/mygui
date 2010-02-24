/*!
	@file
	@author		Albert Semenov
	@date		02/2010
	@module
*/
#pragma once

#include <MyGUI.h>
#include "Config.h"
#include "Marshaling.h"

namespace MyGUI
{
	namespace Managed
	{

		//#ifndef MMYGUI_USING_EXTERNAL_TYPE

		public enum struct SizePolicy
		{
		};

		//#endif // MMYGUI_USING_EXTERNAL_TYPE

		template <> struct Convert<MyGUI::SizePolicy>
		{
			typedef SizePolicy Type;
			inline static const SizePolicy& To(const MyGUI::SizePolicy& _value) { return reinterpret_cast<const SizePolicy&>(_value); }
			inline static MyGUI::SizePolicy& From(SizePolicy& _value) { return reinterpret_cast<MyGUI::SizePolicy&>(_value); }
		};

	} // namespace Managed
} // namespace MyGUI
