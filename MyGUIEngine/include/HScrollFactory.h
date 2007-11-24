#ifndef __HSCROLL_FACTORY_H__
#define __HSCROLL_FACTORY_H__

#include "Prerequest.h"
#include "WidgetFactoryInterface.h"
#include "WidgetDefines.h"

namespace MyGUI
{
	namespace factory
	{

		class _MyGUIExport HScrollFactory : public WidgetFactoryInterface
		{
		public:
			HScrollFactory::HScrollFactory();
			HScrollFactory::~HScrollFactory();

			// ���������� ���������� �������
			const Ogre::String& getType();
			WidgetPtr createWidget(const Ogre::String& _skin, int _left, int _top, int _width, int _height, Align _align, BasisWidgetPtr _parent, const Ogre::String& _name);

			// ������ ��� ��������
		};

	} // namespace factory
} // namespace MyGUI

#endif // __HSCROLL_FACTORY_H__