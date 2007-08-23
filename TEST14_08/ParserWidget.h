#pragma once

#include <Ogre.h>
#include "delegate.h"
#include "ParserManager.h"

namespace widget
{
	namespace helper
	{

		class ParserWidget
		{
		public:
			ParserWidget()
			{
				// �������� ���� �������� �������
				ParserManager & parser = ParserManager::getInstance();
				// ������ ��� ��������� ���������
				ParceDelegate & delegate = parser.registerDelegate("Caption");
				delegate = newDelegate(this, &ParserWidget::SetCaption);
			}

			void SetCaption(WidgetPtr _widget, const Ogre::String &_key, const Ogre::String &_value)
			{
				_widget->setCaption(_value);
			}

		};

	} // namespace helper
} // namespace widget