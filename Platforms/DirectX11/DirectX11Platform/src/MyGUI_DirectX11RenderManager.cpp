/*!
	@file
	@author		Ustinov Igor aka Igor', DadyaIgor
	@date		09/2011
*/
#pragma warning(push, 0)
#include <d3dx11.h>
#pragma warning(pop)
#include <D3Dcompiler.h>
#include "MyGUI_DirectX11RenderManager.h"
#include "MyGUI_DirectX11Texture.h"
#include "MyGUI_DirectX11VertexBuffer.h"
#include "MyGUI_DirectX11Diagnostic.h"
#include "MyGUI_Gui.h"
#include "MyGUI_Timer.h"
#include "MyGUI_DataManager.h"

namespace MyGUI
{

	static const D3D11_INPUT_ELEMENT_DESC vertexLayout[] = {
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	DirectX11RenderManager::DirectX11RenderManager() :
		mIsInitialise(false),
		mpD3DDevice(nullptr),
		mpD3DContext(nullptr),
		mUpdate(false),
		mSamplerState(nullptr),
		mDepthStencilState(nullptr),
		mBlendState(nullptr),
		mRasterizerState(nullptr)
	{
	}

	void DirectX11RenderManager::initialise(ID3D11Device* _device)
	{
		MYGUI_PLATFORM_ASSERT(!mIsInitialise, getClassTypeName() << " initialised twice");
		MYGUI_PLATFORM_LOG(Info, "* Initialise: " << getClassTypeName());

		mpD3DDevice = _device;
		_device->GetImmediateContext(&mpD3DContext);

		mVertexFormat = VertexColourType::ColourABGR;

		if (mpD3DDevice->GetFeatureLevel() == D3D_FEATURE_LEVEL_11_0)
		{
			mVertexProfile = "vs_5_0";
			mPixelProfile = "ps_5_0";
		}
		else if (mpD3DDevice->GetFeatureLevel() == D3D_FEATURE_LEVEL_10_1)
		{
			mVertexProfile = "vs_4_1";
			mPixelProfile = "ps_4_1";
		}
		else if (mpD3DDevice->GetFeatureLevel() == D3D_FEATURE_LEVEL_10_0)
		{
			mVertexProfile = "vs_4_0";
			mPixelProfile = "ps_4_0";
		}
		else if (mpD3DDevice->GetFeatureLevel() == D3D_FEATURE_LEVEL_9_3)
		{
			mVertexProfile = "vs_4_0_level_9_3";
			mPixelProfile = "ps_4_0_level_9_3";
		}
		else if (mpD3DDevice->GetFeatureLevel() == D3D_FEATURE_LEVEL_9_2)
		{
			mVertexProfile = "vs_4_0_level_9_1";
			mPixelProfile = "ps_4_0_level_9_1";
		}
		else if (mpD3DDevice->GetFeatureLevel() == D3D_FEATURE_LEVEL_9_1)
		{
			mVertexProfile = "vs_4_0_level_9_1";
			mPixelProfile = "ps_4_0_level_9_1";
		}

		// Get Current viewports
		memset(&mInfo, 0, sizeof(mInfo));
		UINT numViewports = 0;
		D3D11_VIEWPORT viewports[8];

		mpD3DContext->RSGetViewports(&numViewports, viewports);
		MYGUI_PLATFORM_ASSERT(numViewports == 0, getClassTypeName() << " 0 viewport sets");

		setViewSize((int)viewports[0].Width, (int)viewports[0].Height);


		// Create Blend State
		D3D11_BLEND_DESC blendDesc;
		blendDesc.AlphaToCoverageEnable = false;
		blendDesc.IndependentBlendEnable = false;
		blendDesc.RenderTarget[0].BlendEnable = true;
		blendDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
		blendDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
		blendDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;
		HRESULT hr = mpD3DDevice->CreateBlendState(&blendDesc, &mBlendState);
		MYGUI_PLATFORM_ASSERT(hr == S_OK, "Blend State Create failed!");

		// Create Depth Stencil State
		D3D11_DEPTH_STENCIL_DESC depthDesc;
		depthDesc.DepthEnable = false;
		depthDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
		depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		depthDesc.StencilEnable = false;
		depthDesc.StencilReadMask = depthDesc.StencilWriteMask = 0;
		depthDesc.BackFace.StencilDepthFailOp = depthDesc.BackFace.StencilFailOp = depthDesc.BackFace.StencilPassOp =
			D3D11_STENCIL_OP_KEEP;
		depthDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		depthDesc.FrontFace = depthDesc.BackFace;
		hr = mpD3DDevice->CreateDepthStencilState(&depthDesc, &mDepthStencilState);
		MYGUI_PLATFORM_ASSERT(hr == S_OK, "Depth Stencil State Create failed!");

		// Create Rasterizer State
		D3D11_RASTERIZER_DESC rastDesc;
		rastDesc.FillMode = D3D11_FILL_SOLID;
		rastDesc.CullMode = D3D11_CULL_NONE;
		rastDesc.FrontCounterClockwise = FALSE;
		rastDesc.DepthBias = 0;
		rastDesc.DepthBiasClamp = 0;
		rastDesc.SlopeScaledDepthBias = 0;
		rastDesc.ScissorEnable = FALSE;
		rastDesc.MultisampleEnable = FALSE;
		rastDesc.AntialiasedLineEnable = FALSE;
		hr = mpD3DDevice->CreateRasterizerState(&rastDesc, &mRasterizerState);
		MYGUI_PLATFORM_ASSERT(hr == S_OK, "Rasterizer State Create failed!");

		registerShader("Default", "MyGUI_DirectX11_VP.hlsl", "MyGUI_DirectX11_FP.hlsl");

		mUpdate = false;

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully initialized");
		mIsInitialise = true;
	}

	void DirectX11RenderManager::shutdown()
	{
		MYGUI_PLATFORM_ASSERT(mIsInitialise, getClassTypeName() << " is not initialised");
		MYGUI_PLATFORM_LOG(Info, "* Shutdown: " << getClassTypeName());

		destroyAllResources();

		if (mSamplerState)
			mSamplerState->Release();
		if (mBlendState)
			mBlendState->Release();
		if (mDepthStencilState)
			mDepthStencilState->Release();
		if (mRasterizerState)
			mRasterizerState->Release();
		if (mpD3DContext)
			mpD3DContext->Release();

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully shutdown");
		mIsInitialise = false;
	}

	IVertexBuffer* DirectX11RenderManager::createVertexBuffer()
	{
		return new DirectX11VertexBuffer(this);
	}

	void DirectX11RenderManager::destroyVertexBuffer(IVertexBuffer* _buffer)
	{
		delete _buffer;
	}

	void DirectX11RenderManager::doRender(IVertexBuffer* _buffer, ITexture* _texture, size_t _count)
	{
		DirectX11Texture* texture = static_cast<DirectX11Texture*>(_texture);
		MYGUI_ASSERT(
			_texture != nullptr && texture->mResourceView != nullptr,
			"Rendering without texture is not supported");

		DirectX11VertexBuffer* buffer = static_cast<DirectX11VertexBuffer*>(_buffer);

		mpD3DContext->PSSetSamplers(0, 1, &mSamplerState);
		mpD3DContext->PSSetShaderResources(0, 1, &texture->mResourceView);

		UINT stride = sizeof(Vertex), offset = 0;
		mpD3DContext->IASetVertexBuffers(0, 1, &buffer->mBuffer, &stride, &offset);

		if (texture->mShaderInfo != nullptr)
		{
			mpD3DContext->PSSetShader(texture->mShaderInfo->pixelShader, nullptr, 0);
			mpD3DContext->VSSetShader(texture->mShaderInfo->vertexShader, nullptr, 0);
			mpD3DContext->IASetInputLayout(texture->mShaderInfo->inputLayout);
		}

		mpD3DContext->Draw(_count, 0);

		if (texture->mShaderInfo != nullptr)
		{
			mpD3DContext->PSSetShader(mDefaultShader->pixelShader, nullptr, 0);
			mpD3DContext->VSSetShader(mDefaultShader->vertexShader, nullptr, 0);
			mpD3DContext->IASetInputLayout(mDefaultShader->inputLayout);
		}
	}

	void DirectX11RenderManager::drawOneFrame()
	{
		Gui* gui = Gui::getInstancePtr();
		if (gui == nullptr)
			return;

		static Timer timer;
		static unsigned long last_time = timer.getMilliseconds();
		unsigned long now_time = timer.getMilliseconds();
		unsigned long time = now_time - last_time;

		onFrameEvent(time / 1000.0f);

		last_time = now_time;

		begin();
		onRenderToTarget(this, mUpdate);
		end();

		mUpdate = false;
	}

	DirectX11ShaderInfo* DirectX11RenderManager::getShaderInfo(const std::string& _shaderName) const
	{
		auto iter = mRegisteredShaders.find(_shaderName);
		if (iter != mRegisteredShaders.end())
			return iter->second;
		MYGUI_PLATFORM_LOG(
			Error,
			"Failed to get shader info for shader '" << _shaderName << "'. Did you forgot to register shader?");
		return nullptr;
	}

	void DirectX11RenderManager::begin()
	{
		mpD3DContext->OMSetBlendState(mBlendState, nullptr, 0xffffffff);
		mpD3DContext->RSSetState(mRasterizerState);
		mpD3DContext->OMSetDepthStencilState(mDepthStencilState, 0);
		mpD3DContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		mpD3DContext->PSSetShader(mDefaultShader->pixelShader, nullptr, 0);
		mpD3DContext->VSSetShader(mDefaultShader->vertexShader, nullptr, 0);
		mpD3DContext->IASetInputLayout(mDefaultShader->inputLayout);
	}

	void DirectX11RenderManager::end()
	{
	}

	ITexture* DirectX11RenderManager::createTexture(const std::string& _name)
	{
		MapTexture::const_iterator item = mTextures.find(_name);
		MYGUI_PLATFORM_ASSERT(item == mTextures.end(), "Texture '" << _name << "' already exist");

		DirectX11Texture* texture = new DirectX11Texture(_name, this);
		mTextures[_name] = texture;
		return texture;
	}

	void DirectX11RenderManager::destroyTexture(ITexture* _texture)
	{
		if (_texture == nullptr)
			return;

		MapTexture::iterator item = mTextures.find(_texture->getName());
		MYGUI_PLATFORM_ASSERT(item != mTextures.end(), "Texture '" << _texture->getName() << "' not found");

		mTextures.erase(item);
		delete _texture;
	}

	ITexture* DirectX11RenderManager::getTexture(const std::string& _name)
	{
		MapTexture::const_iterator item = mTextures.find(_name);
		if (item == mTextures.end())
			return nullptr;
		return item->second;
	}

	bool DirectX11RenderManager::isFormatSupported(PixelFormat _format, TextureUsage _usage)
	{
		if (_format != PixelFormat::R8G8B8A8)
			return false;
		return true;
	}

	void DirectX11RenderManager::destroyAllResources()
	{
		for (MapTexture::const_iterator item = mTextures.begin(); item != mTextures.end(); ++item)
		{
			delete item->second;
		}
		mTextures.clear();

		for (const auto& shaderInfo : mRegisteredShaders)
		{
			destroyShader(shaderInfo.second);
		}
		mRegisteredShaders.clear();
	}

	void DirectX11RenderManager::setViewSize(int _width, int _height)
	{
		if (_height == 0)
			_height = 1;
		if (_width == 0)
			_width = 1;

		mViewSize.set(_width, _height);

		mInfo.maximumDepth = 0.0f;
		mInfo.hOffset = 0.0f; //-0.5f / float(mViewSize.width);
		mInfo.vOffset = 0.0f; //-0.5f / float(mViewSize.height);
		mInfo.aspectCoef = float(mViewSize.height) / float(mViewSize.width);
		mInfo.pixScaleX = 1.0f / float(mViewSize.width);
		mInfo.pixScaleY = 1.0f / float(mViewSize.height);

		onResizeView(mViewSize);

		mUpdate = true;
	}

	void DirectX11RenderManager::registerShader(
		const std::string& _shaderName,
		const std::string& _vertexProgramFile,
		const std::string& _fragmentProgramFile)
	{
		auto iter = mRegisteredShaders.find(_shaderName);
		if (iter != mRegisteredShaders.end())
			destroyShader(iter->second);
		mRegisteredShaders[_shaderName] = createShader(_shaderName, _vertexProgramFile, _fragmentProgramFile);
		if (_shaderName == "Default")
			mDefaultShader = mRegisteredShaders[_shaderName];
	}

	std::string DirectX11RenderManager::loadFileContent(const std::string& _file)
	{
		std::string fullPath = DataManager::getInstance().getDataPath(_file);
		if (fullPath.empty())
		{
			MYGUI_PLATFORM_LOG(Error, "Failed to load file content '" << _file << "'.");
			return {};
		}
		std::ifstream fileStream(DataManager::getInstance().getDataPath(_file));
		std::stringstream buffer;
		buffer << fileStream.rdbuf();
		return buffer.str();
	}

	DirectX11ShaderInfo* DirectX11RenderManager::createShader(
		const std::string& _shaderName,
		const std::string& _vertexProgramFile,
		const std::string& _fragmentProgramFile)
	{
		DirectX11ShaderInfo* shaderInfo = new DirectX11ShaderInfo();

		// Build Textured Vertex Shader
		ID3DBlob* bytecode = nullptr;
		ID3DBlob* errors = nullptr;
		ID3DBlob* signature = nullptr;

		UINT flags = D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_OPTIMIZATION_LEVEL3;

		std::string vertexShaderSource = loadFileContent(_vertexProgramFile);
		HRESULT hr = D3DCompile(
			vertexShaderSource.c_str(),
			vertexShaderSource.size(),
			(_shaderName + "VertexShader").c_str(),
			nullptr,
			nullptr,
			"main",
			mVertexProfile.c_str(),
			flags,
			0,
			&bytecode,
			&errors);
		MYGUI_PLATFORM_ASSERT(
			hr == S_OK,
			(errors ? (char*)errors->GetBufferPointer() : "Vertex Shader Compilation failed, unknown errors!"));

		hr = D3DGetInputSignatureBlob(bytecode->GetBufferPointer(), bytecode->GetBufferSize(), &signature);
		MYGUI_PLATFORM_ASSERT(
			hr == S_OK,
			(errors ? (char*)errors->GetBufferPointer()
					: "Vertex Shader Compilation failed, failed to get input signature!"));

		hr = mpD3DDevice->CreateVertexShader(
			bytecode->GetBufferPointer(),
			bytecode->GetBufferSize(),
			nullptr,
			&shaderInfo->vertexShader);
		MYGUI_PLATFORM_ASSERT(
			hr == S_OK,
			(errors ? (char*)errors->GetBufferPointer() : "Vertex Shader Create failed!"));

		if (bytecode)
			bytecode->Release();
		if (errors)
			errors->Release();


		// Build Textured Pixel Shader
		bytecode = nullptr;
		errors = nullptr;
		std::string pixelShaderSource = loadFileContent(_fragmentProgramFile);
		hr = D3DCompile(
			pixelShaderSource.c_str(),
			pixelShaderSource.size(),
			(_shaderName + "PixelShader").c_str(),
			nullptr,
			nullptr,
			"main",
			mPixelProfile.c_str(),
			flags,
			0,
			&bytecode,
			&errors);
		MYGUI_PLATFORM_ASSERT(
			hr == S_OK,
			(errors ? (char*)errors->GetBufferPointer() : "Pixel Shader Compilation failed, unknown errors!"));

		hr = mpD3DDevice->CreatePixelShader(
			bytecode->GetBufferPointer(),
			bytecode->GetBufferSize(),
			nullptr,
			&shaderInfo->pixelShader);
		MYGUI_PLATFORM_ASSERT(hr == S_OK, (errors ? (char*)errors->GetBufferPointer() : "Pixel Shader Create failed!"));

		if (bytecode)
			bytecode->Release();
		if (errors)
			errors->Release();

		// Create Sampler State
		D3D11_SAMPLER_DESC samplerDesc;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.BorderColor[0] = samplerDesc.BorderColor[1] = samplerDesc.BorderColor[2] =
			samplerDesc.BorderColor[3] = 0.0f;
		samplerDesc.ComparisonFunc = (D3D11_COMPARISON_FUNC)0;
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.MaxLOD = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MipLODBias = 0.0f;
		hr = mpD3DDevice->CreateSamplerState(&samplerDesc, &mSamplerState);
		MYGUI_PLATFORM_ASSERT(hr == S_OK, "Sampler State Create failed!");

		// Create Input Layout
		hr = mpD3DDevice->CreateInputLayout(
			vertexLayout,
			3,
			signature->GetBufferPointer(),
			signature->GetBufferSize(),
			&shaderInfo->inputLayout);
		MYGUI_PLATFORM_ASSERT(hr == S_OK, "Input Layout Create failed!");

		signature->Release();

		return shaderInfo;
	}

	void DirectX11RenderManager::destroyShader(DirectX11ShaderInfo* shaderInfo)
	{
		shaderInfo->vertexShader->Release();
		shaderInfo->pixelShader->Release();
		shaderInfo->inputLayout->Release();
		delete shaderInfo;
	}

} // namespace MyGUI
