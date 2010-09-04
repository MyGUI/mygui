#include "precompiled.h"
#include "VertexBuffer.h"
#include "RenderManager.h"
#include "MyGUI_OgreRenderManager.h"

namespace demo
{

	VertexBuffer::VertexBuffer() :
		mVertexCount(6),
		mCurrentTexture(0, 0, 1, 1),
		mCurrentColour(0xFFFFFFFF),
		mRenderMode(RenderModeNormal)
	{
		createVertexBuffer();
	}

	VertexBuffer::~VertexBuffer()
	{
		destroyVertexBuffer();
	}

	const std::string& VertexBuffer::getTextureName()
	{
		return mTextureName;
	}

	void VertexBuffer::setTextureName(const std::string& _value)
	{
		mTextureName = _value;

		if (!Ogre::TextureManager::getSingletonPtr()->resourceExists(mTextureName))
			Ogre::TextureManager::getSingletonPtr()->load(mTextureName, "General", Ogre::TEX_TYPE_2D, 0);
	}

	Ogre::RenderOperation& VertexBuffer::getRenderOperation()
	{
		return mRenderOperation;
	}

	bool VertexBuffer::getEmpty()
	{
		return mVertexCount == 0;
	}

	void VertexBuffer::createVertexBuffer()
	{
		mRenderOperation.vertexData = new Ogre::VertexData();
		mRenderOperation.vertexData->vertexStart = 0;

		Ogre::VertexDeclaration* vd = mRenderOperation.vertexData->vertexDeclaration;
		vd->addElement( 0, 0, Ogre::VET_FLOAT3, Ogre::VES_POSITION );
		vd->addElement( 0, Ogre::VertexElement::getTypeSize( Ogre::VET_FLOAT3 ), Ogre::VET_COLOUR, Ogre::VES_DIFFUSE );
		vd->addElement( 0, Ogre::VertexElement::getTypeSize( Ogre::VET_FLOAT3 ) +
						   Ogre::VertexElement::getTypeSize( Ogre::VET_COLOUR ),
						   Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES );

		// Create the Vertex Buffer, using the Vertex Structure we previously declared in _declareVertexStructure.
		mVertexBuffer = Ogre::HardwareBufferManager::getSingleton( ).createVertexBuffer(
			mRenderOperation.vertexData->vertexDeclaration->getVertexSize(0), // declared Vertex used
			mVertexCount,
			Ogre::HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY_DISCARDABLE,
			false );

		// Bind the created buffer to the renderOperation object.  Now we can manipulate the buffer, and the RenderOp keeps the changes.
		mRenderOperation.vertexData->vertexBufferBinding->setBinding( 0, mVertexBuffer );
		mRenderOperation.operationType = Ogre::RenderOperation::OT_TRIANGLE_LIST;
		mRenderOperation.useIndexes = false;
	}

	void VertexBuffer::destroyVertexBuffer()
	{
		delete mRenderOperation.vertexData;
		mRenderOperation.vertexData = 0;
		mVertexBuffer.setNull();
	}

	const MyGUI::IntCoord& VertexBuffer::getCoord()
	{
		return mCoord;
	}

	void VertexBuffer::setCoord(const MyGUI::IntCoord& _value)
	{
		mCoord = _value;
		update();
	}

	void VertexBuffer::update()
	{
		MyGUI::VertexQuad* quad = (MyGUI::VertexQuad*)mVertexBuffer->lock(Ogre::HardwareVertexBuffer::HBL_DISCARD);

		const MyGUI::RenderTargetInfo& info = MyGUI::OgreRenderManager::getInstance().getInfo();

		float vertex_z = info.maximumDepth;

		float vertex_left = ((info.pixScaleX * (float)(mCoord.left - info.leftOffset) + info.hOffset) * 2) - 1;
		float vertex_right = vertex_left + (info.pixScaleX * (float)mCoord.width * 2);
		float vertex_top = -(((info.pixScaleY * (float)(mCoord.top - info.topOffset) + info.vOffset) * 2) - 1);
		float vertex_bottom = vertex_top - (info.pixScaleY * (float)mCoord.height * 2);

		quad->set(
			vertex_left,
			vertex_top,
			vertex_right,
			vertex_bottom,
			vertex_z,
			mCurrentTexture.left,
			mCurrentTexture.top,
			mCurrentTexture.right,
			mCurrentTexture.bottom,
			mCurrentColour
			);

		mVertexBuffer->unlock();
		mRenderOperation.vertexData->vertexCount = 6;
	}

