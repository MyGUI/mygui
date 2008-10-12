/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_I_WIDGET_FACTORY_H__
#define __MYGUI_I_WIDGET_FACTORY_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Align.h"
#include "MyGUI_WidgetDefines.h"
#include "MyGUI_ICroppedRectangle.h"

namespace MyGUI
{
	class _MyGUIExport IWidgetFactory
	{
	public:
		virtual ~IWidgetFactory() { }

		virtual const std::string & getTypeName() = 0;
		virtual WidgetPtr createWidget(const std::string& _skin, const IntCoord& _coord, Align _align, ICroppedRectangle * _parent, IWidgetCreator * _creator, const std::string& _name) = 0;
	};

	namespace factory
	{

		template <typename T>
		class _MyGUIExport BaseWidgetFactory : public IWidgetFactory
		{
		public:
			BaseWidgetFactory();
			virtual ~BaseWidgetFactory();

			const std::string & getTypeName();
			WidgetPtr createWidget(const std::string& _skin, const IntCoord& _coord, Align _align, ICroppedRectangle * _parent, IWidgetCreator * _creator, const std::string& _name);
			bool isFalseType(WidgetPtr _ptr, const std::string &_key);
		};

	} // namespace factory
} // namespace MyGUI

#include "MyGUI_IWidgetFactory.cpp"

#endif // __MYGUI_I_WIDGET_FACTORY_H__
