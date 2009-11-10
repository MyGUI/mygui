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



   	template <> struct Convert< MyGUI::VScroll* >
	{
		typedef Interface Type;
		inline static Interface To(MyGUI::VScroll* _value)
		{
			if (_value == nullptr) return nullptr;
			Interface* wrapper = _value->getUserData<Interface>(false);
			if (wrapper == nullptr)
			{
				return CreateWrapper(_value);
			}
			return *wrapper;
		}
		inline static MyGUI::VScroll* From(Interface _value)
		{
			return static_cast<MyGUI::VScroll*>( GetNativeByWrapper(_value) );
		}
	};



   	template <> struct Convert< MyGUI::Tab* >
	{
		typedef Interface Type;
		inline static Interface To(MyGUI::Tab* _value)
		{
			if (_value == nullptr) return nullptr;
			Interface* wrapper = _value->getUserData<Interface>(false);
			if (wrapper == nullptr)
			{
				return CreateWrapper(_value);
			}
			return *wrapper;
		}
		inline static MyGUI::Tab* From(Interface _value)
		{
			return static_cast<MyGUI::Tab*>( GetNativeByWrapper(_value) );
		}
	};



   	template <> struct Convert< MyGUI::StaticText* >
	{
		typedef Interface Type;
		inline static Interface To(MyGUI::StaticText* _value)
		{
			if (_value == nullptr) return nullptr;
			Interface* wrapper = _value->getUserData<Interface>(false);
			if (wrapper == nullptr)
			{
				return CreateWrapper(_value);
			}
			return *wrapper;
		}
		inline static MyGUI::StaticText* From(Interface _value)
		{
			return static_cast<MyGUI::StaticText*>( GetNativeByWrapper(_value) );
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
				return CreateWrapper(_value);
			}
			return *wrapper;
		}
		inline static MyGUI::StaticImage* From(Interface _value)
		{
			return static_cast<MyGUI::StaticImage*>( GetNativeByWrapper(_value) );
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



   	template <> struct Convert< MyGUI::RenderBox* >
	{
		typedef Interface Type;
		inline static Interface To(MyGUI::RenderBox* _value)
		{
			if (_value == nullptr) return nullptr;
			Interface* wrapper = _value->getUserData<Interface>(false);
			if (wrapper == nullptr)
			{
				return CreateWrapper(_value);
			}
			return *wrapper;
		}
		inline static MyGUI::RenderBox* From(Interface _value)
		{
			return static_cast<MyGUI::RenderBox*>( GetNativeByWrapper(_value) );
		}
	};



   	template <> struct Convert< MyGUI::Progress* >
	{
		typedef Interface Type;
		inline static Interface To(MyGUI::Progress* _value)
		{
			if (_value == nullptr) return nullptr;
			Interface* wrapper = _value->getUserData<Interface>(false);
			if (wrapper == nullptr)
			{
				return CreateWrapper(_value);
			}
			return *wrapper;
		}
		inline static MyGUI::Progress* From(Interface _value)
		{
			return static_cast<MyGUI::Progress*>( GetNativeByWrapper(_value) );
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



   	template <> struct Convert< MyGUI::MultiList* >
	{
		typedef Interface Type;
		inline static Interface To(MyGUI::MultiList* _value)
		{
			if (_value == nullptr) return nullptr;
			Interface* wrapper = _value->getUserData<Interface>(false);
			if (wrapper == nullptr)
			{
				return CreateWrapper(_value);
			}
			return *wrapper;
		}
		inline static MyGUI::MultiList* From(Interface _value)
		{
			return static_cast<MyGUI::MultiList*>( GetNativeByWrapper(_value) );
		}
	};



   	template <> struct Convert< MyGUI::Message* >
	{
		typedef Interface Type;
		inline static Interface To(MyGUI::Message* _value)
		{
			if (_value == nullptr) return nullptr;
			Interface* wrapper = _value->getUserData<Interface>(false);
			if (wrapper == nullptr)
			{
				return CreateWrapper(_value);
			}
			return *wrapper;
		}
		inline static MyGUI::Message* From(Interface _value)
		{
			return static_cast<MyGUI::Message*>( GetNativeByWrapper(_value) );
		}
	};



   	template <> struct Convert< MyGUI::MenuCtrl* >
	{
		typedef Interface Type;
		inline static Interface To(MyGUI::MenuCtrl* _value)
		{
			if (_value == nullptr) return nullptr;
			Interface* wrapper = _value->getUserData<Interface>(false);
			if (wrapper == nullptr)
			{
				return CreateWrapper(_value);
			}
			return *wrapper;
		}
		inline static MyGUI::MenuCtrl* From(Interface _value)
		{
			return static_cast<MyGUI::MenuCtrl*>( GetNativeByWrapper(_value) );
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



   	template <> struct Convert< MyGUI::List* >
	{
		typedef Interface Type;
		inline static Interface To(MyGUI::List* _value)
		{
			if (_value == nullptr) return nullptr;
			Interface* wrapper = _value->getUserData<Interface>(false);
			if (wrapper == nullptr)
			{
				return CreateWrapper(_value);
			}
			return *wrapper;
		}
		inline static MyGUI::List* From(Interface _value)
		{
			return static_cast<MyGUI::List*>( GetNativeByWrapper(_value) );
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



   	template <> struct Convert< MyGUI::HScroll* >
	{
		typedef Interface Type;
		inline static Interface To(MyGUI::HScroll* _value)
		{
			if (_value == nullptr) return nullptr;
			Interface* wrapper = _value->getUserData<Interface>(false);
			if (wrapper == nullptr)
			{
				return CreateWrapper(_value);
			}
			return *wrapper;
		}
		inline static MyGUI::HScroll* From(Interface _value)
		{
			return static_cast<MyGUI::HScroll*>( GetNativeByWrapper(_value) );
		}
	};



   	template <> struct Convert< MyGUI::Edit* >
	{
		typedef Interface Type;
		inline static Interface To(MyGUI::Edit* _value)
		{
			if (_value == nullptr) return nullptr;
			Interface* wrapper = _value->getUserData<Interface>(false);
			if (wrapper == nullptr)
			{
				return CreateWrapper(_value);
			}
			return *wrapper;
		}
		inline static MyGUI::Edit* From(Interface _value)
		{
			return static_cast<MyGUI::Edit*>( GetNativeByWrapper(_value) );
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



   	template <> struct Convert< MyGUI::Canvas* >
	{
		typedef Interface Type;
		inline static Interface To(MyGUI::Canvas* _value)
		{
			if (_value == nullptr) return nullptr;
			Interface* wrapper = _value->getUserData<Interface>(false);
			if (wrapper == nullptr)
			{
				return CreateWrapper(_value);
			}
			return *wrapper;
		}
		inline static MyGUI::Canvas* From(Interface _value)
		{
			return static_cast<MyGUI::Canvas*>( GetNativeByWrapper(_value) );
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



} // namespace Export

#endif // __EXPORT_MARSHALING_WIDGET_H__
