/*!
	@file
	@author		Ustinov Igor aka Igor', DadyaIgor
	@date		09/2011
*/
#include <d3dx11.h>
#include <D3Dcompiler.h>
#include "MyGUI_DirectX11RenderManager.h"
#include "MyGUI_DirectX11Texture.h"
#include "MyGUI_DirectX11VertexBuffer.h"
#include "MyGUI_DirectX11Diagnostic.h"
#include "MyGUI_Gui.h"
#include "MyGUI_Timer.h"

namespace MyGUI
{

	static const char vsSource[] =
	{
		"void main( in float3 inPosition : POSITION0, in float4 inColor : COLOR0, out float4 outPosition : SV_POSITION, out float4 outColor : TEXCOORD0 ) {\n"
		"	outPosition = float4(inPosition, 1.0f);\n"
		"	outColor = inColor;\n"
		"}\n"
	};

	static const char psSource[] =
	{
		"void main(  in float4 inPosition : SV_POSITION, in float4 inColor : TEXCOORD0, out float4 Out : SV_TARGET ) {\n"
		"	Out = inColor;\n"
		"}\n"
	};

	static const char vsTexturedSource[] =
	{
		"void main( in float3 inPosition : POSITION0, in float4 inColor : COLOR0, in float2 inTexcoord : TEXCOORD0, out float4 outPosition : SV_POSITION, out float4 outColor : TEXCOORD0, out float2 outTexcoord : TEXCOORD1 ) {\n"
		"	outPosition = float4(inPosition, 1.0f);\n"
		"	outColor = inColor;\n"
		"	outTexcoord = inTexcoord;\n"
		"}\n"
	};

	static const char psTexturedSource[] =
	{
		"void main( uniform Texture2D<float4> sampleTexture : register(t0), uniform SamplerState sampleSampler : register(s0), in float4 inPosition : SV_POSITION, in float4 inColor : TEXCOORD0, in float2 inTexcoord : TEXCOORD1, out float4 Out : SV_TARGET ) {\n"
		"	Out = sampleTexture.SampleLevel(sampleSampler, inTexcoord, 0).rgba * inColor;\n"
		"}\n"
	};

	static const D3D11_INPUT_ELEMENT_DESC vertexLayout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR",    0, DXGI_FORMAT_R8G8B8A8_UNORM,  0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 16, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	DirectX11RenderManager::DirectX11RenderManager() :
		mIsInitialise(false),
		mpD3DDevice(nullptr),
		mpD3DContext(nullptr),
		mUpdate(false),
		mVertexShader0(nullptr),
		mVertexShader1(nullptr),
		mPixelShader0(nullptr),
		mPixelShader1(nullptr),
		mSamplerState(nullptr),
		mDepthStencilState(nullptr),
		mBlendState(nullptr),
		mRasterizerState(nullptr),
		mInputLayout0(nullptr),
		mInputLayout1(nullptr)
	{
	}

