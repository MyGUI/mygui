/*!
	@file
	@author		Albert Semenov
	@date		01/2009
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

		public value struct DDWidgetState
		{
			System::UInt32 index;
			bool update;
			bool accept;
			bool refuse;
		};

		//#endif // MMYGUI_USING_EXTERNAL_TYPE

		template <> struct Convert<const MyGUI::DDWidgetState&>
		{
			typedef DDWidgetState Type;
			inline static const DDWidgetState& To(const MyGUI::DDWidgetState& _value)
			{
				return reinterpret_cast<const DDWidgetState&>(_value);
			}
			inline static MyGUI::DDWidgetState& From(DDWidgetState& _value)
			{
				return reinterpret_cast<MyGUI::DDWidgetState&>(_value);
			}
		};

	} // namespace Managed
} // namespace MyGUI
