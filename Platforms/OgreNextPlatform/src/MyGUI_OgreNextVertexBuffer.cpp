#include "MyGUI_OgreNextVertexBuffer.h"

#include "MyGUI_OgreNextDiagnostic.h"

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
		destroyBuffer();
	}

	void OgreNextVertexBuffer::setVertexCount(size_t _count)
	{
		mRequestedCount = _count;
	}

	size_t OgreNextVertexBuffer::getVertexCount() const
	{
		return mRequestedCount;
	}

	void OgreNextVertexBuffer::createBuffer(size_t capacity)
	{
		Ogre::VaoManager* vao = getVaoManager();
		MYGUI_PLATFORM_ASSERT(vao != nullptr, "VaoManager is null");

		mBuffer = vao->createVertexBuffer(makeVertexLayout(), capacity, Ogre::BT_DYNAMIC_PERSISTENT, nullptr, false);

		mVao = vao->createVertexArrayObject({mBuffer}, nullptr, Ogre::OT_TRIANGLE_LIST);
		mCapacity = capacity;
	}

	void OgreNextVertexBuffer::destroyBuffer()
	{
		Ogre::VaoManager* vao = getVaoManager();
		if (vao == nullptr)
			return;

		if (mBuffer != nullptr && mBuffer->getMappingState() != Ogre::MS_UNMAPPED)
			mBuffer->unmap(Ogre::UO_UNMAP_ALL);

		if (mVao != nullptr)
		{
			vao->destroyVertexArrayObject(mVao);
			mVao = nullptr;
		}
		if (mBuffer != nullptr)
		{
			vao->destroyVertexBuffer(mBuffer);
			mBuffer = nullptr;
		}
		mCapacity = 0;
	}

	Vertex* OgreNextVertexBuffer::lock()
	{
		if (mRequestedCount > mCapacity)
		{
			destroyBuffer();
			createBuffer(mRequestedCount + VERTEX_BUFFER_SLACK);
		}

		return static_cast<Vertex*>(mBuffer->map(0u, mRequestedCount));
	}

	void OgreNextVertexBuffer::unlock()
	{
		mBuffer->unmap(Ogre::UO_KEEP_PERSISTENT, 0u, mRequestedCount);
		mVao->setPrimitiveRange(0u, static_cast<Ogre::uint32>(mRequestedCount));
	}

} // namespace MyGUI
