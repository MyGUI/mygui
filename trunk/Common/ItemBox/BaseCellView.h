/*!
	@file
	@author		Albert Semenov
	@date		07/2008
	@module
*/
#ifndef __BASE_CELL_VIEW_H__
#define __BASE_CELL_VIEW_H__

#include <MyGUI.h>
#include "BaseLayout/BaseLayout.h"

namespace wraps
{

	template<typename DataType>
	class BaseCellView2 : public BaseLayout2
	{
	public:
		typedef DataType Type;

	protected:
		BaseCellView2(const std::string & _layout, MyGUI::WidgetPtr _parent) : BaseLayout2(_layout, _parent) { }

	};

	template<typename DataType>
	class BaseCellView : public BaseLayout
	{
	public:
		typedef DataType Type;

	protected:
		BaseCellView(const std::string & _layout) : BaseLayout(_layout) { }

	};

} // namespace wraps

#endif // __BASE_CELL_VIEW_H__
