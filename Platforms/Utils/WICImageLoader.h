#pragma once

#include <wincodec.h>
#include <limits>
#include <memory>

namespace MyGUI
{

	template<typename ReadPixels>
	HRESULT loadWICImage(const wchar_t* _filename, ReadPixels&& _readPixels)
	{
		struct ComInitialization
		{
			HRESULT result = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
			~ComInitialization()
			{
				if (SUCCEEDED(result))
					CoUninitialize();
			}
		} com;
		if (FAILED(com.result) && com.result != RPC_E_CHANGED_MODE)
			return com.result;

		const auto release = [](auto* _object)
		{
			_object->Release();
		};
		IWICImagingFactory* factory = nullptr;
		HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&factory));
		std::unique_ptr<IWICImagingFactory, decltype(release)> factoryOwner(factory, release);
		if (FAILED(hr))
			return hr;

		IWICBitmapDecoder* decoder = nullptr;
		hr = factory
				 ->CreateDecoderFromFilename(_filename, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder);
		std::unique_ptr<IWICBitmapDecoder, decltype(release)> decoderOwner(decoder, release);
		if (FAILED(hr))
			return hr;

		IWICBitmapFrameDecode* frame = nullptr;
		hr = decoder->GetFrame(0, &frame);
		std::unique_ptr<IWICBitmapFrameDecode, decltype(release)> frameOwner(frame, release);
		if (FAILED(hr))
			return hr;

		IWICBitmapSource* converter = nullptr;
		hr = WICConvertBitmapSource(GUID_WICPixelFormat32bppBGRA, frame, &converter);
		std::unique_ptr<IWICBitmapSource, decltype(release)> converterOwner(converter, release);
		if (FAILED(hr))
			return hr;
		UINT width = 0, height = 0;
		hr = converter->GetSize(&width, &height);
		if (FAILED(hr))
			return hr;
		if (width == 0 || height == 0 || width > std::numeric_limits<UINT>::max() / 4 / height)
			return E_INVALIDARG;
		return _readPixels(converter, width, height);
	}

} // namespace MyGUI
