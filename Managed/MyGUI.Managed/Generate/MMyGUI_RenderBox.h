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
		void setPosition( ConvertToType<int>::Type _left, ConvertToType<int>::Type _top, ConvertToType<int>::Type _width, ConvertToType<int>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setPosition( ConvertToType<int>::ConvertFromValue(_left), ConvertToType<int>::ConvertFromValue(_top), ConvertToType<int>::ConvertFromValue(_width), ConvertToType<int>::ConvertFromValue(_height) );
		}



   	public:
		void setPosition( ConvertToType<const MyGUI::types::TCoord< int > &>::Type _coord )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setPosition( ConvertToType<const MyGUI::types::TCoord< int > &>::ConvertFromValue(_coord) );
		}



   	public:
		void setCoord( ConvertToType<int>::Type _left, ConvertToType<int>::Type _top, ConvertToType<int>::Type _width, ConvertToType<int>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setCoord( ConvertToType<int>::ConvertFromValue(_left), ConvertToType<int>::ConvertFromValue(_top), ConvertToType<int>::ConvertFromValue(_width), ConvertToType<int>::ConvertFromValue(_height) );
		}



   	public:
		void setSize( ConvertToType<int>::Type _width, ConvertToType<int>::Type _height )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setSize( ConvertToType<int>::ConvertFromValue(_width), ConvertToType<int>::ConvertFromValue(_height) );
		}



   	public:
		void setPosition( ConvertToType<int>::Type _left, ConvertToType<int>::Type _top )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setPosition( ConvertToType<int>::ConvertFromValue(_left), ConvertToType<int>::ConvertFromValue(_top) );
		}



   	public:
		void setCoord( ConvertToType<const MyGUI::types::TCoord< int > &>::Type _coord )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setCoord( ConvertToType<const MyGUI::types::TCoord< int > &>::ConvertFromValue(_coord) );
		}



   	public:
		void setSize( ConvertToType<const MyGUI::types::TSize< int > &>::Type _size )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setSize( ConvertToType<const MyGUI::types::TSize< int > &>::ConvertFromValue(_size) );
		}



   	public:
		void setPosition( ConvertToType<const MyGUI::types::TPoint< int > &>::Type _point )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setPosition( ConvertToType<const MyGUI::types::TPoint< int > &>::ConvertFromValue(_point) );
		}



   	public:
		ConvertToType<bool>::Type getScreenPosition( ConvertToType<const Ogre::Vector3>::Type _world, ConvertToType<Ogre::Vector2 &>::Type _screen )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<bool>::ConvertToValue( static_cast<ThisType*>(mNative)->getScreenPosition( ConvertToType<const Ogre::Vector3>::ConvertFromValue(_world), ConvertToType<Ogre::Vector2 &>::ConvertFromValue(_screen) ) );
		}



   	public:
		ConvertToType<Ogre::Viewport *>::Type getViewport( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<Ogre::Viewport *>::ConvertToValue( static_cast<ThisType*>(mNative)->getViewport( ) );
		}



   	public:
		void setRenderTarget( ConvertToType<Ogre::Camera *>::Type _camera )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setRenderTarget( ConvertToType<Ogre::Camera *>::ConvertFromValue(_camera) );
		}



   	public:
		property ConvertToType<bool>::Type ViewScale
		{
			ConvertToType<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return ConvertToType<bool>::ConvertToValue( static_cast<ThisType*>(mNative)->getViewScale() );
			}
			void set(ConvertToType<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setViewScale( ConvertToType<bool>::ConvertFromValue(_value) );
			}
		}



   	public:
		property ConvertToType<bool>::Type MouseRotation
		{
			ConvertToType<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return ConvertToType<bool>::ConvertToValue( static_cast<ThisType*>(mNative)->getMouseRotation() );
			}
			void set(ConvertToType<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setMouseRotation( ConvertToType<bool>::ConvertFromValue(_value) );
			}
		}



   	public:
		ConvertToType<Ogre::Degree>::Type getRotationAngle( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<Ogre::Degree>::ConvertToValue( static_cast<ThisType*>(mNative)->getRotationAngle( ) );
		}



   	public:
		void setRotationAngle( ConvertToType<const Ogre::Degree &>::Type _rotationAngle )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setRotationAngle( ConvertToType<const Ogre::Degree &>::ConvertFromValue(_rotationAngle) );
		}



   	public:
		property ConvertToType<const Ogre::ColourValue &>::Type BackgroungColour
		{
			ConvertToType<const Ogre::ColourValue &>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return ConvertToType<const Ogre::ColourValue &>::ConvertToValue( static_cast<ThisType*>(mNative)->getBackgroungColour() );
			}
			void set(ConvertToType<const Ogre::ColourValue &>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setBackgroungColour( ConvertToType<const Ogre::ColourValue &>::ConvertFromValue(_value) );
			}
		}



   	public:
		property ConvertToType<bool>::Type AutoRotation
		{
			ConvertToType<bool>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return ConvertToType<bool>::ConvertToValue( static_cast<ThisType*>(mNative)->getAutoRotation() );
			}
			void set(ConvertToType<bool>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setAutoRotation( ConvertToType<bool>::ConvertFromValue(_value) );
			}
		}



   	public:
		property ConvertToType<int>::Type AutoRotationSpeed
		{
			ConvertToType<int>::Type get( )
			{
				MMYGUI_CHECK_NATIVE(mNative);
				return ConvertToType<int>::ConvertToValue( static_cast<ThisType*>(mNative)->getAutoRotationSpeed() );
			}
			void set(ConvertToType<int>::Type _value)
			{
				MMYGUI_CHECK_NATIVE(mNative);
				static_cast<ThisType*>(mNative)->setAutoRotationSpeed( ConvertToType<int>::ConvertFromValue(_value) );
			}
		}



   	public:
		void clear( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->clear( );
		}



   	public:
		void setAnimation( ConvertToType<const Ogre::String &>::Type _animation )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->setAnimation( ConvertToType<const Ogre::String &>::ConvertFromValue(_animation) );
		}



   	public:
		void injectSceneNode( ConvertToType<Ogre::SceneManager *>::Type _manager, ConvertToType<Ogre::SceneNode *>::Type _sceneNode )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->injectSceneNode( ConvertToType<Ogre::SceneManager *>::ConvertFromValue(_manager), ConvertToType<Ogre::SceneNode *>::ConvertFromValue(_sceneNode) );
		}



   	public:
		void injectObject( ConvertToType<const Ogre::String &>::Type _meshName, ConvertToType<const Ogre::Vector3 &>::Type _position, ConvertToType<const Ogre::Quaternion &>::Type _orientation, ConvertToType<const Ogre::Vector3 &>::Type _scale )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			static_cast<ThisType*>(mNative)->injectObject( ConvertToType<const Ogre::String &>::ConvertFromValue(_meshName), ConvertToType<const Ogre::Vector3 &>::ConvertFromValue(_position), ConvertToType<const Ogre::Quaternion &>::ConvertFromValue(_orientation), ConvertToType<const Ogre::Vector3 &>::ConvertFromValue(_scale) );
		}



   	public:
		ConvertToType<const std::string &>::Type getTypeName( )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<const std::string &>::ConvertToValue( static_cast<ThisType*>(mNative)->getTypeName( ) );
		}



   	public:
		ConvertToType<bool>::Type isType( ConvertToType<const std::type_info &>::Type t )
		{
			MMYGUI_CHECK_NATIVE(mNative);
			return ConvertToType<bool>::ConvertToValue( static_cast<ThisType*>(mNative)->isType( ConvertToType<const std::type_info &>::ConvertFromValue(t) ) );
		}



	};

} // namespace MMyGUI