	void DirectX11RenderManager::initialise(ID3D11Device* _device)
	{
		MYGUI_PLATFORM_ASSERT(!mIsInitialise, getClassTypeName() << " initialised twice");
		MYGUI_PLATFORM_LOG(Info, "* Initialise: " << getClassTypeName());

		mpD3DDevice = _device;
		_device->GetImmediateContext(&mpD3DContext);

		mVertexFormat = VertexColourType::ColourABGR;

		std::string vertexProfile, pixelProfile;
		if ( mpD3DDevice->GetFeatureLevel() == D3D_FEATURE_LEVEL_11_0 )
		{
			vertexProfile = "vs_5_0";
			pixelProfile = "ps_5_0";
		}
		else if ( mpD3DDevice->GetFeatureLevel() == D3D_FEATURE_LEVEL_10_1 )
		{
			vertexProfile = "vs_4_1";
			pixelProfile = "ps_4_1";
		}
		else if ( mpD3DDevice->GetFeatureLevel() == D3D_FEATURE_LEVEL_10_0 )
		{
			vertexProfile = "vs_4_0";
			pixelProfile = "ps_4_0";
		}
		else if ( mpD3DDevice->GetFeatureLevel() == D3D_FEATURE_LEVEL_9_3 )
		{
			vertexProfile = "vs_4_0_level_9_3";
			pixelProfile = "ps_4_0_level_9_3";
		}
		else if ( mpD3DDevice->GetFeatureLevel() == D3D_FEATURE_LEVEL_9_2 )
		{
			vertexProfile = "vs_4_0_level_9_1";
			pixelProfile = "ps_4_0_level_9_1";
		}
		else if ( mpD3DDevice->GetFeatureLevel() == D3D_FEATURE_LEVEL_9_1 )
		{
			vertexProfile = "vs_4_0_level_9_1";
			pixelProfile = "ps_4_0_level_9_1";
		}

		// Get Current viewports
		memset(&mInfo, 0, sizeof(mInfo));
		UINT numViewports = 0;
		D3D11_VIEWPORT viewports[8];

		mpD3DContext->RSGetViewports(&numViewports, viewports);
		MYGUI_PLATFORM_ASSERT(numViewports == 0, getClassTypeName() << " 0 viewport sets");

		setViewSize((int)viewports[0].Width, (int)viewports[0].Height);

		UINT flags = (1 << 11) | (1 << 15);

		// Build Flat Vertex Shader
		ID3DBlob* bytecode = 0, *errors = 0, *signature0 = 0, *signature1 = 0;
		HRESULT hr = D3DCompile(vsSource, strlen(vsSource), "VertexShader0", 0, 0, "main", vertexProfile.c_str(), flags, 0, &bytecode, &errors);
		MYGUI_PLATFORM_ASSERT(hr == S_OK, (errors ? (char*)errors->GetBufferPointer() : "Vertex Shader Compilation failed, unknown errors!"));

		hr = D3DGetInputSignatureBlob(bytecode->GetBufferPointer(), bytecode->GetBufferSize(), &signature0);
		MYGUI_PLATFORM_ASSERT(hr == S_OK, (errors ? (char*)errors->GetBufferPointer() : "Vertex Shader Compilation failed, failed to get input signature!"));

		hr = mpD3DDevice->CreateVertexShader(bytecode->GetBufferPointer(), bytecode->GetBufferSize(), 0, &mVertexShader0);
		MYGUI_PLATFORM_ASSERT(hr == S_OK, (errors ? (char*)errors->GetBufferPointer() : "Vertex Shader Create failed!"));

		if ( bytecode ) bytecode->Release();
		if ( errors ) errors->Release();


		// Build Textured Vertex Shader
		bytecode = 0;
		errors = 0;
		hr = D3DCompile(vsTexturedSource, strlen(vsTexturedSource), "VertexShader1", 0, 0, "main", vertexProfile.c_str(), flags, 0, &bytecode, &errors);
		MYGUI_PLATFORM_ASSERT(hr == S_OK, (errors ? (char*)errors->GetBufferPointer() : "Vertex Shader Compilation failed, unknown errors!"));

		hr = D3DGetInputSignatureBlob(bytecode->GetBufferPointer(), bytecode->GetBufferSize(), &signature1);
		MYGUI_PLATFORM_ASSERT(hr == S_OK, (errors ? (char*)errors->GetBufferPointer() : "Vertex Shader Compilation failed, failed to get input signature!"));

		hr = mpD3DDevice->CreateVertexShader(bytecode->GetBufferPointer(), bytecode->GetBufferSize(), 0, &mVertexShader1);
		MYGUI_PLATFORM_ASSERT(hr == S_OK, (errors ? (char*)errors->GetBufferPointer() : "Vertex Shader Create failed!"));

		if ( bytecode ) bytecode->Release();
		if ( errors ) errors->Release();

		// Build Flat Pixel Shader
		bytecode = 0;
		errors = 0;
		hr = D3DCompile(psSource, strlen(psSource), "PixelShader0", 0, 0, "main", pixelProfile.c_str(), flags, 0, &bytecode, &errors);
		MYGUI_PLATFORM_ASSERT(hr == S_OK, (errors ? (char*)errors->GetBufferPointer() : "Vertex Shader Compilation failed, unknown errors!"));

		hr = mpD3DDevice->CreatePixelShader(bytecode->GetBufferPointer(), bytecode->GetBufferSize(), 0, &mPixelShader0);
		MYGUI_PLATFORM_ASSERT(hr == S_OK, (errors ? (char*)errors->GetBufferPointer() : "Vertex Shader Create failed!"));

		if ( bytecode ) bytecode->Release();
		if ( errors ) errors->Release();


		// Build Textured Pixel Shader
		bytecode = 0;
		errors = 0;
		hr = D3DCompile(psTexturedSource, strlen(psTexturedSource), "PixelShader1", 0, 0, "main", pixelProfile.c_str(), flags, 0, &bytecode, &errors);
		MYGUI_PLATFORM_ASSERT(hr == S_OK, (errors ? (char*)errors->GetBufferPointer() : "Vertex Shader Compilation failed, unknown errors!"));

		hr = mpD3DDevice->CreatePixelShader(bytecode->GetBufferPointer(), bytecode->GetBufferSize(), 0, &mPixelShader1);
		MYGUI_PLATFORM_ASSERT(hr == S_OK, (errors ? (char*)errors->GetBufferPointer() : "Vertex Shader Create failed!"));

		if ( bytecode ) bytecode->Release();
		if ( errors ) errors->Release();

		// Create Sampler State
		D3D11_SAMPLER_DESC samplerDesc;
		samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
		samplerDesc.BorderColor[0] = samplerDesc.BorderColor[1] = samplerDesc.BorderColor[2] = samplerDesc.BorderColor[3] = 0.0f;
		samplerDesc.ComparisonFunc = (D3D11_COMPARISON_FUNC)0;
		samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
		samplerDesc.MaxAnisotropy = 1;
		samplerDesc.MaxLOD = 0;
		samplerDesc.MinLOD = 0;
		samplerDesc.MipLODBias = 0.0f;
		hr = mpD3DDevice->CreateSamplerState(&samplerDesc, &mSamplerState);
		MYGUI_PLATFORM_ASSERT(hr == S_OK, "Sampler State Create failed!");

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
		hr = mpD3DDevice->CreateBlendState(&blendDesc, &mBlendState);
		MYGUI_PLATFORM_ASSERT(hr == S_OK, "Blend State Create failed!");

		// Create Depth Stencil State
		D3D11_DEPTH_STENCIL_DESC depthDesc;
		depthDesc.DepthEnable = false;
		depthDesc.DepthFunc = D3D11_COMPARISON_ALWAYS;
		depthDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
		depthDesc.StencilEnable = false;
		depthDesc.StencilReadMask = depthDesc.StencilWriteMask = 0;
		depthDesc.BackFace.StencilDepthFailOp = depthDesc.BackFace.StencilFailOp = depthDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
		depthDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		depthDesc.FrontFace = depthDesc.BackFace;
		hr = mpD3DDevice->CreateDepthStencilState(&depthDesc, &mDepthStencilState);
		MYGUI_PLATFORM_ASSERT(hr == S_OK, "Depth Stencil State Create failed!");

		// Create Rasterizer State
		D3D11_RASTERIZER_DESC rastDesc;
		rastDesc.FillMode              = D3D11_FILL_SOLID;
		rastDesc.CullMode              = D3D11_CULL_NONE;
		rastDesc.FrontCounterClockwise = FALSE;
		rastDesc.DepthBias             = 0;
		rastDesc.DepthBiasClamp        = 0;
		rastDesc.SlopeScaledDepthBias  = 0;
		rastDesc.ScissorEnable         = FALSE;
		rastDesc.MultisampleEnable     = FALSE;
		rastDesc.AntialiasedLineEnable = FALSE;
		hr = mpD3DDevice->CreateRasterizerState(&rastDesc, &mRasterizerState);
		MYGUI_PLATFORM_ASSERT(hr == S_OK, "Rasterizer State Create failed!");

		// Create Input Layout
		hr = mpD3DDevice->CreateInputLayout(vertexLayout, 3, signature0->GetBufferPointer(), signature0->GetBufferSize(), &mInputLayout0);
		MYGUI_PLATFORM_ASSERT(hr == S_OK, "Input Layout Create failed!");

		signature0->Release();

		// Create Input Layout
		hr = mpD3DDevice->CreateInputLayout(vertexLayout, 3, signature1->GetBufferPointer(), signature1->GetBufferSize(), &mInputLayout1);
		MYGUI_PLATFORM_ASSERT(hr == S_OK, "Input Layout Create failed!");

		signature1->Release();

		mUpdate = false;

		MYGUI_PLATFORM_LOG(Info, getClassTypeName() << " successfully initialized");
		mIsInitialise = true;
	}

