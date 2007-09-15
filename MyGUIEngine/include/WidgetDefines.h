#ifndef _WIDGETDEFINES_H_
#define _WIDGETDEFINES_H_

#include "Prerequest.h"
#include <Ogre.h>
#include "Common.h"


namespace widget
{

	struct tagBasisWidgetStateInfo
	{
		tagBasisWidgetStateInfo() {};
		tagBasisWidgetStateInfo(const floatRect & _offset, const Ogre::ColourValue & _color, float _alpha)
		{
			offset = _offset;
			color = _color;
			alpha = _alpha;
		}
		floatRect offset;
		Ogre::ColourValue color;
		float alpha;
	};
	struct tagWidgetStateInfo {
		tagWidgetStateInfo() : alpha(-1), color(Ogre::ColourValue::ZERO) {}
		std::vector<floatRect> m_offsets;
		Ogre::ColourValue color;
		float alpha;
	};
	struct tagBasisWidgetInfo {
		tagBasisWidgetInfo(const std::string & _type, const intRect & _offset, float _align) : type(_type), offset(_offset), aligin(_align) {}
		intRect offset;
		char aligin;
		std::string type;
	};

	typedef std::map<std::string, tagBasisWidgetStateInfo> ViewInfo;
	typedef std::map<std::string, tagWidgetStateInfo> StateInfo;
	typedef std::vector<tagBasisWidgetInfo> BasisInfo;
	typedef std::map<std::string, char> MapFlags;

	typedef std::map<std::string, std::string> SkinParam;

	// обязательно указатель на WidgetSkinInfo, так как при добавлении может перераспределяться память
	class WidgetSkinInfo;
	typedef WidgetSkinInfo * WidgetSkinInfoPtr;
	typedef std::map<std::string, WidgetSkinInfoPtr> SkinInfo;

	class BasisWidget;
	typedef BasisWidget * BasisWidgetPtr;
	typedef std::vector<BasisWidgetPtr> BasisChild;

	class Widget;
	typedef Widget * WidgetPtr;
	typedef std::vector<WidgetPtr> WidgetChild;
	typedef std::map<std::string, WidgetPtr> mapWidgetPtr;

	typedef Ogre::OverlayElement * OverlayElementPtr;

	typedef std::map<std::string, std::string> MapString;
	typedef std::map<std::string, int> MapInt;

} // namespace widget

#endif
