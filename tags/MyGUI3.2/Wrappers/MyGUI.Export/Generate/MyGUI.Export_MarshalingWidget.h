/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __EXPORT_MARSHALING_WIDGET_H__
#define __EXPORT_MARSHALING_WIDGET_H__

#include "../ExportDefine.h"
#include "../ExportMarshaling.h"

namespace Export
{

	//InsertPoint

   	template <> struct Convert< MyGUI::MultiListItem* >
	{
		typedef Interface Type;
		inline static Interface To(MyGUI::MultiListItem* _value)
		{
			if (_value == nullptr) return nullptr;
			Interface* wrapper = _value->getUserData<Interface>(false);
			if (wrapper == nullptr)
			{
				return CreateWrapper(_value);
			}
			return *wrapper;
		}
		inline static MyGUI::MultiListItem* From(Interface _value)
		{
			return static_cast<MyGUI::MultiListItem*>( GetNativeByWrapper(_value) );
		}
	};



   	template <> struct Convert< MyGUI::TabItem* >
	{
		typedef Interface Type;
		inline static Interface To(MyGUI::TabItem* _value)
		{
			if (_value == nullptr) return nullptr;
			Interface* wrapper = _value->getUserData<Interface>(false);
			if (wrapper == nullptr)
			{
				return CreateWrapper(_value);
			}
			return *wrapper;
		}
		inline static MyGUI::TabItem* From(Interface _value)
		{
			return static_cast<MyGUI::TabItem*>( GetNativeByWrapper(_value) );
		}
	};



   	template <> struct Convert< MyGUI::MenuItem* >
	{
		typedef Interface Type;
		inline static Interface To(MyGUI::MenuItem* _value)
		{
			if (_value == nullptr) return nullptr;
			Interface* wrapper = _value->getUserData<Interface>(false);
			if (wrapper == nullptr)
			{
				return CreateWrapper(_value);
			}
			return *wrapper;
		}
		inline static MyGUI::MenuItem* From(Interface _value)
		{
			return static_cast<MyGUI::MenuItem*>( GetNativeByWrapper(_value) );
		}
	};



   	template <> struct Convert< MyGUI::Window* >
	{
		typedef Interface Type;
		inline static Interface To(MyGUI::Window* _value)
		{
			if (_value == nullptr) return nullptr;
			Interface* wrapper = _value->getUserData<Interface>(false);
			if (wrapper == nullptr)
			{
				return CreateWrapper(_value);
			}
			return *wrapper;
		}
		inline static MyGUI::Window* From(Interface _value)
		{
			return static_cast<MyGUI::Window*>( GetNativeByWrapper(_value) );
		}
	};



   	template <> struct Convert< MyGUI::ScrollBar* >
	{
		typedef Interface Type;
		inline static Interface To(MyGUI::ScrollBar* _value)
		{
			if (_value == nullptr) return nullptr;
			Interface* wrapper = _value->getUserData<Interface>(false);
			if (wrapper == nullptr)
			{
				return CreateWrapper(_value);
			}
			return *wrapper;
		}
		inline static MyGUI::ScrollBar* From(Interface _value)
		{
			return static_cast<MyGUI::ScrollBar*>( GetNativeByWrapper(_value) );
		}
	};



   	template <> struct Convert< MyGUI::TabControl* >
	{
		typedef Interface Type;
		inline static Interface To(MyGUI::TabControl* _value)
		{
			if (_value == nullptr) return nullptr;
			Interface* wrapper = _value->getUserData<Interface>(false);
			if (wrapper == nullptr)
			{
				return CreateWrapper(_value);
			}
			return *wrapper;
		}
		inline static MyGUI::TabControl* From(Interface _value)
		{
			return static_cast<MyGUI::TabControl*>( GetNativeByWrapper(_value) );
		}
	};



