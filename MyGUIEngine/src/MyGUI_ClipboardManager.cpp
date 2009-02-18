/*!
	@file
	@author		Albert Semenov
	@date		11/2007
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
#include "MyGUI_Precompiled.h"
#include "MyGUI_ClipboardManager.h"
#include "MyGUI_Gui.h"
#include "MyGUI_TextIterator.h"

namespace MyGUI
{

	MYGUI_INSTANCE_IMPLEMENT(ClipboardManager);

	void ClipboardManager::initialise()
	{
		MYGUI_ASSERT(false == mIsInitialise, INSTANCE_TYPE_NAME << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		Ogre::RenderWindow * window = Gui::getInstance().getRenderWindow();
		if (window != nullptr) {
			window->getCustomAttribute("WINDOW", &mHwnd);
		}
#endif

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void ClipboardManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully shutdown");
		mIsInitialise = false;
	}

	void ClipboardManager::SetClipboardData(const std::string& _type, const std::string& _data)
	{
		mClipboardData[_type] = _data;

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		if (_type == "Text") {
			mPutTextInClipboard = TextIterator::getOnlyText(Ogre::UTFString(_data));
			size_t size = (mPutTextInClipboard.size() + 1) * 2;
			if (::OpenClipboard((HWND)mHwnd)) {//открываем буфер обмена
				::EmptyClipboard(); //очищаем буфер
				HGLOBAL hgBuffer = ::GlobalAlloc(GMEM_DDESHARE, size);//выделяем память
				wchar_t * chBuffer = NULL;
				if ((hgBuffer) && (chBuffer = (wchar_t*)GlobalLock(hgBuffer))) {
					::memcpy(chBuffer, mPutTextInClipboard.asWStr_c_str(), size);
					::GlobalUnlock(hgBuffer);//разблокируем память
					::SetClipboardData(CF_UNICODETEXT, hgBuffer);//помещаем текст в буфер обмена
				}
				::CloseClipboard(); //закрываем буфер обмена
			}
		}
#endif
	}

	void ClipboardManager::ClearClipboardData(const std::string& _type)
	{
		MapString::iterator iter = mClipboardData.find(_type);
		if (iter != mClipboardData.end()) mClipboardData.erase(iter);
	}

	std::string ClipboardManager::GetClipboardData(const std::string& _type)
	{
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
		if (_type == "Text") {
			Ogre::UTFString buff;
			if ( ::OpenClipboard((HWND)mHwnd) ) {//открываем буфер обмена
				HANDLE hData = ::GetClipboardData(CF_UNICODETEXT);//извлекаем текст из буфера обмена
				wchar_t * chBuffer = NULL;
				if ((hData) && (chBuffer = (wchar_t*)::GlobalLock(hData))) {
					buff = chBuffer;
					::GlobalUnlock(hData);//разблокируем память
				}
				::CloseClipboard();//закрываем буфер обмена
			}
			// если в буфере не то что мы ложили, то берем из буфера
			if (mPutTextInClipboard != buff) {
				// вставляем теги, если нуно
				const Ogre::UTFString & text = TextIterator::toTagsString(buff);
				return text.asUTF8();
			}

			MapString::iterator iter = mClipboardData.find(_type);
			if (iter != mClipboardData.end()) return (*iter).second;
			return "";
		}
#endif

		MapString::iterator iter = mClipboardData.find(_type);
		if (iter != mClipboardData.end()) return (*iter).second;
		return "";
	}

} // namespace MyGUI