	void VertexBuffer::doRender(RenderManager* _renderManager)
	{
		if (mRenderMode == RenderModeFilterNone)
		{
			_renderManager->setCurrentManual(true);

			_renderManager->initState();
			_renderManager->getRenderSystem()->_setTexture(0, true, getTextureName());
			_renderManager->getRenderSystem()->_setTextureUnitFiltering(0, Ogre::FO_NONE, Ogre::FO_NONE, Ogre::FO_NONE);

			_renderManager->getRenderSystem()->_render(getRenderOperation());
		}
		else if (mRenderMode == RenderModePolygonWireframe)
		{
			_renderManager->setCurrentManual(true);

			_renderManager->initState();
			_renderManager->getRenderSystem()->_setTexture(0, true, getTextureName());
			_renderManager->getRenderSystem()->_setPolygonMode(Ogre::PM_WIREFRAME);

			_renderManager->getRenderSystem()->_render(getRenderOperation());
		}
		else if (mRenderMode == RenderModeMask)
		{
			_renderManager->setCurrentManual(true);

			initStateMask(_renderManager);

			_renderManager->getRenderSystem()->_render(getRenderOperation());
		}
		else
		{
			if (_renderManager->getCurrentManual())
			{
				_renderManager->initState();
				_renderManager->setCurrentManual(false);
			}

			_renderManager->getRenderSystem()->_setTexture(0, true, getTextureName());
			_renderManager->getRenderSystem()->_setTextureUnitFiltering(0, Ogre::FO_LINEAR, Ogre::FO_LINEAR, Ogre::FO_POINT);

			_renderManager->getRenderSystem()->_render(getRenderOperation());
		}
	}

	void VertexBuffer::setRenderMode(RenderMode _value)
	{
		mRenderMode = _value;
	}

	void VertexBuffer::initStateMask(RenderManager* _renderManager)
	{
		Ogre::LayerBlendModeEx mColourBlendMode;	//!< Controls colour blending mode used.
		Ogre::LayerBlendModeEx mAlphaBlendMode;	//!< Controls alpha blending mode used.
		Ogre::TextureUnitState::UVWAddressingMode mUvwAddressMode;

		// Initialise blending modes to be used.
		mColourBlendMode.blendType = Ogre::LBT_COLOUR;
		mColourBlendMode.source1 = Ogre::LBS_TEXTURE;
		mColourBlendMode.source2 = Ogre::LBS_DIFFUSE;
		mColourBlendMode.operation = Ogre::LBX_MODULATE;

		mAlphaBlendMode.blendType = Ogre::LBT_ALPHA;
		mAlphaBlendMode.source1 = Ogre::LBS_TEXTURE;
		mAlphaBlendMode.source2 = Ogre::LBS_DIFFUSE;
		mAlphaBlendMode.operation = Ogre::LBX_MODULATE;

		mUvwAddressMode.u = Ogre::TextureUnitState::TAM_CLAMP;
		mUvwAddressMode.v = Ogre::TextureUnitState::TAM_CLAMP;
		mUvwAddressMode.w = Ogre::TextureUnitState::TAM_CLAMP;

		// set-up matrices
		_renderManager->getRenderSystem()->_setWorldMatrix(Ogre::Matrix4::IDENTITY);
		_renderManager->getRenderSystem()->_setViewMatrix(Ogre::Matrix4::IDENTITY);
		_renderManager->getRenderSystem()->_setProjectionMatrix(Ogre::Matrix4::IDENTITY);

		// initialise render settings
		_renderManager->getRenderSystem()->setLightingEnabled(false);
		_renderManager->getRenderSystem()->_setDepthBufferParams(false, false);
		_renderManager->getRenderSystem()->_setDepthBias(0, 0);
		_renderManager->getRenderSystem()->_setCullingMode(Ogre::CULL_NONE);
		_renderManager->getRenderSystem()->_setFog(Ogre::FOG_NONE);
		_renderManager->getRenderSystem()->_setColourBufferWriteEnabled(true, true, true, true);
		_renderManager->getRenderSystem()->unbindGpuProgram(Ogre::GPT_FRAGMENT_PROGRAM);
		_renderManager->getRenderSystem()->unbindGpuProgram(Ogre::GPT_VERTEX_PROGRAM);
		_renderManager->getRenderSystem()->setShadingType(Ogre::SO_GOURAUD);
		_renderManager->getRenderSystem()->_setPolygonMode(Ogre::PM_SOLID);

		// initialise texture settings
		_renderManager->getRenderSystem()->_setTextureCoordCalculation(0, Ogre::TEXCALC_NONE);
		_renderManager->getRenderSystem()->_setTextureCoordSet(0, 0);
		_renderManager->getRenderSystem()->_setTextureUnitFiltering(0, Ogre::FO_LINEAR, Ogre::FO_LINEAR, Ogre::FO_POINT);
		_renderManager->getRenderSystem()->_setTextureAddressingMode(0, mUvwAddressMode);
		_renderManager->getRenderSystem()->_setTextureMatrix(0, Ogre::Matrix4::IDENTITY);
		_renderManager->getRenderSystem()->_setAlphaRejectSettings(Ogre::CMPF_ALWAYS_PASS, 0, false);
		_renderManager->getRenderSystem()->_setTextureBlendMode(0, mColourBlendMode);
		_renderManager->getRenderSystem()->_setTextureBlendMode(0, mAlphaBlendMode);
		_renderManager->getRenderSystem()->_disableTextureUnitsFrom(1);

		// enable alpha blending
		_renderManager->getRenderSystem()->_setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);

		_renderManager->getRenderSystem()->_setTexture(0, true, getTextureName());
	}

} // namespace demo
