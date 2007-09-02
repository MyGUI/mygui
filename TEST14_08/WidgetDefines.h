#pragma once

#include <Ogre.h>
#include "Types.h"


namespace widget
{

	enum WIDGET_ALIGN {

		ALIGN_NONE						= FLAG_NONE,					// 
		ALIGN_HCENTER					= FLAG_NONE,					// center the window horizontally
		ALIGN_VCENTER					= FLAG_NONE,					// center the window vertically
		ALIGN_CENTER					= ALIGN_HCENTER | ALIGN_VCENTER,		// center the window in the dead center
		ALIGN_CENTER_PARENT		    = FLAG(0),						// это скорее флаг, ставит выравнивание по центру отца, по умолчанию относительно текущего положения
		ALIGN_LEFT						= FLAG(1),						// justify left
		ALIGN_RIGHT					= FLAG(2),						// justify right
		ALIGN_HSTRETCH				    = ALIGN_LEFT | ALIGN_RIGHT,			// stretch to fill the entire parent window horizontally (?)
		ALIGN_TOP						= FLAG(3),						// align from the top
		ALIGN_BOTTOM					= FLAG(4),						// align from the bottom
		ALIGN_VSTRETCH				    = ALIGN_TOP | ALIGN_BOTTOM,			// stretch to fill the entire parent window vertically (?)
		ALIGN_STRETCH					= ALIGN_HSTRETCH | ALIGN_VSTRETCH,	 // stretch to fill the entire parent (?)
	};

	struct tagBasisWidgetStateInfo
	{
		tagBasisWidgetStateInfo() {};
		tagBasisWidgetStateInfo(floatRect _offset, Ogre::ColourValue  _color, float _alpha)
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
		tagBasisWidgetInfo(const std::string & _type, intRect _offset, float _align) : type(_type), offset(_offset), aligin(_align) {}
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