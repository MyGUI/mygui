/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __EXPORT_MARSHALING_WIDGET_H__
#define __EXPORT_MARSHALING_WIDGET_H__

#include "ExportDefine.h"
#include "ExportMarshaling.h"

namespace Export
{

	template <> struct Convert< MyGUI::Widget* >
	{
		typedef Interface Type;
		inline static Interface To(MyGUI::Widget* _value)
		{
			if (_value == nullptr) return nullptr;
			Interface* wrapper = _value->getUserData<Interface>(false);
			if (wrapper == nullptr)
			{
				// создать враппер виджета
				return CreateWrapper(_value);
			}
			return *wrapper;
		}
	};

	template <> struct Convert< MyGUI::StaticImage* >
	{
		typedef Interface Type;
		inline static Interface To(MyGUI::StaticImage* _value)
		{
			if (_value == nullptr) return nullptr;
			Interface* wrapper = _value->getUserData<Interface>(false);
			if (wrapper == nullptr)
			{
				// создать враппер виджета
				return CreateWrapper(_value);
			}
			return *wrapper;
		}
	};

	template <> struct Convert< MyGUI::ComboBox* >
	{
		typedef Interface Type;
		inline static Interface To(MyGUI::ComboBox* _value)
		{
			if (_value == nullptr) return nullptr;
			Interface* wrapper = _value->getUserData<Interface>(false);
			if (wrapper == nullptr)
			{
				// создать враппер виджета
				return CreateWrapper(_value);
			}
			return *wrapper;
		}
	};

	template <> struct Convert< MyGUI::DDContainer* >
	{
		typedef Interface Type;
		inline static Interface To(MyGUI::DDContainer* _value)
		{
			if (_value == nullptr) return nullptr;
			Interface* wrapper = _value->getUserData<Interface>(false);
			if (wrapper == nullptr)
			{
				// создать враппер виджета
				return CreateWrapper(_value);
			}
			return *wrapper;
		}
	};

} // namespace Export

#endif // __EXPORT_MARSHALING_WIDGET_H__
