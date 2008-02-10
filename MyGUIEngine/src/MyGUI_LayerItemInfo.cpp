/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/
#include "MyGUI_LayerItemInfo.h"
#include "MyGUI_LayerInfo.h"

namespace MyGUI
{

	LayerItemInfo::LayerItemInfo() :
		mOverlayInfo(0), mLayerInfo(0)
	{
	}

	LayerItemInfo::~LayerItemInfo()
	{
	}

	const std::string & LayerItemInfo::getLayerName()
	{
		return mLayerInfo->getName();
	}

} // namespace MyGUI
