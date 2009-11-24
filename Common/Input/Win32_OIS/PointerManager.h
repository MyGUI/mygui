/*!
	@file
	@author		Albert Semenov
	@date		11/2009
	@module
*/
/*
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
#ifndef __POINTER_MANAGER_H__
#define __POINTER_MANAGER_H__

#include "ResourceW32Pointer.h"

namespace input
{

	class PointerManager
	{
	public:
		PointerManager();
		virtual ~PointerManager();

		void createPointerManager(size_t _handle);
		void destroyPointerManager();

		void setPointerVisible(bool _value);
		void setPointerName(const std::string& _name);

	private:
		void notifyChangeMousePointer(const std::string& _name);
		void setPointerHandle(size_t _id);
		bool isMouseInClient();
		void setPointer(const std::string& _name);

	private:
		typedef std::map<std::string, size_t> MapPointer;
		MapPointer mMapGuiPointer;
		size_t mHwnd;
		bool mManagerPointer;
	};

} // namespace input

#endif // __POINTER_MANAGER_H__
