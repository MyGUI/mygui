/*!
	@file
	@author		Albert Semenov
	@date		07/2008
	@module
*/

#ifndef BASE_CELL_VIEW_H_
#define BASE_CELL_VIEW_H_

#include <MyGUI.h>
#include "BaseLayout/BaseLayout.h"

namespace wraps
{
	template<typename DataType>
	class BaseCellView :
		public BaseLayout
	{
	public:
		typedef DataType Type;

	protected:
		BaseCellView(const std::string& _layout, MyGUI::Widget* _parent) :
			BaseLayout(_layout, _parent)
		{
		}
	};

} // namespace wraps

#endif // BASE_CELL_VIEW_H_
