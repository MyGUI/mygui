/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "StaticRttiBridge.h"
#include "MyGUI_ISerializable.h"

#include <cstdio>
#include <memory>
#include <typeinfo>

int main()
{
	std::unique_ptr<MyGUI::IObject> object(createStaticRttiObject());
	if (!object)
	{
		std::fputs("Static MyGUI factory returned null\n", stderr);
		return 1;
	}
	if (!object->isType<MyGUI::IObject>() || !object->isType<MyGUI::ISerializable>())
	{
		std::fputs("MyGUI isType failed across the shared library boundary\n", stderr);
		return 1;
	}
	const MyGUI::IObject& reference = *object;
	if (typeid(reference) != typeid(MyGUI::ISerializable))
	{
		std::fputs("MyGUI type_info differs across the shared library boundary\n", stderr);
		return 1;
	}
	if (dynamic_cast<MyGUI::ISerializable*>(object.get()) == nullptr)
	{
		std::fputs("MyGUI dynamic_cast failed across the shared library boundary\n", stderr);
		return 1;
	}
	return 0;
}
