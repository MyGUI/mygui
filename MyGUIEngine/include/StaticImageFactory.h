#ifndef __STATIC_IMAGE_FACTORY_H__
#define __STATIC_IMAGE_FACTORY_H__

#include "Prerequest.h"
#include "WidgetFactoryInterface.h"
#include "WidgetDefines.h"

namespace MyGUI
{
	namespace factory
	{

		class _MyGUIExport StaticImageFactory : public WidgetFactoryInterface
		{
		public:
			StaticImageFactory::StaticImageFactory();
			StaticImageFactory::~StaticImageFactory();

			// ���������� ���������� �������
			const Ogre::String& getType();
			WidgetPtr createWidget(const Ogre::String& _skin, int _left, int _top, int _width, int _height, Align _align, BasisWidgetPtr _parent, const Ogre::String& _name);

			// ������ ��� ��������
			void ImageMaterial(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void ImageRect(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void ImageTile(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
			void ImageNum(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value);
		};

	} // namespace factory
} // namespace MyGUI

#endif // __STATIC_IMAGE_FACTORY_H__