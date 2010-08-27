#ifndef _#{Uppercase_Panel_Name}_H_
#define _#{Uppercase_Panel_Name}_H_

#include "BaseLayout/BaseLayout.h"

ATTRIBUTE_CLASS_LAYOUT(#{Panel_Name}, "#{Panel_Name}.layout");
class #{Panel_Name} : public wraps::BaseLayout
{
public:
	#{Panel_Name}();
	virtual ~#{Panel_Name}();
private:
//%LE Widget_Declaration list start
//%LE Widget_Declaration list end
};

#endif // _#{Uppercase_Panel_Name}_H_