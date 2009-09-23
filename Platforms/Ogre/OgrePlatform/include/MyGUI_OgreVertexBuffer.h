/*!
	@file
	@author		Albert Semenov
	@date		04/2009
	@module
*/
/*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __MYGUI_OGRE_VERTEX_BUFFER_H__
#define __MYGUI_OGRE_VERTEX_BUFFER_H__

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

	class OgreVertexBuffer : public IVertexBuffer
	{
	public:
		OgreVertexBuffer();
		virtual ~OgreVertexBuffer();

		virtual void setVertextCount(size_t _count);
		virtual size_t getVertextCount();

		virtual Vertex* lock();
		virtual void unlock();

		Ogre::RenderOperation* getRenderOperation() { return &mRenderOperation; }

	private:
		void createVertexBuffer();
		void destroyVertexBuffer();
		void resizeVertexBuffer();

	private:
		size_t mVertexCount;
		size_t mNeedVertexCount;

		Ogre::RenderOperation mRenderOperation;
		Ogre::HardwareVertexBufferSharedPtr mVertexBuffer;
	};

} // namespace MyGUI

#endif // __MYGUI_OGRE_VERTEX_BUFFER_H__
