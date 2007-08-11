#pragma once

#include <Ogre.h>
#include "BasisWidget.h"

namespace widget
{

	class BasisWidgetFactory
	{
	public:
		virtual const Ogre::String & getType() = 0;
		virtual BasisWidget * createBasisWidget(const tagBasisWidgetInfo &_info, const String & _material, BasisWidget * _parent) = 0;
	}; // BasisWidgetFactory

} // namespace widget