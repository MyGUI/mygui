#ifndef __KeyboardHook_H__
#define __KeyboardHook_H__

/*
	This file is part of NaviLibrary, a library that allows developers to create and
	interact with web-content as an overlay or material in Ogre3D applications.

	Copyright (C) 2009 Adam J. Simmons
	http://princeofcode.com/navilibrary.php

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <windows.h>

class HookListener
{
public:
	virtual void handleKeyMessage(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) = 0;
};

class KeyboardHook
{
public:
	HHOOK getMsgHook;
	static KeyboardHook* instance;
	HookListener* listener;

	KeyboardHook(HookListener* listener);
	~KeyboardHook();

	void handleHook(UINT msg, HWND hwnd, WPARAM wParam, LPARAM lParam);
};


#endif
