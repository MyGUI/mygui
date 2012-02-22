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

#ifndef __FlashSite_H__
#define __FlashSite_H__

#import "PROGID:ShockwaveFlash.ShockwaveFlash" named_guids

#ifndef max
#define max(a, b)	(((a) > (b)) ? (a) : (b))
#endif
#ifndef min
#define min(a, b)	(((a) < (b)) ? (a) : (b))
#endif

namespace Hikari
{

	class FlashSite : public IOleInPlaceSiteWindowless, public IOleClientSite
	{
	public:
		int	refCount;
		FlashControl* owner;

	public:
		FlashSite() : refCount(0), owner(0)
		{
		}

		virtual ~FlashSite()
		{
			if (owner)
				owner->comCount--;
		}

		void Init(FlashControl* owner)
		{
			this->owner = owner;
			owner->comCount++;
		}

		HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, LPVOID* ppv)
		{
			*ppv = 0;

			if (riid == IID_IUnknown)
			{
				*ppv = (IUnknown*)(IOleWindow*)this;
				AddRef();
				return S_OK;
			}
			else if (riid == IID_IOleWindow)
			{
				*ppv = (IOleWindow*)this;
				AddRef();
				return S_OK;
			}
			else if (riid == IID_IOleInPlaceSite)
			{
				*ppv = (IOleInPlaceSite*)this;
				AddRef();
				return S_OK;
			}
			else if (riid == IID_IOleInPlaceSiteEx)
			{
				*ppv = (IOleInPlaceSiteEx*)this;
				AddRef();
				return S_OK;
			}
			else if (riid == IID_IOleInPlaceSiteWindowless)
			{
				*ppv = (IOleInPlaceSiteWindowless*)this;
				AddRef();
				return S_OK;
			}
			else if (riid == IID_IOleClientSite)
			{
				*ppv = (IOleClientSite*)this;
				AddRef();
				return S_OK;
			}
			else
			{
				return E_NOTIMPL;
			}
		}

		ULONG STDMETHODCALLTYPE AddRef()
		{
			return ++refCount;
		}

		ULONG STDMETHODCALLTYPE Release()
		{
			--refCount;

			if (!refCount)
				delete this;

			return refCount;
		}

		virtual HRESULT STDMETHODCALLTYPE SaveObject(void)
		{
			return S_OK;
		}

		virtual HRESULT STDMETHODCALLTYPE GetMoniker(DWORD dwAssign, DWORD dwWhichMoniker, IMoniker** ppmk)
		{
			*ppmk = 0;
			return E_NOTIMPL;
		}

		virtual HRESULT STDMETHODCALLTYPE GetContainer(IOleContainer** theContainerP)
		{
			return E_NOINTERFACE;
		}

		virtual HRESULT  STDMETHODCALLTYPE ShowObject(void)
		{
			return E_NOTIMPL;
		}

		virtual HRESULT  STDMETHODCALLTYPE OnShowWindow(BOOL)
		{
			return E_NOTIMPL;
		}

		virtual HRESULT  STDMETHODCALLTYPE RequestNewObjectLayout(void)
		{
			return E_NOTIMPL;
		}

		HRESULT STDMETHODCALLTYPE ContextSensitiveHelp(BOOL fEnterMode)
		{
			return S_OK;
		}

		HRESULT STDMETHODCALLTYPE GetWindow(HWND __RPC_FAR* theWnndow)
		{
			return E_FAIL;
		}

		HRESULT STDMETHODCALLTYPE CanInPlaceActivate(void)
		{
			return S_OK;
		}

		HRESULT STDMETHODCALLTYPE OnInPlaceActivate(void)
		{
			return S_OK;
		}

		HRESULT STDMETHODCALLTYPE OnUIActivate(void)
		{
			return S_OK;
		}

		HRESULT STDMETHODCALLTYPE GetWindowContext(IOleInPlaceFrame __RPC_FAR* __RPC_FAR* ppFrame, IOleInPlaceUIWindow __RPC_FAR* __RPC_FAR* ppDoc,
			LPRECT lprcPosRect, LPRECT lprcClipRect, LPOLEINPLACEFRAMEINFO lpFrameInfo)
		{
			RECT rect;
			rect.left = 0;
			rect.top = 0;
			rect.right = owner->width;
			rect.bottom = owner->height;

			*lprcPosRect = rect;
			*lprcClipRect = rect;

			*ppFrame = 0;
			QueryInterface(__uuidof(IOleInPlaceFrame), (void**)ppFrame);
			*ppDoc = 0;

			lpFrameInfo->fMDIApp = FALSE;
			lpFrameInfo->hwndFrame = 0;
			lpFrameInfo->haccel = 0;
			lpFrameInfo->cAccelEntries = 0;

			return S_OK;
		}

