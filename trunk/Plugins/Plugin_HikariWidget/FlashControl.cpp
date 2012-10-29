/*
	This file is part of Hikari, a library that allows developers
	to use Flash in their Ogre3D applications.

	Copyright (C) 2008 Adam J. Simmons

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

#include "FlashControl.h"
#include "FlashSite.h"
#include "FlashHandler.h"
#include "RenderBuffer.h"
#include "HikariWidget.h"
#include <direct.h>
#include "Plugin.h"

using namespace Hikari;

std::string getCurrentWorkingDirectory()
{
	char currentPath[_MAX_PATH + 1] = { 0 };
#if MYGUI_PLATFORM == MYGUI_PLATFORM_WIN32
	::GetCurrentDirectoryA(MAX_PATH, currentPath);
#else
	getcwd(currentPath, _MAX_PATH);
#endif
	std::string workingDirectory = currentPath;

	return workingDirectory + "\\";
}

FlashControl::FlashControl(HikariWidget* _owner, HMODULE _lib) :
	mOwner(_owner),
	site(0),
	handler(0),
	flashInterface(0),
	oleObject(0),
	windowlessObject(0),
	width(0),
	height(0),
	state(0),
	comCount(0),
	mainContext(0),
	mainBitmap(0),
	mainBuffer(0),
	altContext(0),
	altBitmap(0),
	altBuffer(0),
	renderBuffer(0),
	isClean(true),
	isTotallyDirty(false),
	isTransparent(false),
	mIsInitialise(false)
{
	renderBuffer = new RenderBuffer(width, height);
	createControl(_lib);
	invalidateTotally();
}

FlashControl::~FlashControl()
{
	if (windowlessObject) windowlessObject->Release();
	if (flashInterface) flashInterface->Release();
	if (handler)
	{
		handler->Shutdown();
		handler->Release();
	}

	if (oleObject)
	{
		oleObject->Close(OLECLOSE_NOSAVE);
		oleObject->Release();
	}

	if (site) site->Release();

	if (comCount)
	{
		MYGUI_LOGGING(plugin::Plugin::LogSection, Warning, "Hikari::FlashControl is leaking COM objects!");
	}

	if (mainContext) ::DeleteDC(mainContext);
	if (mainBitmap) ::DeleteObject(mainBitmap);
	if (altContext) ::DeleteDC(altContext);
	if (altBitmap) ::DeleteObject(altBitmap);
	if (renderBuffer)
	{
		delete renderBuffer;
		renderBuffer = 0;
	}
}

void FlashControl::createControl(HMODULE _lib)
{
	site = new FlashSite();
	site->AddRef();
	site->Init(this);

	// Try to load from user-supplied Flash OCX first
	if (_lib)
	{
		typedef HRESULT (__stdcall * GetClassObject)(REFCLSID rclsid, REFIID riid, LPVOID * ppv);

		IClassFactory* factory = 0;
		GetClassObject getClassFunc = (GetClassObject)GetProcAddress(_lib, "DllGetClassObject");
		HRESULT result = getClassFunc(ShockwaveFlashObjects::CLSID_ShockwaveFlash, IID_IClassFactory, (void**)&factory);
		if (SUCCEEDED(result))
		{
			factory->CreateInstance(NULL, IID_IOleObject, (void**)&oleObject);
			factory->Release();
		}
	}

	// If we still don't have the object, try loading from registry
	if (!oleObject)
	{
		HRESULT result = CoCreateInstance(ShockwaveFlashObjects::CLSID_ShockwaveFlash, 0, CLSCTX_INPROC_SERVER, IID_IOleObject, (void**)&oleObject);
		if (FAILED(result))
		{
			MYGUI_EXCEPT("Unable to load the Flash ActiveX control.");
		}
	}

	IOleClientSite* clientSite = 0;
	site->QueryInterface(__uuidof(IOleClientSite), (void**)&clientSite);
	oleObject->SetClientSite(clientSite);
	clientSite->Release();

	IOleInPlaceObject* inPlaceObject = 0;
	oleObject->QueryInterface(__uuidof(IOleInPlaceObject), (LPVOID*)&inPlaceObject);

	if (inPlaceObject)
	{
		invalidateTotally();

		inPlaceObject->SetObjectRects(&dirtyBounds, &dirtyBounds);
		inPlaceObject->Release();
	}

	oleObject->QueryInterface(__uuidof(ShockwaveFlashObjects::IShockwaveFlash), (LPVOID*)&flashInterface);

	flashInterface->PutWMode("opaque");

	oleObject->DoVerb(OLEIVERB_INPLACEACTIVATE, 0, clientSite, 0, 0, 0);
	clientSite->Release();

	oleObject->QueryInterface(__uuidof(IOleInPlaceObjectWindowless), (LPVOID*)&windowlessObject);

	handler = new FlashHandler();
	handler->AddRef();
	handler->Init(this);

	IViewObject* curView = 0;
	flashInterface->QueryInterface(IID_IViewObject, (void**)&curView);
}

void FlashControl::load(const std::string& movieFilename)
{
	mIsInitialise = false;

	const std::string& full_path = MyGUI::DataManager::getInstance().getDataPath(movieFilename);
	if (full_path.empty())
	{
		MYGUI_LOGGING(plugin::Plugin::LogSection, Error, "Could not load '" << movieFilename << "', the file was not found.");
		return;
	}

	// работают только абсолютные пути c левыми слешами
	std::string absolute_path = full_path.at(0) == '.' ? getCurrentWorkingDirectory() + full_path : full_path;
	for (size_t index = 0; index < absolute_path.size(); ++index)
	{
		if (absolute_path[index] == '/')
			absolute_path[index] = '\\';
	}

	flashInterface->PutMovie(absolute_path.c_str());

	mIsInitialise = true;
}

void FlashControl::setTransparent(bool isTransparent)
{
	this->isTransparent = isTransparent;
	if (isTransparent)
	{
		flashInterface->PutWMode("transparent");
	}
	else
	{
		flashInterface->PutWMode("opaque");
	}

	invalidateTotally();
}

void FlashControl::setQuality(short renderQuality)
{
	switch (renderQuality)
	{
	case RQ_LOW:
		flashInterface->PutQuality2("low");
		break;
	case RQ_MEDIUM:
		flashInterface->PutQuality2("medium");
		break;
	case RQ_HIGH:
		flashInterface->PutQuality2("high");
		break;
	case RQ_BEST:
		flashInterface->PutQuality2("best");
		break;
	case RQ_AUTOLOW:
		flashInterface->PutQuality2("autolow");
		break;
	case RQ_AUTOHIGH:
		flashInterface->PutQuality2("autohigh");
		break;
	}
}

void FlashControl::setScaleMode(short scaleMode)
{
	flashInterface->PutScaleMode(scaleMode);
}

void FlashControl::bind(const MyGUI::UString& funcName, FlashDelegate::IDelegate* callback)
{
	if (funcName.empty() || callback == 0)
		return;

	delegateMap[funcName] = callback;
}

void FlashControl::unbind(const MyGUI::UString& funcName)
{
	DelegateIter i = delegateMap.find(funcName);
	if (i != delegateMap.end())
		delegateMap.erase(i);
}

FlashValue FlashControl::callFunction(MyGUI::UString funcName, const Arguments& args)
{
	BSTR returnVal = 0;
	flashInterface->raw_CallFunction(_bstr_t(serializeInvocation(funcName, args).c_str()), &returnVal);

	return deserializeValue((wchar_t*)returnVal);
}

void FlashControl::injectMouseMove(int xPos, int yPos)
{
	LRESULT result;
	windowlessObject->OnWindowMessage(WM_MOUSEMOVE, 0, MAKELPARAM(xPos, yPos), &result);
}

void FlashControl::injectMouseDown(int xPos, int yPos)
{
	LRESULT result;
	windowlessObject->OnWindowMessage(WM_LBUTTONDOWN, MK_LBUTTON, MAKELPARAM(xPos, yPos), &result);
}

void FlashControl::injectMouseUp(int xPos, int yPos)
{
	LRESULT result;
	windowlessObject->OnWindowMessage(WM_LBUTTONUP, 0, MAKELPARAM(xPos, yPos), &result);
}

#ifndef WM_MOUSEWHEEL
#	define WM_MOUSEWHEEL 0x020A
#endif

void FlashControl::injectMouseWheel(int relScroll, int xPos, int yPos)
{
	LRESULT result;
	windowlessObject->OnWindowMessage(WM_MOUSEWHEEL, MAKEWPARAM(0, relScroll), MAKELPARAM(xPos, yPos), &result);
}

void FlashControl::handleKeyEvent(UINT msg, WPARAM wParam, LPARAM lParam)
{
	LRESULT aResult;
	windowlessObject->OnWindowMessage(msg, wParam, lParam, &aResult);
}

void FlashControl::invalidateTotally()
{
	isClean = false;
	isTotallyDirty = true;
	dirtyBounds.left = 0;
	dirtyBounds.top = 0;
	dirtyBounds.right = width;
	dirtyBounds.bottom = height;
}

void FlashControl::handleFlashCall(const std::wstring& xmlString)
{
	std::wstring funcName;
	Arguments args;

	if (!deserializeInvocation(xmlString, funcName, args))
		return;

	DelegateIter i = delegateMap.find(funcName);
	if (i != delegateMap.end())
	{
		FlashValue retval = FlashValue();
		i->second(mOwner, args, retval);
		flashInterface->SetReturnValue(serializeValue(retval).c_str());
	}
}

void FlashControl::update()
{
	if (isClean)
		return;

	if (mOwner && !mOwner->getVisible())
		return;

	// очищаем текстуру
	if (!mIsInitialise)
	{
		if (mOwner)
		{
			MyGUI::uint8* destBuffer = static_cast<MyGUI::uint8*>(mOwner->lock());
			memset(destBuffer, 0, mOwner->getTextureRealWidth() * mOwner->getTextureRealHeight() * 4);
			mOwner->unlock();
		}
		return;
	}

	int dirtyWidth = dirtyBounds.right - dirtyBounds.left;
	int dirtyHeight = dirtyBounds.bottom - dirtyBounds.top;
	int dirtyBufSize = dirtyWidth * dirtyHeight * 4;

	static int lastDirtyWidth = 0;
	static int lastDirtyHeight = 0;

	IViewObject* curView = 0;
	flashInterface->QueryInterface(IID_IViewObject, (void**)&curView);

	if (!oleObject || !curView)
		return;

	if (!mainContext || dirtyWidth != lastDirtyWidth || dirtyHeight != lastDirtyHeight)
	{
		if (mainContext)
		{
			DeleteDC(mainContext);
			mainContext = 0;
		}
		if (mainBitmap)
		{
			DeleteObject(mainBitmap);
			mainBitmap = 0;
		}

		lastDirtyWidth = dirtyWidth;
		lastDirtyHeight = dirtyHeight;

		HDC hdc = GetDC(0);
		BITMAPINFOHEADER bih = { 0 };
		bih.biSize = sizeof(BITMAPINFOHEADER);
		bih.biBitCount = 32;
		bih.biCompression = BI_RGB;
		bih.biPlanes = 1;
		bih.biWidth = dirtyWidth;
		bih.biHeight = -dirtyHeight;
		mainContext = CreateCompatibleDC(hdc);
		mainBitmap = CreateDIBSection(hdc, (BITMAPINFO*)&bih, DIB_RGB_COLORS, (void**)&mainBuffer, 0, 0);
		SelectObject(mainContext, mainBitmap);

		if (altContext)
		{
			DeleteDC(altContext);
			altContext = 0;
		}
		if (altBitmap)
		{
			DeleteObject(altBitmap);
			altBitmap = 0;
		}

		altContext = CreateCompatibleDC(hdc);
		altBitmap = CreateDIBSection(hdc, (BITMAPINFO*)&bih, DIB_RGB_COLORS, (void**)&altBuffer, 0, 0);
		SelectObject(altContext, altBitmap);

		ReleaseDC(0, hdc);
	}

	RECT local;
	local.left = -dirtyBounds.left;
	local.top = -dirtyBounds.top;
	local.right = local.left + dirtyBounds.right;
	local.bottom = local.top + dirtyBounds.bottom;

	memset(mainBuffer, 0, dirtyBufSize);
	memset(altBuffer, 255, dirtyBufSize);

	OleDraw(curView, DVASPECT_TRANSPARENT, mainContext, &local);
	OleDraw(curView, DVASPECT_TRANSPARENT, altContext, &local);

	// We've rendered the dirty area twice: once on black and once
	// on white. Now we compare the red channels of each to determine
	// the alpha value of each pixel.
	BYTE* blackBuffer, *whiteBuffer;
	blackBuffer = mainBuffer;
	whiteBuffer = altBuffer;
	BYTE blackRed, whiteRed;
	int size = dirtyWidth * dirtyHeight;
	for (int i = 0; i < size; i++)
	{
		blackRed = *blackBuffer;
		whiteRed = *whiteBuffer;
		blackBuffer += 3;
		whiteBuffer += 4;
		*blackBuffer++ = 255 - (whiteRed - blackRed);
	}

	renderBuffer->copyArea(dirtyBounds, mainBuffer, dirtyWidth * 4);

	if (mOwner)
	{
		MyGUI::uint8* destBuffer = static_cast<MyGUI::uint8*>(mOwner->lock());
		renderBuffer->blitBGR(destBuffer, (int)mOwner->getTextureRealWidth() * 4, 4);
		mOwner->unlock();
	}

	isClean = true;
	isTotallyDirty = false;
}

void FlashControl::setSize(int _width, int _height)
{
	width = _width;
	height = _height;

	renderBuffer->reserve(width, height);

	IOleInPlaceObject* inPlaceObject = 0;
	oleObject->QueryInterface(__uuidof(IOleInPlaceObject), (LPVOID*)&inPlaceObject);

	if (inPlaceObject)
	{
		invalidateTotally();
		inPlaceObject->SetObjectRects(&dirtyBounds, &dirtyBounds);
		inPlaceObject->Release();
	}

}
