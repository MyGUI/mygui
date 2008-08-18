/*!
	@file
	@author		Albert Semenov
	@date		07/2008
	@module
*/
#ifndef __BASE_CELL_VIEW_H__
#define __BASE_CELL_VIEW_H__

#include <MyGUI.h>
#include "BaseLayout.h"

template<typename T>
class BaseCellView : public BaseLayout
{
public:
	typedef T Type;

protected:
	BaseCellView(const std::string & _layout) : BaseLayout(_layout) {}
};

#endif // __BASE_CELL_VIEW_H__
