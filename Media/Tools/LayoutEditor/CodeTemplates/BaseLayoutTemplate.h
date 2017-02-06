#pragma once

#include "BaseLayout/BaseLayout.h"

namespace #{Panel_Namespace}
{

ATTRIBUTE_CLASS_LAYOUT(#{Panel_Name}, "#{Layout_Name}");
class #{Panel_Name} : public wraps::BaseLayout
{
public:
	#{Panel_Name}(MyGUI::Widget* _parent = nullptr);

private:
//%LE Widget_Declaration list start
};

}