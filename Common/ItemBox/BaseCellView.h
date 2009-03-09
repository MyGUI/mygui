/*!
	@file
	@author		Albert Semenov
	@date		07/2008
	@module
*//*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __BASE_CELL_VIEW_H__
#define __BASE_CELL_VIEW_H__

#include <MyGUI.h>
#include "BaseLayout/BaseLayout.h"

namespace wraps
{

	template<typename DataType>
	class BaseCellView : public BaseLayout
	{
	public:
		typedef DataType Type;

	protected:
		BaseCellView(const std::string & _layout, MyGUI::WidgetPtr _parent) : BaseLayout(_layout, _parent) { }

	};

} // namespace wraps

#endif // __BASE_CELL_VIEW_H__
