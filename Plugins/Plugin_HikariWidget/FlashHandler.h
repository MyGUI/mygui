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

#ifndef FlashHandler_H_
#define FlashHandler_H_

#import "PROGID:ShockwaveFlash.ShockwaveFlash" named_guids
#include "FlashValue.h"
#include <sstream>

namespace Hikari
{

	inline void replaceAll(std::wstring& sourceStr, const std::wstring& replaceWhat, const std::wstring& replaceWith)
	{
		for (size_t i = sourceStr.find(replaceWhat); i != std::wstring::npos; i = sourceStr.find(replaceWhat, i + replaceWith.length()))
		{
			sourceStr.erase(i, replaceWhat.length());
			sourceStr.insert(i, replaceWith);
		}
	}

	std::wstring serializeValue(const FlashValue& value)
	{
		switch (value.getType())
		{
		case FT_NULL:
			return L"<null/>";
		case FT_BOOLEAN:
			return value.getBool() ? L"<true/>" : L"<false/>";
		case FT_NUMBER:
		{
			static std::wstringstream converter;
			converter.clear();
			converter.str(L"");
			converter << value.getNumber();

			return L"<number>" + converter.str() + L"</number>";
		}
		case FT_STRING:
		{
			std::wstring stringVal = value.getString();
			replaceAll(stringVal, L"&", L"&amp;");
			replaceAll(stringVal, L"\"", L"&quot;");
			replaceAll(stringVal, L"'", L"&apos;");
			replaceAll(stringVal, L"<", L"&lt;");
			replaceAll(stringVal, L">", L"&gt;");

			return L"<string>" + stringVal + L"</string>";
		}
		}

		return L"<null/>";
	}

	FlashValue deserializeValue(const std::wstring& valueStr)
	{
		if (valueStr == L"<null/>")
			return FlashValue();
		else if (valueStr == L"<true/>")
			return FlashValue(true);
		else if (valueStr == L"<false/>")
			return FlashValue(false);

		if (valueStr.substr(0, 8) == L"<string>")
		{
			std::wstring stringVal = valueStr.substr(8, valueStr.find(L"</string>", 8) - 8);
			replaceAll(stringVal, L"&quot;", L"\"");
			replaceAll(stringVal, L"&apos;", L"'");
			replaceAll(stringVal, L"&lt;", L"<");
			replaceAll(stringVal, L"&gt;", L">");
			replaceAll(stringVal, L"&amp;", L"&");
			return FlashValue(stringVal);
		}
		else if (valueStr.substr(0, 8) == L"<number>")
		{
			static std::wstringstream converter;
			converter.clear();
			float numValue = 0;

			converter.str(valueStr.substr(8, valueStr.find(L"</number>", 8) - 8));
			converter >> numValue;

			return FlashValue(numValue);
		}

		return FlashValue();
	}

	std::wstring serializeInvocation(const std::wstring& funcName, const Arguments& args)
	{
		std::wstring result;

		result += L"<invoke name=\"" + funcName + L"\" returntype=\"xml\">";

		if (!args.empty())
		{
			result += L"<arguments>";
			for (Arguments::const_iterator i = args.begin(); i != args.end(); ++i)
				result += serializeValue(*i);
			result += L"</arguments>";
		}

		result += L"</invoke>";

		return result;
	}

	bool deserializeInvocation(const std::wstring& xmlString, std::wstring& funcName, Arguments& args)
	{
		size_t indexA = 0;
		size_t indexB = 0;

		if ((indexA = xmlString.find(L"<invoke name=\"")) == std::wstring::npos)
			return false;

		if ((indexB = xmlString.find(L"\"", indexA + 14)) == std::wstring::npos)
			return false;

		funcName = xmlString.substr(indexA + 14, indexB - (indexA + 14));
		args.clear();

		if ((indexA = xmlString.find(L"<arguments>", indexB)) == std::wstring::npos)
			return true;

		indexA += 11;
		std::wstring argString(xmlString.substr(indexA, xmlString.find(L"</arguments>", indexA) - indexA));

		for (indexA = 0, indexB = 0; true;)
		{
			if ((indexA = argString.find(L"<", indexB)) == std::wstring::npos)
				break;

			if ((indexB = argString.find(L">", indexA)) == std::wstring::npos)
				break;

			if (argString[indexB-1] != L'/')
			{
				if ((indexB = argString.find(L">", indexB + 1)) == std::wstring::npos)
					break;
			}

			args.push_back(deserializeValue(argString.substr(indexA, indexB + 1 - indexA)));
		}

		return true;
	}

