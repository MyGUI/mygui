/*!
	@file
	@author		Albert Semenov
	@date		04/2009
*/

#ifndef MYGUI_OGRE_VERTEX_BUFFER_H_
#define MYGUI_OGRE_VERTEX_BUFFER_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_IVertexBuffer.h"

#include <OgrePrerequisites.h>
#include <OgreRenderOperation.h>
#include <OgreHardwareVertexBuffer.h>

namespace MyGUI
{

	class OgreVertexBuffer : public IVertexBuffer
	{
	public:
		~OgreVertexBuffer() override;

		void setVertexCount(size_t _count) override;
		size_t getVertexCount() const override;

		Vertex* lock() override;
		void unlock() override;

		Ogre::RenderOperation* getRenderOperation()
		{
			return &mRenderOperation;
		}

	private:
		void create();
		void destroy();
		void resize();

	private:
		size_t mVertexCount{0};
		size_t mNeedVertexCount{0};

		Ogre::RenderOperation mRenderOperation;
		Ogre::HardwareVertexBufferSharedPtr mVertexBuffer;
	};

} // namespace MyGUI

#endif // MYGUI_OGRE_VERTEX_BUFFER_H_
