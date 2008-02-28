/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_SUB_WIDGET_FACTORY_INTERFACE_H__
#define __MYGUI_SUB_WIDGET_FACTORY_INTERFACE_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_CroppedRectangleInterface.h"

namespace MyGUI
{

	class _MyGUIExport SubWidgetFactoryInterface
	{
	public:
		virtual const std::string & getType() = 0;
		virtual CroppedRectangleInterface * createSubWidget(const SubWidgetInfo &_info, CroppedRectangleInterface * _parent) = 0;
	};

	template <class ClassName>
	class CroppedRectangleFactory : public SubWidgetFactoryInterface
	{

	public:
		CroppedRectangleFactory(const std::string& _type) : mType(_type) {}

		const std::string & getType() {return mType;};

		CroppedRectangleInterface * createSubWidget(const SubWidgetInfo& _info, CroppedRectangleInterface* _parent)
		{
			return new ClassName(_info, _parent);
		}
	private:
		std::string mType;

	};

} // namespace MyGUI

#endif // __MYGUI_SUB_WIDGET_FACTORY_INTERFACE_H__
