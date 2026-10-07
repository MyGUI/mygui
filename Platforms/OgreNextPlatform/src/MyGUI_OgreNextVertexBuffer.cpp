#include "MyGUI_OgreNextVertexBuffer.h"

#include "MyGUI_OgreNextDiagnostic.h"
#include "MyGUI_OgreNextRenderManager.h"
#include "MyGUI_OgreNextManager.h"

#include <OgreRoot.h>
#include <OgreRenderSystem.h>
#include <Vao/OgreVaoManager.h>
#include <Vao/OgreVertexBufferPacked.h>
#include <Vao/OgreVertexArrayObject.h>
#include <Vao/OgreVertexElements.h>

#include "MyGUI_LastHeader.h"

namespace MyGUI
{
	namespace
	{

		constexpr size_t VERTEX_IN_QUAD = 6;
		constexpr size_t VERTEX_BUFFER_SLACK = 5 * VERTEX_IN_QUAD;

		Ogre::VaoManager* getVaoManager()
		{
			Ogre::RenderSystem* rs = Ogre::Root::getSingleton().getRenderSystem();
			return rs ? rs->getVaoManager() : nullptr;
		}

		Ogre::VertexElement2Vec makeVertexLayout()
		{
			Ogre::VertexElement2Vec elements;
			elements.emplace_back(Ogre::VET_FLOAT3, Ogre::VES_POSITION);
			elements.emplace_back(Ogre::VET_COLOUR, Ogre::VES_DIFFUSE);
			elements.emplace_back(Ogre::VET_FLOAT2, Ogre::VES_TEXTURE_COORDINATES);
			return elements;
		}

	}

	OgreNextVertexBuffer::OgreNextVertexBuffer() = default;

	OgreNextVertexBuffer::~OgreNextVertexBuffer()
	{
		for (auto& slot : mSlots)
			destroyBuffer(slot);
	}

	void OgreNextVertexBuffer::setVertexCount(size_t _count)
	{
		mRequestedCount = _count;
	}

	size_t OgreNextVertexBuffer::getVertexCount() const
	{
		return mRequestedCount;
	}

	void OgreNextVertexBuffer::createBuffer(BufferSlot& slot, size_t capacity)
	{
		Ogre::VaoManager* vao = getVaoManager();
		MYGUI_PLATFORM_ASSERT(vao != nullptr, "VaoManager is null");

		slot.buffer =
			vao->createVertexBuffer(makeVertexLayout(), capacity, Ogre::BT_DYNAMIC_PERSISTENT, nullptr, false);

		slot.vao = vao->createVertexArrayObject({slot.buffer}, nullptr, Ogre::OT_TRIANGLE_LIST);
		slot.capacity = capacity;
	}

	void OgreNextVertexBuffer::destroyBuffer(BufferSlot& slot)
	{
		if (!slot.buffer && !slot.vao)
			return;
		auto* render = OgreNextRenderManager::getInstancePtr();
		auto* manager = render ? render->getManager() : nullptr;
		if (manager && manager->suspendBatch())
			manager->resumeBatch();
		Ogre::VaoManager* vao = getVaoManager();
		if (vao == nullptr)
			return;

		if (slot.buffer != nullptr && slot.buffer->getMappingState() != Ogre::MS_UNMAPPED)
			slot.buffer->unmap(Ogre::UO_UNMAP_ALL);

		if (slot.vao != nullptr)
		{
			vao->destroyVertexArrayObject(slot.vao);
			slot.vao = nullptr;
		}
		if (slot.buffer != nullptr)
		{
			vao->destroyVertexBuffer(slot.buffer);
			slot.buffer = nullptr;
		}
		slot.capacity = 0;
	}

	Vertex* OgreNextVertexBuffer::lock()
	{
		// Ogre dynamic buffers may be mapped only once per frame. Retain one slot
		// per update so a later upload cannot overwrite vertices still used by the GPU.
		const auto frame = getVaoManager()->getFrameCount();
		if (mFrame != frame)
		{
			mFrame = frame;
			mNextSlot = 0;
		}
		if (mNextSlot == mSlots.size())
			mSlots.emplace_back();
		auto& slot = mSlots[mNextSlot++];
		if (mRequestedCount > slot.capacity)
		{
			destroyBuffer(slot);
			createBuffer(slot, mRequestedCount + VERTEX_BUFFER_SLACK);
		}

		return static_cast<Vertex*>(slot.buffer->map(0u, mRequestedCount));
	}

	void OgreNextVertexBuffer::unlock()
	{
		auto& slot = mSlots[mNextSlot - 1];
		slot.buffer->unmap(Ogre::UO_KEEP_PERSISTENT, 0u, mRequestedCount);
		slot.vao->setPrimitiveRange(0u, static_cast<Ogre::uint32>(mRequestedCount));
	}

} // namespace MyGUI
