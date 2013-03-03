/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_MainSkin.h"
#include "MyGUI_RenderItem.h"
#include "MyGUI_LayerManager.h"
#include "MyGUI_CommonStateInfo.h"

namespace MyGUI
{

	MainSkin::MainSkin() :
		SubSkin()
	{
	}

	MainSkin::~MainSkin()
	{
	}

	void MainSkin::_setAlign(const IntSize& _oldsize)
	{
		mCurrentCoord.set(0, 0, mCroppedParent->getWidth(), mCroppedParent->getHeight());
		mAlign = Align::Stretch;

		Base::_setAlign(_oldsize);
	}

} // namespace MyGUI
