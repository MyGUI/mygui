/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#include "MyGUI_SheetFactory.h"
#include "MyGUI_Sheet.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_WidgetManager.h"

namespace MyGUI
{
	namespace factory
	{

		SheetFactory::SheetFactory()
		{
			// ������������ ����
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.registerFactory(this);

			// ������������ ��� �������
			manager.registerDelegate("Sheet_ButtonWidth") = newDelegate(this, &SheetFactory::Sheet_ButtonWidth);
			manager.registerDelegate("Sheet_Select") = newDelegate(this, &SheetFactory::Sheet_Select);
			//manager.registerDelegate("Sheet_SmoothSelect") = newDelegate(this, &SheetFactory::Sheet_Select);
		}

		SheetFactory::~SheetFactory()
		{
			// ������� ����
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			manager.unregisterFactory(this);

			// ������� ��� �������
			manager.unregisterDelegate("Sheet_ButtonWidth");
			manager.unregisterDelegate("Sheet_Select");
			//manager.unregisterDelegate("Sheet_SmoothSelect");
		}

		const std::string & SheetFactory::getTypeName()
		{
			return Sheet::getClassTypeName();
		}

		WidgetPtr SheetFactory::createWidget(const std::string& _skin, const IntCoord& _coord, Align _align, ICroppedRectangle * _parent, IWidgetCreator * _creator, const std::string& _name)
		{
			return new Sheet(_coord, _align, SkinManager::getInstance().getSkin(_skin), _parent, _creator, _name);
		}

		void SheetFactory::Sheet_ButtonWidth(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(Sheet, _widget, _key);
			static_cast<SheetPtr>(_widget)->setButtonWidth(utility::parseInt(_value));
		}

		void SheetFactory::Sheet_Select(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			MYGUI_RETURN_IS_FALSE_TYPE(Sheet, _widget, _key);
			if (utility::parseBool(_value)) static_cast<SheetPtr>(_widget)->setItemSelected();
		}

	} // namespace factory
} // namespace MyGUI
