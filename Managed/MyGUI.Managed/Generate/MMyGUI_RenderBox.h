/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include "MMyGUI_Widget.h"


namespace MyGUI
{
	namespace Managed
	{

		public ref class RenderBox : public Widget
		{

			//--------------------------------------------------------------------
			// объявление типов и конструкторов
			MMYGUI_DECLARE_DERIVED( RenderBox, RenderBox, Widget );

			

			//InsertPoint

   	public:
		delegate void HandleUpdateViewport( Convert<MyGUI::RenderBox  *>::Type _sender );
		event HandleUpdateViewport^ EventUpdateViewport
		{
			void add(HandleUpdateViewport^ _value)
			{
				mDelegateUpdateViewport += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->eventUpdateViewport =
					static_cast< MyGUI::delegates::IDelegate1< MyGUI::RenderBox  * > *>(
						new Delegate1< HandleUpdateViewport^, MyGUI::RenderBox  * >(mDelegateUpdateViewport) );
			}
			void remove(HandleUpdateViewport^ _value)
			{
				mDelegateUpdateViewport -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateUpdateViewport == nullptr)
					static_cast<ThisType*>(mNative)->eventUpdateViewport = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventUpdateViewport =
						static_cast< MyGUI::delegates::IDelegate1< MyGUI::RenderBox  * > *>(
							new Delegate1< HandleUpdateViewport^, MyGUI::RenderBox  * >(mDelegateUpdateViewport) );
			}
		}
	private:
		HandleUpdateViewport^ mDelegateUpdateViewport;




		};

	} // namespace Managed
} // namespace MyGUI
