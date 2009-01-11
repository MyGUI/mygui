/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __MMYGUI_WIDGET_H__
#define __MMYGUI_WIDGET_H__

#include <MyGUI.h>

#include "MMyGUI_Utility.h"
#include "MMyGUI_Align.h"
#include "MMyGUI_IntCoord.h"
#include "MMyGUI_Delegate1.h"
#include "MMyGUI_Delegate2.h"
#include "MMyGUI_Delegate3.h"
#include "MMyGUI_Delegate4.h"
#include "MMyGUI_Property.h"

namespace MMyGUI
{

	public ref class Widget
	{

	public:
		//--------------------------------------------------------------------
		// базовые методы
		Widget() : mNative(0) { }
		Widget( MyGUI::WidgetPtr _native ) : mNative(_native) { }

		Widget( System::String^ _skin, IntCoord _coord, Align _align, System::String^ _layer)
		{
			CreateWidget(MyGUI::Widget::getClassTypeName(), nullptr,  _skin, _coord, _align, _layer, "");
		}

		Widget( System::String^ _skin, IntCoord _coord, Align _align, System::String^ _layer, System::String^ _name)
		{
			CreateWidget(MyGUI::Widget::getClassTypeName(), nullptr,  _skin, _coord, _align, _layer, _name);
		}

		Widget( Widget^ _parent, System::String^ _skin, IntCoord _coord, Align _align)
		{
			CreateWidget(MyGUI::Widget::getClassTypeName(), _parent,  _skin, _coord, _align, "", "");
		}

		Widget( Widget^ _parent, System::String^ _skin, IntCoord _coord, Align _align, System::String^ _name)
		{
			CreateWidget(MyGUI::Widget::getClassTypeName(), _parent,  _skin, _coord, _align, "", _name);
		}

		~Widget()
		{
			if (mNative != 0)
			{
				DestroyChilds();
				mParent = nullptr;
				MyGUI::WidgetManager::getInstance().destroyWidget(mNative);
				mNative = 0;
			}
		}

	protected:
		void CreateWidget(const std::string& _type, Widget^ _parent, System::String^ _skin, IntCoord _coord, Align _align, System::String^ _layer, System::String^ _name)
		{
			if (_parent == nullptr) {
				mNative = MyGUI::Gui::getInstance().createWidgetT(
					_type, 
					managed_to_utf8(_skin),
					_coord,
					_align,
					managed_to_utf8(_layer),
					managed_to_utf8(_name));
			}
			else {
				mNative = _parent->mNative->createWidgetT(
					MyGUI::Widget::getClassTypeName(), 
					managed_to_utf8(_skin),
					_coord,
					_align,
					managed_to_utf8(_name));

				mParent = _parent;
				mParent->mChilds.Add(this);
			}

			WidgetHolder sender = this;
			mNative->setUserData(sender);
		}

		void DestroyChilds()
		{
			while (mChilds.Count > 0) {
				Widget^ child = mChilds[mChilds.Count - 1];
				mChilds.RemoveAt(mChilds.Count - 1);
				delete child;
				child = nullptr;
			}
		}
		


		//--------------------------------------------------------------------
		// методы паблик интерфейса
		DECLARE_PROPERTY(Alpha, float);
		DECLARE_PROPERTY(Coord, IntCoord);


		//--------------------------------------------------------------------
		// делегаты паблик интерфейса
		MYGUI_DECLARE_DELEGATE2(MouseLostFocus, MyGUI::Widget*, MyGUI::Widget*);
		MYGUI_DECLARE_DELEGATE2(MouseSetFocus, MyGUI::Widget*, MyGUI::Widget*);

		MYGUI_DECLARE_DELEGATE3(MouseDrag, MyGUI::Widget*, int, int);
		MYGUI_DECLARE_DELEGATE3(MouseMove, MyGUI::Widget*, int, int);
		MYGUI_DECLARE_DELEGATE2(MouseWheel, MyGUI::Widget*, int);

		MYGUI_DECLARE_DELEGATE4(MouseButtonPressed, MyGUI::Widget*, int, int, MyGUI::MouseButton);
		MYGUI_DECLARE_DELEGATE4(MouseButtonReleased, MyGUI::Widget*, int, int, MyGUI::MouseButton);
		MYGUI_DECLARE_DELEGATE1(MouseButtonClick, MyGUI::Widget*);
		MYGUI_DECLARE_DELEGATE1(MouseButtonDoubleClick, MyGUI::Widget*);

		MYGUI_DECLARE_DELEGATE2(KeyLostFocus, MyGUI::Widget*, MyGUI::Widget*);
		MYGUI_DECLARE_DELEGATE2(KeySetFocus, MyGUI::Widget*, MyGUI::Widget*);

		MYGUI_DECLARE_DELEGATE3(KeyButtonPressed, MyGUI::Widget*, MyGUI::KeyCode, MyGUI::Char);
		MYGUI_DECLARE_DELEGATE2(KeyButtonReleased, MyGUI::Widget*, MyGUI::KeyCode);

		MYGUI_DECLARE_DELEGATE2(RootMouseChangeFocus, MyGUI::Widget*, bool);
		MYGUI_DECLARE_DELEGATE2(RootKeyChangeFocus, MyGUI::Widget*, bool);


		//--------------------------------------------------------------------
		// единственный указатель всей иерархии
	protected:
		MyGUI::Widget* mNative;
		Widget^ mParent;
		System::Collections::Generic::List<Widget^> mChilds;
	};

} // namespace MMyGUI

#endif // __MMYGUI_WIDGET_H__