	class FlashHandler : public ShockwaveFlashObjects::_IShockwaveFlashEvents
	{
	public:
		LPCONNECTIONPOINT connectionPoint;
		DWORD cookie;
		int refCount;
		FlashControl* owner;

	public:
		FlashHandler() : cookie(0), connectionPoint(0), refCount(0), owner(0)
		{
		}

		virtual ~FlashHandler()
		{
			owner->comCount--;
		}

		HRESULT Init(FlashControl* owner)
		{
			this->owner = owner;
			owner->comCount++;

			HRESULT result = NOERROR;
			LPCONNECTIONPOINTCONTAINER cPointContainer = 0;

			if ((owner->flashInterface->QueryInterface(IID_IConnectionPointContainer, (void**)&cPointContainer) == S_OK) &&
					(cPointContainer->FindConnectionPoint(__uuidof(ShockwaveFlashObjects::_IShockwaveFlashEvents), &connectionPoint) == S_OK))
			{
				IDispatch* dispatch = 0;
				QueryInterface(__uuidof(IDispatch), (void**)&dispatch);

				if (dispatch)
				{
					result = connectionPoint->Advise((LPUNKNOWN)dispatch, &cookie);
					dispatch->Release();
				}
			}

			if (cPointContainer)
				cPointContainer->Release();

			return result;
		}

		HRESULT Shutdown()
		{
			HRESULT result = S_OK;

			if (connectionPoint)
			{
				if (cookie)
				{
					result = connectionPoint->Unadvise(cookie);
					cookie = 0;
				}

				connectionPoint->Release();
				connectionPoint = 0;
			}

			return result;
		}

		HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, LPVOID* ppv)
		{
			*ppv = 0;

			if (riid == IID_IUnknown)
			{
				*ppv = (LPUNKNOWN)this;
				AddRef();
				return S_OK;
			}
			else if (riid == IID_IDispatch)
			{
				*ppv = (IDispatch*)this;
				AddRef();
				return S_OK;
			}
			else if (riid == __uuidof(ShockwaveFlashObjects::_IShockwaveFlashEvents))
			{
				*ppv = (ShockwaveFlashObjects::_IShockwaveFlashEvents*) this;
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

		virtual HRESULT STDMETHODCALLTYPE GetTypeInfoCount(UINT* pctinfo)
		{
			return E_NOTIMPL;
		}

		virtual HRESULT STDMETHODCALLTYPE GetTypeInfo(UINT iTInfo, LCID lcid, ITypeInfo** ppTInfo)
		{
			return E_NOTIMPL;
		}

		virtual HRESULT STDMETHODCALLTYPE GetIDsOfNames(REFIID riid, LPOLESTR* rgszNames, UINT cNames, LCID lcid, DISPID* rgDispId)
		{
			return E_NOTIMPL;
		}

		virtual HRESULT STDMETHODCALLTYPE Invoke(DISPID dispIdMember, REFIID riid, LCID lcid,
			WORD wFlags, ::DISPPARAMS __RPC_FAR* pDispParams, VARIANT __RPC_FAR* pVarResult,
			::EXCEPINFO __RPC_FAR* pExcepInfo, UINT __RPC_FAR* puArgErr)
		{
			switch (dispIdMember)
			{
			case 0x7a6:
				break;
			case 0x96:
				if ((pDispParams->cArgs == 2) && (pDispParams->rgvarg[0].vt == VT_BSTR) && (pDispParams->rgvarg[1].vt == VT_BSTR))
					FSCommand(pDispParams->rgvarg[1].bstrVal, pDispParams->rgvarg[0].bstrVal);
				break;
			case 0xC5:
				if ((pDispParams->cArgs == 1) && (pDispParams->rgvarg[0].vt == VT_BSTR))
					FlashCall(pDispParams->rgvarg[0].bstrVal);
				break;
			case DISPID_READYSTATECHANGE:
				break;
			default:
				return DISP_E_MEMBERNOTFOUND;
			}

			return NOERROR;
		}

		HRESULT OnReadyStateChange (long newState)
		{
			return S_OK;
		}

		HRESULT OnProgress(long percentDone )
		{
			return S_OK;
		}

		HRESULT FSCommand (_bstr_t command, _bstr_t args)
		{
			// TODO: Handle FSCommand

			return S_OK;
		}

		HRESULT FlashCall (_bstr_t request)
		{
			owner->handleFlashCall((wchar_t*)request);

			return S_OK;
		}
	};

}

#endif
