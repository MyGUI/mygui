/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_WIDGET_DEFINES_H__
#define __MYGUI_WIDGET_DEFINES_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Types.h"
#include "MyGUI_AlignInfo.h"
#include <OgreColourValue.h>

namespace MyGUI
{

	struct SubWidgetStateInfo
	{
		SubWidgetStateInfo() :
			colour(Ogre::ColourValue::ZERO),
			alpha(-1)
		{
		}

		SubWidgetStateInfo(const FloatRect& _offset, const Ogre::ColourValue & _colour, float _alpha) :
			offset(_offset),
			colour(_colour),
			alpha(_alpha)
		{
		}

		FloatRect offset;
		Ogre::ColourValue colour;
		float alpha;
	};

	struct WidgetStateInfo
	{
		WidgetStateInfo() :
			colour(Ogre::ColourValue::ZERO),
			alpha(-1)
		{
		}

		std::vector<FloatRect> offsets;
		Ogre::ColourValue colour;
		float alpha;
	};

	struct SubWidgetInfo
	{
		SubWidgetInfo(const std::string & _type, const IntCoord& _coord, Align _align) :
			coord(_coord),
			align(_align),
			type(_type)
		{
		}

		IntCoord coord;
		Align align;
		std::string type;
	};

	typedef std::map<std::string, SubWidgetStateInfo> MapSubWidgetStateInfo;
	typedef std::map<std::string, WidgetStateInfo> MapWidgetStateInfo;
	typedef std::vector<SubWidgetInfo> VectorSubWidgetInfo;

	class WidgetSkinInfo;
	typedef WidgetSkinInfo * WidgetSkinInfoPtr;
	typedef std::map<std::string, WidgetSkinInfoPtr> MapWidgetSkinInfoPtr;

	class CroppedRectangleInterface;
	typedef CroppedRectangleInterface * CroppedRectanglePtr;
	typedef std::vector<CroppedRectanglePtr> VectorCroppedRectanglePtr;

	class Widget;
	typedef Widget * WidgetPtr;
	typedef std::vector<WidgetPtr> VectorWidgetPtr;
	typedef std::map<std::string, WidgetPtr> MapWidgetPtr;

	class SubWidgetTextInterface;
	typedef SubWidgetTextInterface * SubWidgetTextInterfacePtr;

} // namespace MyGUI

#endif // __MYGUI_WIDGET_DEFINES_H__
