/*!
	@file
	@author		Albert Semenov
	@date		04/2009
*/

#ifndef MYGUI_OGRE_VERTEX_BUFFER_H_
#define MYGUI_OGRE_VERTEX_BUFFER_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_IVertexBuffer.h"

#include <OgreHardwareBufferManager.h>
#include <OgreHardwareVertexBuffer.h>
#include <OgrePrerequisites.h>
#include <OgreRenderOperation.h>
#include <OgreRenderSystem.h>
#include <OgreTextureManager.h>
#include <OgreTextureUnitState.h>

#include "MyGUI_LastHeader.h"

namespace MyGUI
{

	class OgreVertexBuffer :
		public IVertexBuffer
	{
	public:
		OgreVertexBuffer();
		~OgreVertexBuffer() override;

		void setVertexCount(size_t _count) override;
		size_t getVertexCount() override;

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
		size_t mVertexCount;
		size_t mNeedVertexCount;

		Ogre::RenderOperation mRenderOperation;
		Ogre::HardwareVertexBufferSharedPtr mVertexBuffer;
	};

} // namespace MyGUI

#endif // MYGUI_OGRE_VERTEX_BUFFER_H_
