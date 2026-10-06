/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "StaticRttiBridge.h"
#include "MyGUI_ISerializable.h"

MyGUI::IObject* createStaticRttiObject()
{
	return new MyGUI::ISerializable;
}
