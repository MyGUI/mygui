/*!
	@file
	@author		Albert Semenov
	@date		04/2008
	@module
*/
#ifndef __MYGUI_MULTI_LIST_FACTORY_H__
#define __MYGUI_MULTI_LIST_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_WidgetFactoryInterface.h"
#include "MyGUI_WidgetDefines.h"

namespace MyGUI
{
	namespace factory
	{

		class _MyGUIExport MultiListFactory : public WidgetFactoryInterface
		{
		public:
			MultiListFactory();
			~MultiListFactory();

			// ���������� ���������� �������
			const std::string& getType();
			WidgetPtr createWidget(const std::string& _skin, const IntCoord& _coord, Align _align, CroppedRectangleInterface * _parent, WidgetCreator * _creator, const std::string& _name);

		};

	} // namespace factory
} // namespace MyGUI

#endif // __MYGUI_MULTI_LIST_FACTORY_H__
