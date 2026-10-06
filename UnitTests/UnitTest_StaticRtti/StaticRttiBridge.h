/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_STATIC_RTTI_BRIDGE_H_
#define MYGUI_STATIC_RTTI_BRIDGE_H_

#include "MyGUI_IObject.h"

// The bridge has its own public API, independent of the static engine's exports.
__attribute__((visibility("default"))) MyGUI::IObject* createStaticRttiObject();

#endif // MYGUI_STATIC_RTTI_BRIDGE_H_
