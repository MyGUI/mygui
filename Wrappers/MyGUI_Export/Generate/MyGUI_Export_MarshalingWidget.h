/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/
#ifndef EXPORT_MARSHALING_WIDGET_H_
#define EXPORT_MARSHALING_WIDGET_H_

#include "../ExportDefine.h"
#include "../ExportMarshaling.h"

namespace Export
{
	//InsertPoint

	template <> struct Convert< MyGUI::MultiListItem*>
	{
		typedef MyGUI::Widget* Type;
		inline static Type To(MyGUI::MultiListItem* _value)
		{
			return _value;
		}
		inline static MyGUI::MultiListItem* From(Type _value)
		{
			return static_cast<MyGUI::MultiListItem*>(_value);
		}
	};
	template <> struct Convert< MyGUI::TabItem*>
	{
		typedef MyGUI::Widget* Type;
		inline static Type To(MyGUI::TabItem* _value)
		{
			return _value;
		}
		inline static MyGUI::TabItem* From(Type _value)
		{
			return static_cast<MyGUI::TabItem*>(_value);
		}
	};
	template <> struct Convert< MyGUI::MenuItem*>
	{
		typedef MyGUI::Widget* Type;
		inline static Type To(MyGUI::MenuItem* _value)
		{
			return _value;
		}
		inline static MyGUI::MenuItem* From(Type _value)
		{
			return static_cast<MyGUI::MenuItem*>(_value);
		}
	};
	template <> struct Convert< MyGUI::Window*>
	{
		typedef MyGUI::Widget* Type;
		inline static Type To(MyGUI::Window* _value)
		{
			return _value;
		}
		inline static MyGUI::Window* From(Type _value)
		{
			return static_cast<MyGUI::Window*>(_value);
		}
	};
	template <> struct Convert< MyGUI::ScrollBar*>
	{
		typedef MyGUI::Widget* Type;
		inline static Type To(MyGUI::ScrollBar* _value)
		{
			return _value;
		}
		inline static MyGUI::ScrollBar* From(Type _value)
		{
			return static_cast<MyGUI::ScrollBar*>(_value);
		}
	};
	template <> struct Convert< MyGUI::TabControl*>
	{
		typedef MyGUI::Widget* Type;
		inline static Type To(MyGUI::TabControl* _value)
		{
			return _value;
		}
		inline static MyGUI::TabControl* From(Type _value)
		{
			return static_cast<MyGUI::TabControl*>(_value);
		}
	};
	template <> struct Convert< MyGUI::TextBox*>
	{
		typedef MyGUI::Widget* Type;
		inline static Type To(MyGUI::TextBox* _value)
		{
			return _value;
		}
		inline static MyGUI::TextBox* From(Type _value)
		{
			return static_cast<MyGUI::TextBox*>(_value);
		}
	};
	template <> struct Convert< MyGUI::ImageBox*>
	{
		typedef MyGUI::Widget* Type;
		inline static Type To(MyGUI::ImageBox* _value)
		{
			return _value;
		}
		inline static MyGUI::ImageBox* From(Type _value)
		{
			return static_cast<MyGUI::ImageBox*>(_value);
		}
	};
	template <> struct Convert< MyGUI::ScrollView*>
	{
		typedef MyGUI::Widget* Type;
		inline static Type To(MyGUI::ScrollView* _value)
		{
			return _value;
		}
		inline static MyGUI::ScrollView* From(Type _value)
		{
			return static_cast<MyGUI::ScrollView*>(_value);
		}
	};
	template <> struct Convert< MyGUI::ProgressBar*>
	{
		typedef MyGUI::Widget* Type;
		inline static Type To(MyGUI::ProgressBar* _value)
		{
			return _value;
		}
		inline static MyGUI::ProgressBar* From(Type _value)
		{
			return static_cast<MyGUI::ProgressBar*>(_value);
		}
	};
	template <> struct Convert< MyGUI::PopupMenu*>
	{
		typedef MyGUI::Widget* Type;
		inline static Type To(MyGUI::PopupMenu* _value)
		{
			return _value;
		}
		inline static MyGUI::PopupMenu* From(Type _value)
		{
			return static_cast<MyGUI::PopupMenu*>(_value);
		}
	};
	template <> struct Convert< MyGUI::MultiListBox*>
	{
		typedef MyGUI::Widget* Type;
		inline static Type To(MyGUI::MultiListBox* _value)
		{
			return _value;
		}
		inline static MyGUI::MultiListBox* From(Type _value)
		{
			return static_cast<MyGUI::MultiListBox*>(_value);
		}
	};
	template <> struct Convert< MyGUI::MenuControl*>
	{
		typedef MyGUI::Widget* Type;
		inline static Type To(MyGUI::MenuControl* _value)
		{
			return _value;
		}
		inline static MyGUI::MenuControl* From(Type _value)
		{
			return static_cast<MyGUI::MenuControl*>(_value);
		}
	};
	template <> struct Convert< MyGUI::MenuBar*>
	{
		typedef MyGUI::Widget* Type;
		inline static Type To(MyGUI::MenuBar* _value)
		{
			return _value;
		}
		inline static MyGUI::MenuBar* From(Type _value)
		{
			return static_cast<MyGUI::MenuBar*>(_value);
		}
	};
	template <> struct Convert< MyGUI::ListBox*>
	{
		typedef MyGUI::Widget* Type;
		inline static Type To(MyGUI::ListBox* _value)
		{
			return _value;
		}
		inline static MyGUI::ListBox* From(Type _value)
		{
			return static_cast<MyGUI::ListBox*>(_value);
		}
	};
	template <> struct Convert< MyGUI::ItemBox*>
	{
		typedef MyGUI::Widget* Type;
		inline static Type To(MyGUI::ItemBox* _value)
		{
			return _value;
		}
		inline static MyGUI::ItemBox* From(Type _value)
		{
			return static_cast<MyGUI::ItemBox*>(_value);
		}
	};
	template <> struct Convert< MyGUI::EditBox*>
	{
		typedef MyGUI::Widget* Type;
		inline static Type To(MyGUI::EditBox* _value)
		{
			return _value;
		}
		inline static MyGUI::EditBox* From(Type _value)
		{
			return static_cast<MyGUI::EditBox*>(_value);
		}
	};
	template <> struct Convert< MyGUI::DDContainer*>
	{
		typedef MyGUI::Widget* Type;
		inline static Type To(MyGUI::DDContainer* _value)
		{
			return _value;
		}
		inline static MyGUI::DDContainer* From(Type _value)
		{
			return static_cast<MyGUI::DDContainer*>(_value);
		}
	};
	template <> struct Convert< MyGUI::ComboBox*>
	{
		typedef MyGUI::Widget* Type;
		inline static Type To(MyGUI::ComboBox* _value)
		{
			return _value;
		}
		inline static MyGUI::ComboBox* From(Type _value)
		{
			return static_cast<MyGUI::ComboBox*>(_value);
		}
	};
	template <> struct Convert< MyGUI::Button*>
	{
		typedef MyGUI::Widget* Type;
		inline static Type To(MyGUI::Button* _value)
		{
			return _value;
		}
		inline static MyGUI::Button* From(Type _value)
		{
			return static_cast<MyGUI::Button*>(_value);
		}
	};
	template <> struct Convert< MyGUI::Widget*>
	{
		typedef MyGUI::Widget* Type;
		inline static Type To(MyGUI::Widget* _value)
		{
			return _value;
		}
		inline static MyGUI::Widget* From(Type _value)
		{
			return static_cast<MyGUI::Widget*>(_value);
		}
	};
}

#endif // EXPORT_MARSHALING_WIDGET_H_
