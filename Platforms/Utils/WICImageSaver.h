#pragma once

#include <wincodec.h>
#include <memory>

namespace MyGUI
{

	inline HRESULT saveWICImage(const wchar_t* _filename, UINT _width, UINT _height, UINT _stride, const BYTE* _pixels)
	{
		const auto release = [](auto* object)
		{
			object->Release();
		};
		IWICImagingFactory* factory = nullptr;
		HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&factory));
		std::unique_ptr<IWICImagingFactory, decltype(release)> factoryOwner(factory, release);
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
		std::unique_ptr<IWICBitmap, decltype(release)> bitmapOwner(bitmap, release);
		if (FAILED(hr))
			return hr;

		IWICStream* stream = nullptr;
		hr = factory->CreateStream(&stream);
		std::unique_ptr<IWICStream, decltype(release)> streamOwner(stream, release);
		if (FAILED(hr))
			return hr;

		hr = stream->InitializeFromFilename(_filename, GENERIC_WRITE);
		if (FAILED(hr))
			return hr;

		IWICBitmapEncoder* encoder = nullptr;
		hr = factory->CreateEncoder(GUID_ContainerFormatPng, nullptr, &encoder);
		std::unique_ptr<IWICBitmapEncoder, decltype(release)> encoderOwner(encoder, release);
		if (FAILED(hr))
			return hr;

		hr = encoder->Initialize(stream, WICBitmapEncoderNoCache);
		if (FAILED(hr))
			return hr;

		IWICBitmapFrameEncode* frame = nullptr;
		IPropertyBag2* props = nullptr;
		hr = encoder->CreateNewFrame(&frame, &props);
		std::unique_ptr<IWICBitmapFrameEncode, decltype(release)> frameOwner(frame, release);
		std::unique_ptr<IPropertyBag2, decltype(release)> propsOwner(props, release);
		if (FAILED(hr))
			return hr;
		hr = frame->Initialize(props);
		if (FAILED(hr))
			return hr;
		hr = frame->SetSize(_width, _height);
		if (FAILED(hr))
			return hr;
		WICPixelFormatGUID fmt = GUID_WICPixelFormat32bppBGRA;
		hr = frame->SetPixelFormat(&fmt);
		if (FAILED(hr))
			return hr;
		hr = frame->WriteSource(bitmap, nullptr);
		if (FAILED(hr))
			return hr;
		hr = frame->Commit();
		return FAILED(hr) ? hr : encoder->Commit();
	}

} // namespace MyGUI