   	template <> struct Convert< MyGUI::TextBox* >
	{
		typedef Interface Type;
		inline static Interface To(MyGUI::TextBox* _value)
		{
			if (_value == nullptr) return nullptr;
			Interface* wrapper = _value->getUserData<Interface>(false);
			if (wrapper == nullptr)
			{
				return CreateWrapper(_value);
			}
			return *wrapper;
		}
		inline static MyGUI::TextBox* From(Interface _value)
		{
			return static_cast<MyGUI::TextBox*>( GetNativeByWrapper(_value) );
		}
	};



   	template <> struct Convert< MyGUI::ImageBox* >
	{
		typedef Interface Type;
		inline static Interface To(MyGUI::ImageBox* _value)
		{
			if (_value == nullptr) return nullptr;
			Interface* wrapper = _value->getUserData<Interface>(false);
			if (wrapper == nullptr)
			{
				return CreateWrapper(_value);
			}
			return *wrapper;
		}
		inline static MyGUI::ImageBox* From(Interface _value)
		{
			return static_cast<MyGUI::ImageBox*>( GetNativeByWrapper(_value) );
		}
	};



   	template <> struct Convert< MyGUI::ScrollView* >
	{
		typedef Interface Type;
		inline static Interface To(MyGUI::ScrollView* _value)
		{
			if (_value == nullptr) return nullptr;
			Interface* wrapper = _value->getUserData<Interface>(false);
			if (wrapper == nullptr)
			{
				return CreateWrapper(_value);
			}
			return *wrapper;
		}
		inline static MyGUI::ScrollView* From(Interface _value)
		{
			return static_cast<MyGUI::ScrollView*>( GetNativeByWrapper(_value) );
		}
	};



   	template <> struct Convert< MyGUI::ProgressBar* >
	{
		typedef Interface Type;
		inline static Interface To(MyGUI::ProgressBar* _value)
		{
			if (_value == nullptr) return nullptr;
			Interface* wrapper = _value->getUserData<Interface>(false);
			if (wrapper == nullptr)
			{
				return CreateWrapper(_value);
			}
			return *wrapper;
		}
		inline static MyGUI::ProgressBar* From(Interface _value)
		{
			return static_cast<MyGUI::ProgressBar*>( GetNativeByWrapper(_value) );
		}
	};



   	template <> struct Convert< MyGUI::PopupMenu* >
	{
		typedef Interface Type;
		inline static Interface To(MyGUI::PopupMenu* _value)
		{
			if (_value == nullptr) return nullptr;
			Interface* wrapper = _value->getUserData<Interface>(false);
			if (wrapper == nullptr)
			{
				return CreateWrapper(_value);
			}
			return *wrapper;
		}
		inline static MyGUI::PopupMenu* From(Interface _value)
		{
			return static_cast<MyGUI::PopupMenu*>( GetNativeByWrapper(_value) );
		}
	};



   	template <> struct Convert< MyGUI::MultiListBox* >
	{
		typedef Interface Type;
		inline static Interface To(MyGUI::MultiListBox* _value)
		{
			if (_value == nullptr) return nullptr;
			Interface* wrapper = _value->getUserData<Interface>(false);
			if (wrapper == nullptr)
			{
				return CreateWrapper(_value);
			}
			return *wrapper;
		}
		inline static MyGUI::MultiListBox* From(Interface _value)
		{
			return static_cast<MyGUI::MultiListBox*>( GetNativeByWrapper(_value) );
		}
	};



   	template <> struct Convert< MyGUI::MenuControl* >
	{
		typedef Interface Type;
		inline static Interface To(MyGUI::MenuControl* _value)
		{
			if (_value == nullptr) return nullptr;
			Interface* wrapper = _value->getUserData<Interface>(false);
			if (wrapper == nullptr)
			{
				return CreateWrapper(_value);
			}
			return *wrapper;
		}
		inline static MyGUI::MenuControl* From(Interface _value)
		{
			return static_cast<MyGUI::MenuControl*>( GetNativeByWrapper(_value) );
		}
	};



