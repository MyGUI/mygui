#ifndef MYGUI_OGRE_NEXT_VERTEX_BUFFER_H_
#define MYGUI_OGRE_NEXT_VERTEX_BUFFER_H_

#include "MyGUI_OgreNextPrerequisites.h"
#include "MyGUI_IVertexBuffer.h"

#include "MyGUI_LastHeader.h"

namespace MyGUI
{
	// v2 vertex buffer for MyGUI. Wraps a single VertexBufferPacked (BT_DYNAMIC_PERSISTENT)
	// and a VertexArrayObject that binds it as a non-indexed OT_TRIANGLE_LIST.
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

		size_t mRequestedCount{0};
		size_t mCapacity{0};

		Ogre::VertexBufferPacked* mBuffer{nullptr};
		Ogre::VertexArrayObject* mVao{nullptr};
	};

} // namespace MyGUI

#endif // MYGUI_OGRE_NEXT_VERTEX_BUFFER_H_
