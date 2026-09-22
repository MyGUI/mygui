#ifndef MYGUI_OGRE_NEXT_VERTEX_BUFFER_H_
#define MYGUI_OGRE_NEXT_VERTEX_BUFFER_H_

#include "MyGUI_OgreNextPrerequisites.h"
#include "MyGUI_IVertexBuffer.h"

#include <vector>

#include "MyGUI_LastHeader.h"

namespace MyGUI
{

	// v2 vertex buffer for MyGUI. Retains VertexBufferPacked/VertexArrayObject slots
	// for each update within a frame, using non-indexed OT_TRIANGLE_LIST draws.
	class OgreNextVertexBuffer : public IVertexBuffer
	{
	public:
		OgreNextVertexBuffer();
		~OgreNextVertexBuffer() override;

		void setVertexCount(size_t _count) override;
		size_t getVertexCount() const override;

		Vertex* lock() override;
		void unlock() override;

		Ogre::VertexArrayObject* getVao() const
		{
			return mVao;
		}

	private:
		void createBuffer(size_t capacity);
		void destroyBuffer();

		struct BufferSlot
		{
			Ogre::VertexBufferPacked* buffer{};
			Ogre::VertexArrayObject* vao{};
			size_t capacity{};
		};
		std::vector<BufferSlot> mSlots;
		uint32 mFrame{};
		size_t mNextSlot{};

		size_t mRequestedCount{0};
		size_t mCapacity{0};

		Ogre::VertexBufferPacked* mBuffer{nullptr};
		Ogre::VertexArrayObject* mVao{nullptr};
	};

} // namespace MyGUI

#endif // MYGUI_OGRE_NEXT_VERTEX_BUFFER_H_
