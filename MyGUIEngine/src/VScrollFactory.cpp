
#include "VScrollFactory.h"
#include "VScroll.h"
#include "SkinManager.h"
#include "WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		VScrollFactory::VScrollFactory()
		{
			// ������������ ����
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);

			// ������������ ��� �������
		}

		VScrollFactory::~VScrollFactory()
		{
			// ������������ ����
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);

			// ������������ ��� �������
		}

		const Ogre::String& VScrollFactory::getType()
		{
			static Ogre::String type("VScroll");
			return type;
		}

		WidgetPtr VScrollFactory::createWidget(const Ogre::String& _skin, int _x, int _y, int _cx, int _cy, Align _align, BasisWidgetPtr _parent, const Ogre::String& _name)
		{
			return new VScroll(_x, _y, _cx, _cy, _align, SkinManager::getInstance().getSkin(_skin), _parent, _name);
		}

	} // namespace factory
} // namespace MyGUI