	void DirectX11RenderManager::shutdown()
	{
		MYGUI_PLATFORM_ASSERT(mIsInitialise, getClassTypeName() << " is not initialised");
		MYGUI_PLATFORM_LOG(Info, "* Shutdown: " << getClassTypeName());

		destroyAllResources();

		if ( mVertexShader0 ) mVertexShader0->Release();
		if ( mVertexShader1 ) mVertexShader1->Release();
		if ( mPixelShader0 ) mPixelShader0->Release();
		if ( mPixelShader1 ) mPixelShader1->Release();
		if ( mSamplerState ) mSamplerState->Release();
		if ( mBlendState ) mBlendState->Release();
		if ( mDepthStencilState ) mDepthStencilState->Release();
		if ( mRasterizerState ) mRasterizerState->Release();
		if ( mInputLayout0 ) mInputLayout0->Release();
		if ( mInputLayout1 ) mInputLayout1->Release();
		if ( mpD3DContext) mpD3DContext->Release();

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
		if ( texture == 0 || texture->mResourceView == 0 )
		{
			DirectX11VertexBuffer* buffer = static_cast<DirectX11VertexBuffer*>(_buffer);
			mpD3DContext->PSSetShader(mPixelShader0, 0, 0);
			mpD3DContext->VSSetShader(mVertexShader0, 0, 0);

			UINT stride = sizeof(Vertex), offset = 0;
			mpD3DContext->IASetVertexBuffers(0, 1, &buffer->mBuffer, &stride, &offset);
			mpD3DContext->IASetInputLayout(mInputLayout0);
			mpD3DContext->Draw(_count, 0);
		}
		else
		{
			DirectX11VertexBuffer* buffer = static_cast<DirectX11VertexBuffer*>(_buffer);
			mpD3DContext->PSSetShader(mPixelShader1, 0, 0);
			mpD3DContext->VSSetShader(mVertexShader1, 0, 0);

			mpD3DContext->PSSetSamplers(0, 1, &mSamplerState);
			mpD3DContext->PSSetShaderResources(0, 1, &texture->mResourceView);

			UINT stride = sizeof(Vertex), offset = 0;
			mpD3DContext->IASetVertexBuffers(0, 1, &buffer->mBuffer, &stride, &offset);
			mpD3DContext->IASetInputLayout(mInputLayout1);
			mpD3DContext->Draw(_count, 0);
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

		onFrameEvent((float)((double)(time) / (double)1000));

		last_time = now_time;

		begin();
		onRenderToTarget(this, mUpdate);
		end();

		mUpdate = false;
	}

	void DirectX11RenderManager::begin()
	{
		mpD3DContext->OMSetBlendState(mBlendState, 0, 0xffffffff);
		mpD3DContext->RSSetState(mRasterizerState);
		mpD3DContext->OMSetDepthStencilState(mDepthStencilState, 0);
		mpD3DContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
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
		if ( _format != PixelFormat::R8G8B8A8 ) return false;
		return true;
	}

	void DirectX11RenderManager::destroyAllResources()
	{
		for (MapTexture::const_iterator item = mTextures.begin(); item != mTextures.end(); ++item)
		{
			delete item->second;
		}
		mTextures.clear();
	}

	void DirectX11RenderManager::setViewSize(int _width, int _height)
	{
		if (_height == 0)
			_height = 1;
		if (_width == 0)
			_width = 1;

		mViewSize.set(_width, _height);

		mInfo.maximumDepth = 0.0f;
		mInfo.hOffset = 0.0f;//-0.5f / float(mViewSize.width);
		mInfo.vOffset = 0.0f;//-0.5f / float(mViewSize.height);
		mInfo.aspectCoef = float(mViewSize.height) / float(mViewSize.width);
		mInfo.pixScaleX = 1.0f / float(mViewSize.width);
		mInfo.pixScaleY = 1.0f / float(mViewSize.height);

		onResizeView(mViewSize);

		mUpdate = true;
	}


} // namespace MyGUI
