#pragma once

#include <vector>
#include <Ogre.h>


namespace widget
{

	//Bit flags done easy
	#define FLAG_NONE  0
	#define FLAG(num)  (1<<(num))

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

	typedef Ogre::TRect<int> INTRect;

	struct tagBasisWidgetStateInfo
	{
		tagBasisWidgetStateInfo() {};
		tagBasisWidgetStateInfo(Ogre::FloatRect _offset, Ogre::ColourValue  _color, float _alpha)
		{
			offset = _offset;
			color = _color;
			alpha = _alpha;
		}
		Ogre::FloatRect offset;
		Ogre::ColourValue color;
		float alpha;
	};
	struct tagWidgetStateInfo {
		tagWidgetStateInfo() : alpha(-1), color(Ogre::ColourValue::ZERO) {}
		std::vector<Ogre::FloatRect> m_offsets;
		Ogre::ColourValue color;
		float alpha;
	};
	struct tagBasisWidgetInfo {
		tagBasisWidgetInfo(const Ogre::String & _type, INTRect _offset, float _align) : type(_type), offset(_offset), aligin(_align) {}
		INTRect offset;
		char aligin;
		Ogre::String type;
	};

	typedef std::map<Ogre::String, tagBasisWidgetStateInfo> ViewInfo;
	typedef std::map<Ogre::String, tagWidgetStateInfo> StateInfo;
	typedef std::vector<tagBasisWidgetInfo> BasisInfo;

	typedef std::map<Ogre::String, Ogre::String> SkinParam;

	// обязательно указатель на WidgetSkinInfo, так как при добавлении может перераспределяться память
	class WidgetSkinInfo;
	typedef WidgetSkinInfo * WidgetSkinInfoPtr;
	typedef std::map<Ogre::String, WidgetSkinInfoPtr> SkinInfo;

	class BasisWidget;
	typedef BasisWidget * BasisWidgetPtr;
	typedef std::vector<BasisWidgetPtr> BasisChild;

	class Widget;
	typedef Widget * WidgetPtr;
	typedef std::vector<WidgetPtr> WidgetChild;
	typedef std::map<Ogre::String, WidgetPtr> mapWidgetPtr;

	typedef Ogre::OverlayElement * OverlayElementPtr;


} // namespace widget