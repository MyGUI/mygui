/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_WIDGET_DEFINES_H__
#define __MYGUI_WIDGET_DEFINES_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Common.h"

namespace MyGUI
{

	struct CroppedRectangleStateInfo
	{
		CroppedRectangleStateInfo()
		{
		}

		CroppedRectangleStateInfo(const FloatRect & _offset, const Ogre::ColourValue & _colour, float _alpha)
		{
			offset = _offset;
			color = _colour;
			alpha = _alpha;
		}

		FloatRect offset;
		Ogre::ColourValue color;
		float alpha;
	};

	struct WidgetStateInfo
	{
		WidgetStateInfo() : alpha(-1), color(Ogre::ColourValue::ZERO)
		{
		}

		std::vector<FloatRect> offsets;
		Ogre::ColourValue color;
		float alpha;
	};

	struct CroppedRectangleInfo
	{
		CroppedRectangleInfo(const std::string & _type, const IntRect & _offset, Align _align) : type(_type), offset(_offset), align(_align)
		{
		}

		IntRect offset;
		Align align;
		std::string type;
	};

	typedef std::map<std::string, CroppedRectangleStateInfo> MapCroppedRectangleStateInfo;
	typedef std::map<std::string, WidgetStateInfo> MapWidgetStateInfo;
	typedef std::vector<CroppedRectangleInfo> VectorCroppedRectangleInfo;

	class WidgetSkinInfo;
	typedef WidgetSkinInfo * WidgetSkinInfoPtr;
	typedef std::map<std::string, WidgetSkinInfoPtr> MapWidgetSkinInfoPtr;

	class CroppedRectangleBase;
	typedef CroppedRectangleBase * CroppedRectanglePtr;
	typedef std::vector<CroppedRectanglePtr> VectorCroppedRectanglePtr;

	class Widget;
	typedef Widget * WidgetPtr;
	typedef std::vector<WidgetPtr> VectorWidgetPtr;
	typedef std::map<std::string, WidgetPtr> MapWidgetPtr;

	typedef std::map<std::string, std::string> MapString;

} // namespace MyGUI

#endif // __MYGUI_WIDGET_DEFINES_H__