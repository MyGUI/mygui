#pragma once

#include <wincodec.h>

namespace MyGUI
{

	inline HRESULT saveWICImage(const wchar_t* _filename, UINT _width, UINT _height, UINT _stride, const BYTE* _pixels)
	{
		IWICImagingFactory* factory = nullptr;
		HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&factory));
		if (FAILED(hr))
			return hr;

		IWICBitmap* bitmap = nullptr;
		hr = factory->CreateBitmapFromMemory(
			_width,
			_height,
			GUID_WICPixelFormat32bppBGRA,
			_stride,
			_height * _stride,
			const_cast<BYTE*>(_pixels),
			&bitmap);
		if (FAILED(hr))
		{
			factory->Release();
			return hr;
		}

		IWICStream* stream = nullptr;
		hr = factory->CreateStream(&stream);
		if (FAILED(hr))
		{
			bitmap->Release();
			factory->Release();
			return hr;
		}

		hr = stream->InitializeFromFilename(_filename, GENERIC_WRITE);
		if (FAILED(hr))
		{
			stream->Release();
			bitmap->Release();
			factory->Release();
			return hr;
		}

		IWICBitmapEncoder* encoder = nullptr;
		hr = factory->CreateEncoder(GUID_ContainerFormatPng, nullptr, &encoder);
		if (FAILED(hr))
		{
			stream->Release();
			bitmap->Release();
			factory->Release();
			return hr;
		}

		hr = encoder->Initialize(stream, WICBitmapEncoderNoCache);
		if (FAILED(hr))
		{
			encoder->Release();
			stream->Release();
			bitmap->Release();
			factory->Release();
			return hr;
		}

		IWICBitmapFrameEncode* frame = nullptr;
		IPropertyBag2* props = nullptr;
		hr = encoder->CreateNewFrame(&frame, &props);
		if (SUCCEEDED(hr))
		{
			hr = frame->Initialize(props);
			if (SUCCEEDED(hr))
			{
				hr = frame->SetSize(_width, _height);
				if (SUCCEEDED(hr))
				{
					WICPixelFormatGUID fmt = GUID_WICPixelFormat32bppBGRA;
					hr = frame->SetPixelFormat(&fmt);
				}
				if (SUCCEEDED(hr))
					hr = frame->WriteSource(bitmap, nullptr);
				if (SUCCEEDED(hr))
					hr = frame->Commit();
			}
			frame->Release();
			props->Release();
		}
		hr = encoder->Commit();

		encoder->Release();
		stream->Release();
		bitmap->Release();
		factory->Release();
		return hr;
	}

} // namespace MyGUI
