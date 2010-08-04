/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
/*
	This file is part of MyGUI.

	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#include "MyGUI_Precompiled.h"
#include "MyGUI_FilterNoneSkin.h"
#include "MyGUI_RenderItem.h"
#include "MyGUI_LayerManager.h"
#include "MyGUI_CommonStateInfo.h"

#ifdef MYGUI_OGRE_PLATFORM
#include <MyGUI_OgreRenderManager.h>
#endif

namespace MyGUI
{

	FilterNone::FilterNone() :
		SubSkin()
	{
		mSeparate = true;
	}

	FilterNone::~FilterNone()
	{
	}

	void FilterNone::_setAlign(const IntSize& _oldsize)
	{
		mCurrentCoord.set(0, 0, mCroppedParent->getWidth(), mCroppedParent->getHeight());
		mAlign = Align::Stretch;

		Base::_setAlign(_oldsize);
	}

	void FilterNone::preRender()
	{
#ifdef MYGUI_OGRE_PLATFORM
		OgreRenderManager::getInstance().setTextureFilter(Ogre::FO_NONE);
#endif
	}

	void FilterNone::postRender()
	{
#ifdef MYGUI_OGRE_PLATFORM
		OgreRenderManager::getInstance().setTextureFilter(Ogre::FO_LINEAR);
#endif
	}

} // namespace MyGUI
