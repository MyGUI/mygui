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
#include "MyGUI_Precompiled.h"
#include "MyGUI_OgreVertexBuffer.h"

#include "Ogre.h"

#include "MyGUI_LastHeader.h"

namespace MyGUI
{

	const size_t VERTEX_IN_QUAD = 6;
	const size_t RENDER_ITEM_STEEP_REALLOCK = 5 * VERTEX_IN_QUAD;

	OgreVertexBuffer::OgreVertexBuffer() :
		mNeedVertexCount(0),
		mVertexCount(RENDER_ITEM_STEEP_REALLOCK)
	{
		createVertexBuffer();
	}

	OgreVertexBuffer::~OgreVertexBuffer()
	{
		destroyVertexBuffer();
	}

	void OgreVertexBuffer::createVertexBuffer()
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

	void OgreVertexBuffer::destroyVertexBuffer()
	{
		delete mRenderOperation.vertexData;
		mRenderOperation.vertexData = 0;
		mVertexBuffer.setNull();
	}

	void OgreVertexBuffer::resizeVertexBuffer()
	{
		mVertexCount = mNeedVertexCount + RENDER_ITEM_STEEP_REALLOCK;
		destroyVertexBuffer();
		createVertexBuffer();
	}

	void OgreVertexBuffer::setVertextCount(size_t _count)
	{
		mNeedVertexCount = _count;
	}

	size_t OgreVertexBuffer::getVertextCount()
	{
		return mNeedVertexCount;
	}

	Vertex* OgreVertexBuffer::lock()
	{
		if (mNeedVertexCount > mVertexCount) resizeVertexBuffer();

		return (Vertex*)mVertexBuffer->lock(Ogre::HardwareVertexBuffer::HBL_DISCARD);
	}

	void OgreVertexBuffer::unlock()
	{
		mVertexBuffer->unlock();
	}

} // namespace MyGUI
