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

		public enum struct LogLevel
		{
			Info = MyGUI::LogLevel::Info,
			Warning = MyGUI::LogLevel::Warning,
			Error = MyGUI::LogLevel::Error,
			Critical = MyGUI::LogLevel::Critical,
		};

		//#endif // MMYGUI_USING_EXTERNAL_TYPE

		template <> struct Convert<MyGUI::LogLevel>
		{
			typedef LogLevel Type;
			inline static const LogLevel& To(const MyGUI::LogLevel& _value)
			{
				return reinterpret_cast<const LogLevel&>(_value);
			}
			inline static MyGUI::LogLevel& From(LogLevel& _value)
			{
				return reinterpret_cast<MyGUI::LogLevel&>(_value);
			}
		};

	} // namespace Managed
} // namespace MyGUI
