#include "MyGUI_OgreNextManager.h"

#include "MyGUI_OgreNextDiagnostic.h"
#include "MyGUI_OgreNextRenderable.h"
#include "MyGUI_OgreNextTexture.h"
#include "MyGUI_OgreNextVertexBuffer.h"

#include <OgreRoot.h>
#include <OgreRenderSystem.h>
#include <OgreSceneManager.h>
#include <OgreSceneNode.h>
#include <OgreMovableObject.h>
#include <OgreHlmsManager.h>
#include <OgreHlms.h>
#include <OgreHlmsCommon.h>
#include <OgreHlmsPso.h>
#include <OgreMaterial.h>
#include <OgreTechnique.h>
#include <OgrePass.h>
#include <OgreGpuProgramParams.h>
#include <OgreHighLevelGpuProgramManager.h>
#include <OgreHighLevelGpuProgram.h>
#include <OgreUnifiedHighLevelGpuProgram.h>
#include <OgreRenderPassDescriptor.h>
#include <OgreRenderQueue.h>
#include <OgreResourceGroupManager.h>
#include <OgreRoot.h>
#include <Vao/OgreVaoManager.h>
#include <Vao/OgreVertexArrayObject.h>
#include <Vao/OgreIndirectBufferPacked.h>
#include <CommandBuffer/OgreCommandBuffer.h>
#include <CommandBuffer/OgreCbPipelineStateObject.h>
#include <CommandBuffer/OgreCbShaderBuffer.h>
#include <CommandBuffer/OgreCbDrawCall.h>

#include "MyGUI_LastHeader.h"

namespace MyGUI
{
	namespace
	{
		const Ogre::HlmsCache c_dummyCache(0, Ogre::HLMS_MAX, Ogre::HLMS_CACHE_FLAGS_NONE, Ogre::HlmsPso());

		// A visible=false movable object that we use only so QueuedRenderable has
		// a MovableObject to point at. Attaches to SCENE_STATIC to avoid touching
		// dynamic updates.
		class DummyMO final : public Ogre::MovableObject
		{
		public:
			DummyMO(
				Ogre::IdType id,
				Ogre::ObjectMemoryManager* objectMemoryManager,
				Ogre::SceneManager* manager,
				Ogre::uint8 renderQueueId) :
				MovableObject(id, objectMemoryManager, manager, renderQueueId)
			{
			}

			const Ogre::String& getMovableType() const override
			{
				return Ogre::BLANKSTRING;
			}
		};

		constexpr size_t INITIAL_INDIRECT_DRAWS = 64u;

		// ----------- shader source table -----------

		const char* c_vertShaderHLSL = R"(
struct VS_INPUT
{
    float3 pos : POSITION;
    float4 col : COLOR0;
    float2 uv  : TEXCOORD0;
};
struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float4 col : COLOR0;
    float2 uv  : TEXCOORD0;
};
uniform float4x4 worldViewProj;
PS_INPUT main(VS_INPUT input)
{
    PS_INPUT o;
    o.pos = mul(worldViewProj, float4(input.pos, 1.0f));
    o.col = input.col;
    o.uv  = input.uv;
    return o;
}
)";

		const char* c_fragShaderHLSL = R"(
struct PS_INPUT
{
    float4 pos : SV_POSITION;
    float4 col : COLOR0;
    float2 uv  : TEXCOORD0;
};
sampler sampler0 : register(s0);
Texture2D texture0 : register(t0);
float4 main(PS_INPUT input) : SV_Target
{
    return input.col * texture0.Sample(sampler0, input.uv);
}
)";

		const char* c_vertShaderGLSL = R"(#version 150
uniform mat4 worldViewProj;
in vec3 vertex;
in vec4 colour;
in vec2 uv0;
out vec2 outUV;
out vec4 outCol;
void main()
{
    gl_Position = worldViewProj * vec4(vertex, 1.0);
    outUV = uv0;
    outCol = colour;
}
)";

		const char* c_fragShaderGLSL = R"(#version 150
