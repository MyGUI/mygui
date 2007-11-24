#ifndef __WIDGET_DEFINES_H__
#define __WIDGET_DEFINES_H__

#include "Prerequest.h"
#include "Common.h"

namespace MyGUI
{

	struct BasisWidgetStateInfo
	{
		BasisWidgetStateInfo()
		{
		}

		BasisWidgetStateInfo(const FloatRect & _offset, const Ogre::ColourValue & _colour, float _alpha)
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

	struct BasisWidgetInfo
	{
		BasisWidgetInfo(const std::string & _type, const IntRect & _offset, Align _align) : type(_type), offset(_offset), align(_align)
		{
		}

		IntRect offset;
		Align align;
		std::string type;
	};

	typedef std::map<std::string, BasisWidgetStateInfo> MapBasisWidgetStateInfo;
	typedef std::map<std::string, WidgetStateInfo> MapWidgetStateInfo;
	typedef std::vector<BasisWidgetInfo> VectorBasisWidgetInfo;

	class WidgetSkinInfo;
	typedef WidgetSkinInfo * WidgetSkinInfoPtr;
	typedef std::map<std::string, WidgetSkinInfoPtr> MapWidgetSkinInfoPtr;

	class BasisWidget;
	typedef BasisWidget * BasisWidgetPtr;
	typedef std::vector<BasisWidgetPtr> VectorBasisWidgetPtr;

	class Widget;
	typedef Widget * WidgetPtr;
	typedef std::vector<WidgetPtr> VectorWidgetPtr;
	typedef std::map<std::string, WidgetPtr> MapWidgetPtr;

	typedef std::map<std::string, std::string> MapString;

} // namespace MyGUI

#endif // __WIDGET_DEFINES_H__