   	template <> struct Convert< MyGUI::MenuBar* >
	{
		typedef Interface Type;
		inline static Interface To(MyGUI::MenuBar* _value)
		{
			if (_value == nullptr) return nullptr;
			Interface* wrapper = _value->getUserData<Interface>(false);
			if (wrapper == nullptr)
			{
				return CreateWrapper(_value);
			}
			return *wrapper;
		}
		inline static MyGUI::MenuBar* From(Interface _value)
		{
			return static_cast<MyGUI::MenuBar*>( GetNativeByWrapper(_value) );
		}
	};



   	template <> struct Convert< MyGUI::ListBox* >
	{
		typedef Interface Type;
		inline static Interface To(MyGUI::ListBox* _value)
		{
			if (_value == nullptr) return nullptr;
			Interface* wrapper = _value->getUserData<Interface>(false);
			if (wrapper == nullptr)
			{
				return CreateWrapper(_value);
			}
			return *wrapper;
		}
		inline static MyGUI::ListBox* From(Interface _value)
		{
			return static_cast<MyGUI::ListBox*>( GetNativeByWrapper(_value) );
		}
	};



   	template <> struct Convert< MyGUI::ItemBox* >
	{
		typedef Interface Type;
		inline static Interface To(MyGUI::ItemBox* _value)
		{
			if (_value == nullptr) return nullptr;
			Interface* wrapper = _value->getUserData<Interface>(false);
			if (wrapper == nullptr)
			{
				return CreateWrapper(_value);
			}
			return *wrapper;
		}
		inline static MyGUI::ItemBox* From(Interface _value)
		{
			return static_cast<MyGUI::ItemBox*>( GetNativeByWrapper(_value) );
		}
	};



   	template <> struct Convert< MyGUI::EditBox* >
	{
		typedef Interface Type;
		inline static Interface To(MyGUI::EditBox* _value)
		{
			if (_value == nullptr) return nullptr;
			Interface* wrapper = _value->getUserData<Interface>(false);
			if (wrapper == nullptr)
			{
				return CreateWrapper(_value);
			}
			return *wrapper;
		}
		inline static MyGUI::EditBox* From(Interface _value)
		{
			return static_cast<MyGUI::EditBox*>( GetNativeByWrapper(_value) );
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
				return CreateWrapper(_value);
			}
			return *wrapper;
		}
		inline static MyGUI::DDContainer* From(Interface _value)
		{
			return static_cast<MyGUI::DDContainer*>( GetNativeByWrapper(_value) );
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
				return CreateWrapper(_value);
			}
			return *wrapper;
		}
		inline static MyGUI::ComboBox* From(Interface _value)
		{
			return static_cast<MyGUI::ComboBox*>( GetNativeByWrapper(_value) );
		}
	};



   	template <> struct Convert< MyGUI::Button* >
	{
		typedef Interface Type;
		inline static Interface To(MyGUI::Button* _value)
		{
			if (_value == nullptr) return nullptr;
			Interface* wrapper = _value->getUserData<Interface>(false);
			if (wrapper == nullptr)
			{
				return CreateWrapper(_value);
			}
			return *wrapper;
		}
		inline static MyGUI::Button* From(Interface _value)
		{
			return static_cast<MyGUI::Button*>( GetNativeByWrapper(_value) );
		}
	};



   	template <> struct Convert< MyGUI::Widget* >
	{
		typedef Interface Type;
		inline static Interface To(MyGUI::Widget* _value)
		{
			if (_value == nullptr) return nullptr;
			Interface* wrapper = _value->getUserData<Interface>(false);
			if (wrapper == nullptr)
			{
				return CreateWrapper(_value);
			}
			return *wrapper;
		}
		inline static MyGUI::Widget* From(Interface _value)
		{
			return static_cast<MyGUI::Widget*>( GetNativeByWrapper(_value) );
		}
	};



} // namespace Export

#endif // __EXPORT_MARSHALING_WIDGET_H__
