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

#include "KeyboardHook.h"

LRESULT CALLBACK GetMessageProc(int nCode, WPARAM wParam, LPARAM lParam);

KeyboardHook* KeyboardHook::instance = 0;

KeyboardHook::KeyboardHook(HookListener* listener) : listener(listener)
{
	instance = this;

	HINSTANCE hInstance = GetModuleHandle(0);

	getMsgHook = SetWindowsHookEx(WH_GETMESSAGE, GetMessageProc, hInstance, GetCurrentThreadId());
}

KeyboardHook::~KeyboardHook()
{
	UnhookWindowsHookEx(getMsgHook);

	instance = 0;
}

void KeyboardHook::handleHook(UINT msg, HWND hwnd, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_KEYDOWN:
	case WM_KEYUP:
	case WM_CHAR:
	case WM_DEADCHAR:
	case WM_SYSKEYDOWN:
	case WM_SYSKEYUP:
	case WM_SYSDEADCHAR:
	case WM_SYSCHAR:
	case WM_IME_CHAR:
	case WM_IME_COMPOSITION:
	case WM_IME_COMPOSITIONFULL:
	case WM_IME_CONTROL:
	case WM_IME_ENDCOMPOSITION:
	case WM_IME_KEYDOWN:
	case WM_IME_KEYUP:
	case WM_IME_NOTIFY:
	case WM_IME_REQUEST:
	case WM_IME_SELECT:
	case WM_IME_SETCONTEXT:
	case WM_IME_STARTCOMPOSITION:
	case WM_HELP:
	case WM_CANCELMODE:
	{
		if (listener)
			listener->handleKeyMessage(hwnd, msg, wParam, lParam);

		break;
	}
	}
}

LRESULT CALLBACK GetMessageProc(int nCode, WPARAM wParam, LPARAM lParam)
{
	if (nCode == HC_ACTION)
	{
		MSG* msg = (MSG*)lParam;
		if (wParam & PM_REMOVE)
			KeyboardHook::instance->handleHook(msg->message, msg->hwnd, msg->wParam, msg->lParam);
	}

	return CallNextHookEx(KeyboardHook::instance->getMsgHook, nCode, wParam, lParam);
}