		HRESULT STDMETHODCALLTYPE Scroll(SIZE scrollExtant)
		{
			return S_OK;
		}

		HRESULT STDMETHODCALLTYPE OnUIDeactivate(BOOL fUndoable)
		{
			return S_OK;
		}

		HRESULT STDMETHODCALLTYPE OnInPlaceDeactivate(void)
		{
			return S_OK;
		}

		HRESULT STDMETHODCALLTYPE DiscardUndoState(void)
		{
			return S_OK;
		}

		HRESULT STDMETHODCALLTYPE DeactivateAndUndo(void)
		{
			return S_OK;
		}

		HRESULT STDMETHODCALLTYPE OnPosRectChange(LPCRECT lprcPosRect)
		{
			return S_OK;
		}

		HRESULT STDMETHODCALLTYPE OnInPlaceActivateEx(BOOL __RPC_FAR* pfNoRedraw, DWORD dwFlags)
		{
			return S_OK;
		}

		HRESULT STDMETHODCALLTYPE OnInPlaceDeactivateEx(BOOL fNoRedraw)
		{
			return S_OK;
		}

		HRESULT STDMETHODCALLTYPE RequestUIActivate(void)
		{
			return S_FALSE;
		}

		HRESULT STDMETHODCALLTYPE CanWindowlessActivate(void)
		{
			return S_OK;
		}

		HRESULT STDMETHODCALLTYPE GetCapture(void)
		{
			return S_FALSE;
		}

		HRESULT STDMETHODCALLTYPE SetCapture(BOOL fCapture)
		{
			return S_FALSE;
		}

		HRESULT STDMETHODCALLTYPE GetFocus(void)
		{
			return S_OK;
		}

		HRESULT STDMETHODCALLTYPE SetFocus(BOOL fFocus)
		{
			return S_OK;
		}

		HRESULT STDMETHODCALLTYPE GetDC(LPCRECT pRect, DWORD grfFlags, HDC __RPC_FAR* phDC)
		{
			return E_INVALIDARG;
		}

		HRESULT STDMETHODCALLTYPE ReleaseDC(HDC hDC)
		{
			return E_INVALIDARG;
		}

		HRESULT STDMETHODCALLTYPE InvalidateRect(LPCRECT pRect, BOOL fErase)
		{
			if (owner->isTotallyDirty)
				return S_OK;

			if (pRect)
			{
				if (owner->isClean)
				{
					owner->dirtyBounds.left = pRect->left;
					owner->dirtyBounds.right = pRect->right;
					owner->dirtyBounds.top = pRect->top;
					owner->dirtyBounds.bottom = pRect->bottom;
					owner->isClean = false;
				}
				else
				{
					owner->dirtyBounds.left = min(owner->dirtyBounds.left, pRect->left);
					owner->dirtyBounds.right = max(owner->dirtyBounds.right, pRect->right);
					owner->dirtyBounds.top = min(owner->dirtyBounds.top, pRect->top);
					owner->dirtyBounds.bottom = max(owner->dirtyBounds.bottom, pRect->bottom);
				}
			}

			if (owner->dirtyBounds.left < 0 || owner->dirtyBounds.top < 0 ||
				owner->dirtyBounds.right > owner->width || owner->dirtyBounds.bottom > owner->height || !pRect)
			{
				owner->invalidateTotally();
			}

			return S_OK;
		}

		HRESULT STDMETHODCALLTYPE InvalidateRgn(HRGN hRGN, BOOL fErase)
		{
			owner->invalidateTotally();

			return S_OK;
		}

		HRESULT STDMETHODCALLTYPE ScrollRect(INT dx, INT dy, LPCRECT pRectScroll, LPCRECT pRectClip)
		{
			return S_OK;
		}

		HRESULT STDMETHODCALLTYPE AdjustRect(LPRECT prc)
		{
			if (!prc)
				return E_INVALIDARG;

			return S_OK;
		}

		HRESULT STDMETHODCALLTYPE OnDefWindowMessage(UINT msg, WPARAM wParam, LPARAM lParam, LRESULT __RPC_FAR* plResult)
		{
			return S_FALSE;
		}
	};

}

#endif