in vec2 outUV;
in vec4 outCol;
uniform sampler2D sampler0;
out vec4 fragColour;
void main()
{
    fragColour = outCol * texture(sampler0, outUV);
}
)";

		const char* c_vertShaderVK = R"(
vulkan( layout( ogre_P0 ) uniform Params { )
    uniform mat4 worldViewProj;
vulkan( }; )
vulkan_layout( OGRE_POSITION ) in vec3 vertex;
vulkan_layout( OGRE_DIFFUSE ) in vec4 colour;
vulkan_layout( OGRE_TEXCOORD0 ) in vec2 uv0;
vulkan_layout( location = 1 )
out block
{
    vec2 uv;
    vec4 col;
} outVs;
void main()
{
    gl_Position = worldViewProj * vec4(vertex, 1.0);
    outVs.uv = uv0;
    outVs.col = colour;
}
)";

		const char* c_fragShaderVK = R"(
vulkan_layout( ogre_t0 ) uniform texture2D sampler0;
vulkan( layout( ogre_s0 ) uniform sampler texSampler; )
vulkan_layout( location = 0 ) out vec4 fragColour;
vulkan_layout( location = 1 )
in block
{
    vec2 uv;
    vec4 col;
} inPs;
void main()
{
    fragColour = inPs.col * texture( vkSampler2D( sampler0, texSampler ), inPs.uv );
}
)";

		const char* c_vertShaderMetal = R"(
#include <metal_stdlib>
using namespace metal;

struct Constant
{
    float4x4 worldViewProj;
};

struct VertexIn
{
    float3 position  [[attribute(VES_POSITION)]];
    float4 colour    [[attribute(VES_DIFFUSE)]];
    float2 uv        [[attribute(VES_TEXTURE_COORDINATES0)]];
};

struct VertexOut
{
    float4 position [[position]];
    float2 uv;
    float4 col;
};

vertex VertexOut vertex_main(VertexIn in                 [[stage_in]],
                             constant Constant &uniforms [[buffer(PARAMETER_SLOT)]])
{
    VertexOut o;
    o.position = uniforms.worldViewProj * float4(in.position, 1.0);
    o.uv = in.uv;
    o.col = in.colour;
    return o;
}
)";

		const char* c_fragShaderMetal = R"(
#include <metal_stdlib>
using namespace metal;

struct VertexOut
{
    float4 position [[position]];
    float2 uv;
    float4 col;
};

