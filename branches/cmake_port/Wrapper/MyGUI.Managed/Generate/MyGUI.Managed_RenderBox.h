/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include "MyGUI.Managed_Widget.h"

namespace MyGUI
{
	namespace Managed
	{

		public ref class RenderBox : public Widget
		{

		private:
			typedef MyGUI::RenderBox ThisType;

		public:
			RenderBox() : Widget() { }

		internal:
			RenderBox( MyGUI::RenderBox* _native ) : Widget(_native) { }
			RenderBox( BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name )
			{
				CreateWidget(_parent, _style, _skin, _coord, _align, _layer, _name);
			}

			virtual const std::string& getClassTypeName() override { return ThisType::getClassTypeName(); }

			static BaseWidget^ WidgetCreator(BaseWidget^ _parent, MyGUI::WidgetStyle _style, const std::string& _skin, const MyGUI::IntCoord& _coord, MyGUI::Align _align, const std::string& _layer, const std::string& _name)
			{
				return gcnew RenderBox(_parent, _style, _skin, _coord, _align, _layer, _name);
			}


			//InsertPoint

   	public:
		delegate void HandleUpdateViewport(
			Convert<MyGUI::RenderBox  *>::Type _sender );
		event HandleUpdateViewport^ EventUpdateViewport
		{
			void add(HandleUpdateViewport^ _value)
			{
				mDelegateUpdateViewport += _value;
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->eventUpdateViewport =
					static_cast< MyGUI::delegates::IDelegate1<
						MyGUI::RenderBox  * > *>(
							new Delegate1< HandleUpdateViewport^ ,
							MyGUI::RenderBox  * >(mDelegateUpdateViewport) );
			}
			void remove(HandleUpdateViewport^ _value)
			{
				mDelegateUpdateViewport -= _value;
				MMYGUI_CHECK_NATIVE(mNative);
				if (mDelegateUpdateViewport == nullptr)
					static_cast<ThisType*>(mNative)->eventUpdateViewport = nullptr;
				else
					static_cast<ThisType*>(mNative)->eventUpdateViewport =
						static_cast< MyGUI::delegates::IDelegate1<
							MyGUI::RenderBox  * > *>(
								new Delegate1< HandleUpdateViewport^ ,
									MyGUI::RenderBox  * >(mDelegateUpdateViewport) );
			}
		}
	private:
		HandleUpdateViewport^ mDelegateUpdateViewport;



   	public:
		property Convert<const Ogre::ColourValue &>::Type BackgroundColour
		{
			Convert<const Ogre::ColourValue &>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<const Ogre::ColourValue &>::To( static_cast<ThisType*>(mNative)->getBackgroundColour() );
			}
			void set(Convert<const Ogre::ColourValue &>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setBackgroundColour( Convert<const Ogre::ColourValue &>::From(_value) );
			}
		}
	


   


   


   


   


   


   


   


   


   


   


		};

	} // namespace Managed
} // namespace MyGUI
