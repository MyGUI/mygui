/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include <MyGUI.h>

#include "Generate/MMyGUI_Widget.h"

namespace MMyGUI
{

	public ref class Gui
	{
	private:
		Gui() { }

	public:
		static property Gui^ Instance
		{
			Gui^ get( )
			{
				if (m_native == nullptr) m_native = MyGUI::Gui::getInstancePtr();
				return m_instance;
			}
		}

	public:
		generic <typename T> where T : ref class
		T CreateWidget(System::String^ _skin, IntCoord _coord, Align _align, System::String^ _layer, System::String^ _name)
		{
			Widget^ child = (Widget^)(System::Activator::CreateInstance<T>());
			child->CreateWidget(nullptr, _skin, _coord, _align, _layer, _name);
			return (T)child;
		}

	public:
		generic <typename T> where T : ref class
		T CreateWidget(System::String^ _skin, IntCoord _coord, Align _align, System::String^ _layer)
		{
			Widget^ child = (Widget^)(System::Activator::CreateInstance<T>());
			child->CreateWidget(nullptr, _skin, _coord, _align, _layer, "");
			return (T)child;
		}

	private:
		static Gui^ m_instance = gcnew Gui();
		static MyGUI::Gui * m_native = nullptr;

	};

} // namespace MMyGUI
