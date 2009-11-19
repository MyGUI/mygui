/*!
	@file
	@author		Albert Semenov
	@date		1/2009
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
#include "precompiled.h"
#include "PointerManager.h"
#include <MyGUI.h>

namespace input
{

	size_t g_pointer_sizeall = (size_t)::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEALL));
	size_t g_pointer_sizens = (size_t)::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENS));
	size_t g_pointer_sizewe = (size_t)::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZEWE));
	size_t g_pointer_sizenesw = (size_t)::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENESW));
	size_t g_pointer_sizenwse = (size_t)::LoadCursor(NULL, MAKEINTRESOURCE(IDC_SIZENWSE));
	size_t g_pointer_arrow = (size_t)::LoadCursor(NULL, MAKEINTRESOURCE(IDC_ARROW));
	size_t g_pointer_beam = (size_t)::LoadCursor(NULL, MAKEINTRESOURCE(IDC_IBEAM));
	size_t g_pointer_link = (size_t)::LoadCursor(NULL, MAKEINTRESOURCE(IDC_HAND));

	PointerManager::PointerManager() :
		mHwnd(0)
	{
	}

	PointerManager::~PointerManager()
	{
	}

	void PointerManager::createPointerManager(size_t _handle)
	{
		mHwnd = _handle;

		MyGUI::PointerManager& manager = MyGUI::PointerManager::getInstance();
		manager.setVisible(false);
		manager.eventChangeMousePointer += MyGUI::newDelegate(this, &PointerManager::notifyChangeMousePointer);

		// забиваем карту маппинга на стандартные курсоры
		mMapGuiPointer["arrow"] = g_pointer_arrow;
		mMapGuiPointer["beam"] = g_pointer_beam;
		mMapGuiPointer["size_left"] = g_pointer_sizenwse;
		mMapGuiPointer["size_right"] = g_pointer_sizenesw;
		mMapGuiPointer["size_horz"] = g_pointer_sizewe;
		mMapGuiPointer["size_vert"] = g_pointer_sizens;
		mMapGuiPointer["hand"] = g_pointer_sizeall;
		mMapGuiPointer["link"] = g_pointer_link;
	}

	void PointerManager::destroyPointerManager()
	{
		MyGUI::PointerManager& manager = MyGUI::PointerManager::getInstance();
		manager.eventChangeMousePointer -= MyGUI::newDelegate(this, &PointerManager::notifyChangeMousePointer);
	}

	void PointerManager::setPointerVisible(bool _value)
	{
		ShowCursor(_value);
	}

	void PointerManager::notifyChangeMousePointer(const std::string& _name)
	{
		//if (mManagerPointer)
		{
			MapPointer::iterator iter = mMapGuiPointer.find(_name);
			if (iter != mMapGuiPointer.end())
				setPointerHandle(iter->second);
		}
	}

	void PointerManager::setPointerHandle(size_t _id)
	{
		SetClassLong((HWND)mHwnd, GCL_HCURSOR, (LONG)_id);
		if ((GetCapture() == (HWND)mHwnd)
			|| isMouseInClient())
		{
			::SetCursor((HCURSOR)_id);
		}
	}

	bool PointerManager::isMouseInClient()
	{
		POINT point = { 0, 0 };
		ClientToScreen((HWND)mHwnd, &point);
		// x и y всегда 0
		RECT client_rect = { 0, 0, 0, 0 };
		GetClientRect((HWND)mHwnd, &client_rect);

		POINT cursor_point = { 0, 0 };
		GetCursorPos(&cursor_point);

		bool hor = cursor_point.x >= point.x && cursor_point.x < (point.x + client_rect.right);
		bool ver = cursor_point.y >= point.y && cursor_point.y < (point.y + client_rect.bottom);

		return hor && ver;
	}

} // namespace input
