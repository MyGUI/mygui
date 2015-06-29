/*!
	@file
	@author		Albert Semenov
	@date		04/2009
*/

#ifndef MYGUI_OGRE2_VERTEX_BUFFER_H_
#define MYGUI_OGRE2_VERTEX_BUFFER_H_

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

	class Ogre2VertexBuffer :
		public IVertexBuffer
	{
	public:
		Ogre2VertexBuffer();
		virtual ~Ogre2VertexBuffer();

		virtual void setVertexCount(size_t _count);
		virtual void setOperationVertexCount(size_t _count);
		virtual size_t getVertexCount();

		virtual Vertex* lock();
		virtual void unlock();

		Ogre::v1::RenderOperation* getRenderOperation()
		{
			return &mRenderOperation;
		}

		unsigned int getUniqueID() const {
			return mUniqueID;
		}

	private:
		void createVertexBuffer();
		void destroyVertexBuffer();
		void resizeVertexBuffer();

	private:
		static unsigned int GlobalID;

	private:
		unsigned int mUniqueID;

		size_t mVertexCount;
		size_t mNeedVertexCount;

		Ogre::v1::RenderOperation mRenderOperation;
		Ogre::v1::HardwareVertexBufferSharedPtr mVertexBuffer;
	};

} // namespace MyGUI

#endif // MYGUI_OGRE_VERTEX_BUFFER_H_
