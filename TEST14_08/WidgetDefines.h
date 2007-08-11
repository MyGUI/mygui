#pragma once

#include <vector>
#include <Ogre.h>


namespace widget
{

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

} // namespace widget