fragment float4 fragment_main(VertexOut in [[stage_in]],
                              texture2d<float> tex [[texture(0)]])
{
    constexpr sampler linearSampler(coord::normalized, min_filter::linear,
                                    mag_filter::linear, mip_filter::linear);
    return in.col * tex.sample(linearSampler, in.uv);
}
)";

	} // namespace

	OgreNextManager::OgreNextManager(Ogre::SceneManager* scene) :
		mScene(scene)
	{
		MYGUI_PLATFORM_ASSERT(mScene != nullptr, "SceneManager must be non-null");

		mCommandBuffer = new Ogre::CommandBuffer();

		createPrograms();
		createDummyMovable();
	}

	OgreNextManager::~OgreNextManager()
	{
		for (auto& item : mRenderables)
		{
			item.second->clearVao();
			delete item.second;
		}
		mRenderables.clear();

		destroyIndirectBuffer();

		if (mDummyMovable != nullptr)
		{
			// MovableObject's destructor does not detach from its parent node.
			if (mScene != nullptr)
				mScene->getRootSceneNode(Ogre::SCENE_STATIC)->detachObject(mDummyMovable);
			delete mDummyMovable;
			mDummyMovable = nullptr;
		}

		delete mCommandBuffer;
		mCommandBuffer = nullptr;
	}

	void OgreNextManager::createDummyMovable()
	{
		mDummyMovable = new DummyMO(
			Ogre::Id::generateNewId<Ogre::MovableObject>(),
			&mScene->_getEntityMemoryManager(Ogre::SCENE_STATIC),
			mScene,
			254u);
		mScene->getRootSceneNode(Ogre::SCENE_STATIC)->attachObject(mDummyMovable);
		mDummyMovable->setVisible(false);
		mDummyMovable->setCastShadows(false);
	}

	void OgreNextManager::createPrograms()
	{
		auto& mgr = Ogre::HighLevelGpuProgramManager::getSingleton();
		const Ogre::String& group = Ogre::ResourceGroupManager::INTERNAL_RESOURCE_GROUP_NAME;

		Ogre::HighLevelGpuProgramPtr vertUnified = mgr.getByName("mygui/VP");
		Ogre::HighLevelGpuProgramPtr fragUnified = mgr.getByName("mygui/FP");
		if (!vertUnified)
			vertUnified = mgr.createProgram("mygui/VP", group, "unified", Ogre::GPT_VERTEX_PROGRAM);
		if (!fragUnified)
			fragUnified = mgr.createProgram("mygui/FP", group, "unified", Ogre::GPT_FRAGMENT_PROGRAM);

		auto* vertPtr = static_cast<Ogre::UnifiedHighLevelGpuProgram*>(vertUnified.get());
		auto* fragPtr = static_cast<Ogre::UnifiedHighLevelGpuProgram*>(fragUnified.get());

		auto ensureDelegate =
			[&](const std::string& name,
				const std::string& language,
				Ogre::GpuProgramType type,
				const char* source,
				const char* hlslTarget,
				const char* metalEntry,
				bool vulkan,
				Ogre::UnifiedHighLevelGpuProgram* parent)
		{
			Ogre::HighLevelGpuProgramPtr program = mgr.getByName(name);
			if (program)
				return;
			program = mgr.createProgram(name, group, language, type);
			program->setSource(source);
			if (hlslTarget != nullptr)
			{
				program->setParameter("target", hlslTarget);
				program->setParameter("entry_point", "main");
			}
			if (metalEntry != nullptr)
			{
				program->setParameter("entry_point", metalEntry);
			}
			if (vulkan)
			{
				program->setPrefabRootLayout(Ogre::PrefabRootLayout::Standard);
			}
			program->load();
			parent->addDelegateProgram(program->getName());
		};

		ensureDelegate(
			"mygui/VP/D3D11",
			"hlsl",
			Ogre::GPT_VERTEX_PROGRAM,
			c_vertShaderHLSL,
			"vs_5_0 vs_4_0",
			nullptr,
			false,
			vertPtr);
		ensureDelegate(
			"mygui/FP/D3D11",
			"hlsl",
			Ogre::GPT_FRAGMENT_PROGRAM,
			c_fragShaderHLSL,
			"ps_5_0 ps_4_0",
			nullptr,
			false,
			fragPtr);

		ensureDelegate(
			"mygui/VP/GL150",
			"glsl",
			Ogre::GPT_VERTEX_PROGRAM,
			c_vertShaderGLSL,
			nullptr,
			nullptr,
			false,
			vertPtr);
		ensureDelegate(
			"mygui/FP/GL150",
			"glsl",
			Ogre::GPT_FRAGMENT_PROGRAM,
			c_fragShaderGLSL,
			nullptr,
			nullptr,
			false,
			fragPtr);

		ensureDelegate(
			"mygui/VP/vulkan",
			"glslvk",
			Ogre::GPT_VERTEX_PROGRAM,
			c_vertShaderVK,
			nullptr,
			nullptr,
			true,
			vertPtr);
		ensureDelegate(
			"mygui/FP/vulkan",
			"glslvk",
			Ogre::GPT_FRAGMENT_PROGRAM,
			c_fragShaderVK,
			nullptr,
			nullptr,
			true,
			fragPtr);

		ensureDelegate(
			"mygui/VP/Metal",
			"metal",
			Ogre::GPT_VERTEX_PROGRAM,
			c_vertShaderMetal,
			nullptr,
			"vertex_main",
			false,
			vertPtr);
		ensureDelegate(
			"mygui/FP/Metal",
			"metal",
			Ogre::GPT_FRAGMENT_PROGRAM,
			c_fragShaderMetal,
			nullptr,
			"fragment_main",
			false,
			fragPtr);
	}

	void OgreNextManager::ensureIndirectBuffer(size_t neededDraws)
	{
		Ogre::VaoManager* vao = Ogre::Root::getSingleton().getRenderSystem()->getVaoManager();
		if (mIndirectBuffer != nullptr && neededDraws * sizeof(Ogre::CbDrawStrip) <= mIndirectBuffer->getNumElements())
			return;

		destroyIndirectBuffer();

		mIndirectCapacityDraws = std::max<size_t>(INITIAL_INDIRECT_DRAWS, neededDraws);
		mIndirectBuffer = vao->createIndirectBuffer(
			mIndirectCapacityDraws * sizeof(Ogre::CbDrawStrip),
			Ogre::BT_DYNAMIC_PERSISTENT,
			nullptr,
			false);
	}

	void OgreNextManager::destroyIndirectBuffer()
	{
		if (mIndirectBuffer == nullptr)
			return;

		if (mIndirectBuffer->getMappingState() != Ogre::MS_UNMAPPED)
			mIndirectBuffer->unmap(Ogre::UO_UNMAP_ALL);

		Ogre::VaoManager* vao = Ogre::Root::getSingleton().getRenderSystem()->getVaoManager();
		if (vao != nullptr)
			vao->destroyIndirectBuffer(mIndirectBuffer);
		mIndirectBuffer = nullptr;
		mIndirectCapacityDraws = 0;
	}

	OgreNextRenderable* OgreNextManager::renderableFor(Ogre::TextureGpu* tex, const Ogre::MaterialPtr& material)
	{
		auto it = mRenderables.find(tex);
		if (it != mRenderables.end())
			return it->second;

		auto* renderable = new OgreNextRenderable();
		renderable->setMaterial(material);
		mRenderables[tex] = renderable;
		return renderable;
	}

	void OgreNextManager::notifyTextureDestroyed(Ogre::TextureGpu* tex)
	{
		auto it = mRenderables.find(tex);
		if (it == mRenderables.end())
			return;
		it->second->clearVao();
		delete it->second;
		mRenderables.erase(it);
	}

	bool OgreNextManager::suspendBatch()
	{
		if (mActiveRPD == nullptr)
			return false;

		SavedBatchState saved;
		saved.rpd = mActiveRPD;
		saved.target = mActiveTarget;
		saved.projMatrix = mActiveProjMatrix;
		saved.passCache = mPassCache;
		saved.hlms = mActiveHlms;
		saved.baseInstanceAndIndirectBuffers = mBaseInstanceAndIndirectBuffers;
		mSavedBatchStates.push_back(saved);

		endBatch();
		return true;
	}

	void OgreNextManager::resumeBatch()
	{
		if (mSavedBatchStates.empty())
			return;

		SavedBatchState saved = mSavedBatchStates.back();
		mSavedBatchStates.pop_back();

		mActiveRPD = saved.rpd;
		mActiveTarget = saved.target;
		mDrawIndex = 0;
		mLastDrawCount = 0;

		Ogre::RenderSystem* rs = Ogre::Root::getSingleton().getRenderSystem();
		mCommandBuffer->setCurrentRenderSystem(rs);

		ensureIndirectBuffer(INITIAL_INDIRECT_DRAWS);

		if (rs->getVaoManager()->supportsIndirectBuffers())
		{
			mIndirectMapped = static_cast<uint8_t*>(mIndirectBuffer->map(0u, mIndirectBuffer->getNumElements()));
		}
		else
		{
			mIndirectMapped = static_cast<uint8_t*>(mIndirectBuffer->getSwBufferPtr());
		}

		mActiveProjMatrix = saved.projMatrix;

		const Ogre::Vector4 viewportSize(0, 0, 1, 1);
		const Ogre::Vector4 scissors(0, 0, 1, 1);
		rs->beginRenderPassDescriptor(mActiveRPD, mActiveTarget, 0u, &viewportSize, &scissors, 1u, false, false);
		rs->executeRenderPassDescriptorDelayedActions();

		mActiveHlms = saved.hlms;
		mPassCache = saved.passCache;
		mBaseInstanceAndIndirectBuffers = saved.baseInstanceAndIndirectBuffers;
	}

	void OgreNextManager::beginBatch(Ogre::RenderPassDescriptor* rpd, Ogre::TextureGpu* target)
	{
		MYGUI_PLATFORM_ASSERT(mActiveRPD == nullptr, "beginBatch called while a batch is already active");

		mActiveRPD = rpd;
		mActiveTarget = target;
		mDrawIndex = 0;
		mLastDrawCount = 0;

		Ogre::RenderSystem* rs = Ogre::Root::getSingleton().getRenderSystem();
		mCommandBuffer->setCurrentRenderSystem(rs);

		// Grow indirect buffer to at least our default. It will be grown further
		// if a batch actually needs more draws (see submitDraw).
		ensureIndirectBuffer(INITIAL_INDIRECT_DRAWS);

		if (rs->getVaoManager()->supportsIndirectBuffers())
		{
			mIndirectMapped = static_cast<uint8_t*>(mIndirectBuffer->map(0u, mIndirectBuffer->getNumElements()));
		}
		else
		{
			mIndirectMapped = static_cast<uint8_t*>(mIndirectBuffer->getSwBufferPtr());
		}

		// Compute the projection matrix: identity, adjusted for backend depth
		// range and flipped if the target requires it.
		Ogre::Matrix4 proj;
		rs->_convertProjectionMatrix(Ogre::Matrix4::IDENTITY, proj);
		if (rpd != nullptr && rpd->requiresTextureFlipping())
		{
			proj[1][0] = -proj[1][0];
			proj[1][1] = -proj[1][1];
			proj[1][2] = -proj[1][2];
			proj[1][3] = -proj[1][3];
		}
		mActiveProjMatrix = proj;

		const Ogre::Vector4 viewportSize(0, 0, 1, 1);
		const Ogre::Vector4 scissors(0, 0, 1, 1);
		rs->beginRenderPassDescriptor(rpd, target, 0u, &viewportSize, &scissors, 1u, false, false);
		rs->executeRenderPassDescriptorDelayedActions();

		Ogre::HlmsManager* hlmsManager = Ogre::Root::getSingleton().getHlmsManager();
		mActiveHlms = hlmsManager->getHlms(Ogre::HLMS_LOW_LEVEL);
		mPassCache = mActiveHlms->preparePassHash(nullptr, false, false, mScene);

		mBaseInstanceAndIndirectBuffers = 0;
		Ogre::VaoManager* vaoMgr = rs->getVaoManager();
		if (vaoMgr->supportsIndirectBuffers())
			mBaseInstanceAndIndirectBuffers = 2;
		else if (vaoMgr->supportsBaseInstance())
			mBaseInstanceAndIndirectBuffers = 1;
	}

	void OgreNextManager::deferSamplerRestore(Ogre::TextureUnitState* tu, const Ogre::HlmsSamplerblock* original)
	{
		// Only store the first (original) state per TUS; subsequent calls for
		// the same TUS are no-ops because setSamplerblock already made the
		// TUS point to the FO_NONE block.
		mPendingSamplerRestores.emplace(tu, original);
	}

	void OgreNextManager::submitDraw(IVertexBuffer* buffer, ITexture* texture, size_t count)
	{
		MYGUI_PLATFORM_ASSERT(mActiveRPD != nullptr, "submitDraw outside beginBatch/endBatch");
		if (count == 0)
			return;

		auto* vbo = static_cast<OgreNextVertexBuffer*>(buffer);
		auto* tex = static_cast<OgreNextTexture*>(texture);
		Ogre::VertexArrayObject* vao = vbo->getVao();
		Ogre::TextureGpu* gpuTex = tex->getOgreTexture();
		Ogre::MaterialPtr material = tex->getMaterial();
		if (vao == nullptr || gpuTex == nullptr || !material)
			return;

		OgreNextRenderable* renderable = renderableFor(gpuTex, material);
		renderable->setVao(vao);

		// Grow indirect buffer if this draw would overflow. To avoid remapping
		// mid-batch, remap once and continue.
		if ((mDrawIndex + 1u) * sizeof(Ogre::CbDrawStrip) > mIndirectBuffer->getNumElements())
		{
			Ogre::RenderSystem* rs = Ogre::Root::getSingleton().getRenderSystem();
			if (mIndirectBuffer->getMappingState() != Ogre::MS_UNMAPPED)
				mIndirectBuffer->unmap(Ogre::UO_UNMAP_ALL);
			ensureIndirectBuffer((mDrawIndex + 1u) * 2u);
			if (rs->getVaoManager()->supportsIndirectBuffers())
				mIndirectMapped = static_cast<uint8_t*>(mIndirectBuffer->map(0u, mIndirectBuffer->getNumElements()));
			else
				mIndirectMapped = static_cast<uint8_t*>(mIndirectBuffer->getSwBufferPtr());
		}

		// Update the projection uniform on this material.
		material->getTechnique(0)->getPass(0)->getVertexProgramParameters()->setNamedConstant(
			"worldViewProj",
			mActiveProjMatrix);

		Ogre::QueuedRenderable queuedRenderable(0u, renderable, mDummyMovable);
		const Ogre::HlmsCache* hlmsCache =
			mActiveHlms->getMaterial(&c_dummyCache, mPassCache, queuedRenderable, false, nullptr);

		Ogre::CbPipelineStateObject* psoCmd = mCommandBuffer->addCommand<Ogre::CbPipelineStateObject>();
		*psoCmd = Ogre::CbPipelineStateObject(&hlmsCache->pso);

		mActiveHlms->fillBuffersForV2(hlmsCache, queuedRenderable, false, 0u, mCommandBuffer);

		*mCommandBuffer->addCommand<Ogre::CbVao>() = Ogre::CbVao(vao);
		*mCommandBuffer->addCommand<Ogre::CbIndirectBuffer>() = Ogre::CbIndirectBuffer(mIndirectBuffer);

		// Write CbDrawStrip at slot mDrawIndex.
		Ogre::CbDrawStrip* drawStrip =
			reinterpret_cast<Ogre::CbDrawStrip*>(mIndirectMapped + mDrawIndex * sizeof(Ogre::CbDrawStrip));
		drawStrip->primCount = count;
		drawStrip->instanceCount = 1u;
		drawStrip->firstVertexIndex =
			static_cast<uint32_t>(vao->getBaseVertexBuffer()->_getFinalBufferStart() + vao->getPrimitiveStart());
		drawStrip->baseInstance = 0u;

		void* indirectOffset =
			reinterpret_cast<void*>(mIndirectBuffer->_getFinalBufferStart() + mDrawIndex * sizeof(Ogre::CbDrawStrip));

		Ogre::CbDrawCallStrip* drawCall = mCommandBuffer->addCommand<Ogre::CbDrawCallStrip>();
		*drawCall = Ogre::CbDrawCallStrip(mBaseInstanceAndIndirectBuffers, vao, indirectOffset);
		drawCall->numDraws = 1u;

		++mDrawIndex;
		++mLastDrawCount;
	}

	void OgreNextManager::endBatch()
	{
		if (mActiveRPD == nullptr)
			return;

		Ogre::RenderSystem* rs = Ogre::Root::getSingleton().getRenderSystem();

		if (mIndirectBuffer != nullptr && mIndirectBuffer->getMappingState() != Ogre::MS_UNMAPPED)
			mIndirectBuffer->unmap(Ogre::UO_KEEP_PERSISTENT);
		mIndirectMapped = nullptr;

		if (mDrawIndex > 0 && mActiveHlms != nullptr)
		{
			mActiveHlms->preCommandBufferExecution(mCommandBuffer);
			mCommandBuffer->execute();
			mActiveHlms->postCommandBufferExecution(mCommandBuffer);
		}

		// Restore samplerblocks that were temporarily overridden by doManualRender.
		// Must happen after command-buffer execution so the override is live during the draw.
		for (const auto& restore : mPendingSamplerRestores)
			restore.first->setSamplerblock(*restore.second);
		mPendingSamplerRestores.clear();

		mCommandBuffer->clear();

		rs->endRenderPassDescriptor();

		mActiveRPD = nullptr;
		mActiveTarget = nullptr;
		mActiveHlms = nullptr;
		mDrawIndex = 0;
	}

} // namespace MyGUI
