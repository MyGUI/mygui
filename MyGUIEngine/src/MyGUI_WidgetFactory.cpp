/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*//*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "MyGUI_Precompiled.h"
#include "MyGUI_WidgetFactory.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_DelegateManager.h"
#include "MyGUI_LanguageManager.h"

#define FACTORY_WIDGET_EVENT_DELEGATE(eventName) \
		void WidgetFactory::Widget_##eventName(WidgetPtr _widget, const std::string &_key, const std::string &_value) \
		{ \
			DelegateManager & manager = DelegateManager::getInstance(); \
			_widget->setUserString( #eventName, _value); \
			_widget->eventName = newDelegate(&manager, &DelegateManager::eventName); \
		}

namespace MyGUI
{
	namespace factory
	{

		WidgetFactory::WidgetFactory() :
			mNeedTranslate(false)
		{
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			LanguageManager::getInstance().eventChangeLanguage += newDelegate(this, &WidgetFactory::notifyChangeLanguage);

			// регестрируем все парсеры
			manager.registerDelegate("Widget_Caption") = newDelegate(this, &WidgetFactory::Widget_Caption);
			manager.registerDelegate("Widget_Position") = newDelegate(this, &WidgetFactory::Widget_Position);
			manager.registerDelegate("Widget_Size") = newDelegate(this, &WidgetFactory::Widget_Size);
			manager.registerDelegate("Widget_Coord") = newDelegate(this, &WidgetFactory::Widget_Coord);
			manager.registerDelegate("Widget_Visible") = newDelegate(this, &WidgetFactory::Widget_Visible);
			manager.registerDelegate("Widget_TextColour") = newDelegate(this, &WidgetFactory::Widget_TextColour);
			manager.registerDelegate("Widget_FontName") = newDelegate(this, &WidgetFactory::Widget_FontName);
			manager.registerDelegate("Widget_FontHeight") = newDelegate(this, &WidgetFactory::Widget_FontHeight);
			manager.registerDelegate("Widget_Alpha") = newDelegate(this, &WidgetFactory::Widget_Alpha);
			manager.registerDelegate("Widget_InheritsAlpha") = newDelegate(this, &WidgetFactory::Widget_InheritsAlpha);
			manager.registerDelegate("Widget_InheritsPick") = newDelegate(this, &WidgetFactory::Widget_InheritsPick);
			manager.registerDelegate("Widget_MaskPick") = newDelegate(this, &WidgetFactory::Widget_MaskPick);
			manager.registerDelegate("Widget_State") = newDelegate(this, &WidgetFactory::Widget_State);
			manager.registerDelegate("Widget_NeedKey") = newDelegate(this, &WidgetFactory::Widget_NeedKey);
			manager.registerDelegate("Widget_NeedMouse") = newDelegate(this, &WidgetFactory::Widget_NeedMouse);
			manager.registerDelegate("Widget_TextAlign") = newDelegate(this, &WidgetFactory::Widget_TextAlign);
			manager.registerDelegate("Widget_Enabled") = newDelegate(this, &WidgetFactory::Widget_Enabled);
			manager.registerDelegate("Widget_NeedToolTip") = newDelegate(this, &WidgetFactory::Widget_NeedToolTip);

			//OBSOLETE
			manager.registerDelegate("Widget_Show") = newDelegate(this, &WidgetFactory::Widget_Visible);
			manager.registerDelegate("Widget_Colour") = newDelegate(this, &WidgetFactory::Widget_TextColour);
			manager.registerDelegate("Widget_InheritsPeek") = newDelegate(this, &WidgetFactory::Widget_InheritsPick);
			manager.registerDelegate("Widget_MaskPeek") = newDelegate(this, &WidgetFactory::Widget_MaskPick);
			manager.registerDelegate("Widget_AlignText") = newDelegate(this, &WidgetFactory::Widget_TextAlign);

			// delegates
			manager.registerDelegate("Widget_eventMouseLostFocus") = newDelegate(this, &WidgetFactory::Widget_eventMouseLostFocus);
			manager.registerDelegate("Widget_eventMouseSetFocus") = newDelegate(this, &WidgetFactory::Widget_eventMouseSetFocus);
			manager.registerDelegate("Widget_eventMouseDrag") = newDelegate(this, &WidgetFactory::Widget_eventMouseDrag);
			manager.registerDelegate("Widget_eventMouseMove") = newDelegate(this, &WidgetFactory::Widget_eventMouseMove);
			manager.registerDelegate("Widget_eventMouseWheel") = newDelegate(this, &WidgetFactory::Widget_eventMouseWheel);
			manager.registerDelegate("Widget_eventMouseButtonPressed") = newDelegate(this, &WidgetFactory::Widget_eventMouseButtonPressed);
			manager.registerDelegate("Widget_eventMouseButtonReleased") = newDelegate(this, &WidgetFactory::Widget_eventMouseButtonReleased);
			manager.registerDelegate("Widget_eventMouseButtonClick") = newDelegate(this, &WidgetFactory::Widget_eventMouseButtonClick);
			manager.registerDelegate("Widget_eventMouseButtonDoubleClick") = newDelegate(this, &WidgetFactory::Widget_eventMouseButtonDoubleClick);
			//... many events still missed
			manager.registerDelegate("Widget_eventToolTip") = newDelegate(this, &WidgetFactory::Widget_eventToolTip);
		}

		WidgetFactory::~WidgetFactory()
		{
			MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
			LanguageManager::getInstance().eventChangeLanguage -= newDelegate(this, &WidgetFactory::notifyChangeLanguage);

			// удаляем все парсеры
			manager.unregisterDelegate("Widget_Caption");
			manager.unregisterDelegate("Widget_Position");
			manager.unregisterDelegate("Widget_Size");
			manager.unregisterDelegate("Widget_Coord");
			manager.unregisterDelegate("Widget_Visible");
			manager.unregisterDelegate("Widget_TextColour");
			manager.unregisterDelegate("Widget_FontName");
			manager.unregisterDelegate("Widget_FontHeight");
			manager.unregisterDelegate("Widget_Alpha");
			manager.unregisterDelegate("Widget_InheritsAlpha");
			manager.unregisterDelegate("Widget_InheritsPick");
			manager.unregisterDelegate("Widget_MaskPick");
			manager.unregisterDelegate("Widget_State");
			manager.unregisterDelegate("Widget_NeedKey");
			manager.unregisterDelegate("Widget_NeedMouse");
			manager.unregisterDelegate("Widget_TextAlign");
			manager.unregisterDelegate("Widget_Enabled");
			manager.unregisterDelegate("Widget_NeedToolTip");

			//OBSOLETE
			manager.unregisterDelegate("Widget_Show");
			manager.unregisterDelegate("Widget_Colour");
			manager.unregisterDelegate("Widget_InheritsPeek");
			manager.unregisterDelegate("Widget_MaskPeek");
			manager.unregisterDelegate("Widget_AlignText");

			// delegates
			manager.unregisterDelegate("Widget_eventMouseLostFocus");
			manager.unregisterDelegate("Widget_eventMouseSetFocus");
			manager.unregisterDelegate("Widget_eventMouseDrag");
			manager.unregisterDelegate("Widget_eventMouseMove");
			manager.unregisterDelegate("Widget_eventMouseWheel");
			manager.unregisterDelegate("Widget_eventMouseButtonPressed");
			manager.unregisterDelegate("Widget_eventMouseButtonReleased");
			manager.unregisterDelegate("Widget_eventMouseButtonClick");
			manager.unregisterDelegate("Widget_eventMouseButtonDoubleClick");
			//... many events still missed
			manager.unregisterDelegate("Widget_eventToolTip");
		}

		void WidgetFactory::Widget_Caption(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			// change '\n' on char 10
			size_t pos = _value.find("\\n");
			if (pos == std::string::npos) {
				_widget->setCaption(mNeedTranslate ? LanguageManager::getInstance().replaceTags(_value) : Ogre::UTFString(_value));
			}
			else {
				std::string value(_value);
				while (pos != std::string::npos) {
					value[pos++] = '\n';
					value.erase(pos, 1);
					pos = value.find("\\n");
				}
				_widget->setCaption(mNeedTranslate ? LanguageManager::getInstance().replaceTags(value) : Ogre::UTFString(value));
			}
		}

		void WidgetFactory::Widget_Position(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			IntPoint point;
			if (utility::parseComplex(_value, point.left, point.top)) _widget->setPosition(point);
		}

		void WidgetFactory::Widget_Size(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			IntSize size;
			if (utility::parseComplex(_value, size.width, size.height)) _widget->setSize(size);
		}

		void WidgetFactory::Widget_Coord(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			IntCoord coord;
			if (utility::parseComplex(_value, coord.left, coord.top, coord.width, coord.height)) _widget->setCoord(coord);
		}

		void WidgetFactory::Widget_Visible(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			_widget->setVisible(utility::parseBool(_value));
		}

		void WidgetFactory::Widget_TextColour(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			_widget->_setTextColour(Colour::parse(_value));
		}

		void WidgetFactory::Widget_FontName(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			_widget->_setFontName(_value);
		}

		void WidgetFactory::Widget_FontHeight(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			_widget->_setFontHeight(utility::parseUInt(_value));
		}

		void WidgetFactory::Widget_Alpha(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			_widget->setAlpha(utility::parseFloat(_value));
		}

		void WidgetFactory::Widget_State(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			_widget->setState(_value);
		}

		void WidgetFactory::Widget_NeedKey(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			_widget->setNeedKeyFocus(utility::parseBool(_value));
		}

		void WidgetFactory::Widget_NeedMouse(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			_widget->setNeedMouseFocus(utility::parseBool(_value));
		}

		void WidgetFactory::Widget_TextAlign(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			_widget->_setTextAlign(Align::parse(_value));
		}

		void WidgetFactory::Widget_Enabled(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			_widget->setEnabled(utility::parseBool(_value));
		}

		void WidgetFactory::Widget_InheritsAlpha(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			_widget->setInheritsAlpha(utility::parseBool(_value));
		}

		void WidgetFactory::Widget_InheritsPick(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			_widget->setInheritsPick(utility::parseBool(_value));
		}

		void WidgetFactory::Widget_MaskPick(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			_widget->setMaskPick(_value);
		}

		void WidgetFactory::Widget_NeedToolTip(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			_widget->setNeedToolTip(utility::parseBool(_value));
		}

		FACTORY_WIDGET_EVENT_DELEGATE(eventMouseLostFocus);
		FACTORY_WIDGET_EVENT_DELEGATE(eventMouseSetFocus);
		FACTORY_WIDGET_EVENT_DELEGATE(eventMouseDrag);
		FACTORY_WIDGET_EVENT_DELEGATE(eventMouseMove);
		FACTORY_WIDGET_EVENT_DELEGATE(eventMouseWheel);
		FACTORY_WIDGET_EVENT_DELEGATE(eventMouseButtonPressed);
		FACTORY_WIDGET_EVENT_DELEGATE(eventMouseButtonReleased);
		FACTORY_WIDGET_EVENT_DELEGATE(eventMouseButtonClick);
		FACTORY_WIDGET_EVENT_DELEGATE(eventMouseButtonDoubleClick);
		//... many events still missed
		FACTORY_WIDGET_EVENT_DELEGATE(eventToolTip);

		/*void WidgetFactory::Widget_eventMouseMove(WidgetPtr _widget, const std::string &_key, const std::string &_value)
		{
			DelegateManager & manager = DelegateManager::getInstance();
			_widget->setUserString("eventMouseMove", _value);
			_widget->eventMouseMove = newDelegate(&manager, &DelegateManager::eventMouseMove);
		}*/

		void WidgetFactory::notifyChangeLanguage(const std::string & _language)
		{
			mNeedTranslate = true;
		}

	} // namespace factory
} // namespace MyGUI
