#ifndef _#{Uppercase_Panel_Name}_H_
#define _#{Uppercase_Panel_Name}_H_

#include "BaseLayout/BaseLayout.h"

namespace #{Panel_Namespace}
{

	ATTRIBUTE_CLASS_LAYOUT(#{Panel_Name}, "#{Layout_Name}");
	class #{Panel_Name} :
		public wraps::BaseLayout
	{
	public:
		#{Panel_Name}(MyGUI::Widget* _parent = nullptr);
		virtual ~#{Panel_Name}();

	private:
	//%LE Widget_Declaration list start
	//%LE Widget_Declaration list end
	};

} // namespace #{Panel_Namespace}

#endif // _#{Uppercase_Panel_Name}_H_