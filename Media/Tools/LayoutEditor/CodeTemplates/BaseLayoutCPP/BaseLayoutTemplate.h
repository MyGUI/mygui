#ifndef __#{Uppercase_Panel_Name}_H__
#define __#{Uppercase_Panel_Name}_H__

#include <BaseLayout.h>

ATTRIBUTE_CLASS_LAYOUT(#{Panel_Name}, "#{Panel_Name}.layout");
class #{Panel_Name} : public wraps::BaseLayout
{
public:
	#{Panel_Name}();
	virtual ~#{Panel_Name}();
private:
//%LE Widget_Declaration list start
};

#endif // __#{Uppercase_Panel_Name}_H__
