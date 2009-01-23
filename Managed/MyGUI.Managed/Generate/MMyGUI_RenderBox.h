/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/
#pragma once

#include "MMyGUI_Widget.h"


namespace MMyGUI
{

	public ref class RenderBox : public Widget
	{

		//--------------------------------------------------------------------
		// объявление типов и конструкторов
		MMYGUI_DECLARE_DERIVED( RenderBox, Widget );

		//InsertPoint

   	public:
		Convert<bool>::Type GetScreenPosition( Convert<const Ogre::Vector3>::Type _world, Convert<Ogre::Vector2 &>::Type _screen )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<bool>::To( static_cast<ThisType*>(mNative)->getScreenPosition( Convert<const Ogre::Vector3>::From(_world), Convert<Ogre::Vector2 &>::From(_screen) ) );
		}



   	public:
		Convert<Ogre::Viewport *>::Type GetViewport( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return Convert<Ogre::Viewport *>::To( static_cast<ThisType*>(mNative)->getViewport( ) );
		}



   	public:
		void SetRenderTarget( Convert<Ogre::Camera *>::Type _camera )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setRenderTarget( Convert<Ogre::Camera *>::From(_camera) );
		}



   	public:
		property Convert<bool>::Type ViewScale
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->getViewScale() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setViewScale( Convert<bool>::From(_value) );
			}
		}



   	public:
		property Convert<bool>::Type MouseRotation
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->getMouseRotation() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setMouseRotation( Convert<bool>::From(_value) );
			}
		}



   	public:
		property Convert<const Ogre::Degree &>::Type RotationAngle
		{
			Convert<const Ogre::Degree &>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<const Ogre::Degree &>::To( static_cast<ThisType*>(mNative)->getRotationAngle() );
			}
			void set(Convert<const Ogre::Degree &>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setRotationAngle( Convert<const Ogre::Degree &>::From(_value) );
			}
		}



   	public:
		property Convert<const Ogre::ColourValue &>::Type BackgroungColour
		{
			Convert<const Ogre::ColourValue &>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<const Ogre::ColourValue &>::To( static_cast<ThisType*>(mNative)->getBackgroungColour() );
			}
			void set(Convert<const Ogre::ColourValue &>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setBackgroungColour( Convert<const Ogre::ColourValue &>::From(_value) );
			}
		}



   	public:
		property Convert<bool>::Type AutoRotation
		{
			Convert<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<bool>::To( static_cast<ThisType*>(mNative)->getAutoRotation() );
			}
			void set(Convert<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setAutoRotation( Convert<bool>::From(_value) );
			}
		}



   	public:
		property Convert<int>::Type AutoRotationSpeed
		{
			Convert<int>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return Convert<int>::To( static_cast<ThisType*>(mNative)->getAutoRotationSpeed() );
			}
			void set(Convert<int>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setAutoRotationSpeed( Convert<int>::From(_value) );
			}
		}



   	public:
		void Clear( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->clear( );
		}



   	public:
		void SetAnimation( Convert<const Ogre::String &>::Type _animation )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setAnimation( Convert<const Ogre::String &>::From(_animation) );
		}



   	public:
		void InjectSceneNode( Convert<Ogre::SceneManager *>::Type _manager, Convert<Ogre::SceneNode *>::Type _sceneNode )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->injectSceneNode( Convert<Ogre::SceneManager *>::From(_manager), Convert<Ogre::SceneNode *>::From(_sceneNode) );
		}



   	public:
		void InjectObject( Convert<const Ogre::String &>::Type _meshName, Convert<const Ogre::Vector3 &>::Type _position, Convert<const Ogre::Quaternion &>::Type _orientation, Convert<const Ogre::Vector3 &>::Type _scale )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->injectObject( Convert<const Ogre::String &>::From(_meshName), Convert<const Ogre::Vector3 &>::From(_position), Convert<const Ogre::Quaternion &>::From(_orientation), Convert<const Ogre::Vector3 &>::From(_scale) );
		}


   	public:
		void InjectObject( Convert<const Ogre::String &>::Type _meshName, Convert<const Ogre::Vector3 &>::Type _position, Convert<const Ogre::Quaternion &>::Type _orientation )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->injectObject( Convert<const Ogre::String &>::From(_meshName), Convert<const Ogre::Vector3 &>::From(_position), Convert<const Ogre::Quaternion &>::From(_orientation) );
		}


   	public:
		void InjectObject( Convert<const Ogre::String &>::Type _meshName, Convert<const Ogre::Vector3 &>::Type _position )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->injectObject( Convert<const Ogre::String &>::From(_meshName), Convert<const Ogre::Vector3 &>::From(_position) );
		}


   	public:
		void InjectObject( Convert<const Ogre::String &>::Type _meshName )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->injectObject( Convert<const Ogre::String &>::From(_meshName) );
		}



	};

} // namespace MMyGUI
