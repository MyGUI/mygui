/*!
	@file
	@author		Albert Semenov
	@date		07/2024
*/

#ifndef MYGUI_OSG_VERTEX_BUFFER_H_
#define MYGUI_OSG_VERTEX_BUFFER_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_IVertexBuffer.h"

#include <osg/Array>
#include <osg/ref_ptr>

namespace osg
{
	class VertexBufferObject;
}

namespace MyGUI
{

	class OsgVertexBuffer : public IVertexBuffer
	{
	public:
		OsgVertexBuffer();
		~OsgVertexBuffer() override;

		void setVertexCount(size_t _count) override;
		size_t getVertexCount() const override;

		Vertex* lock() override;
		void unlock() override;

		/*internal:*/
		osg::Array* getVertexArray();
		osg::VertexBufferObject* getVertexBuffer();
		void markUsed();

	private:
		osg::UByteArray* create();

	private:
		// 4 slots to match the batch ring depth of the drawables, so the vertex data
		// stays valid while a batch referencing it is still waiting to be drawn
		osg::ref_ptr<osg::VertexBufferObject> mBuffer[4];
		osg::ref_ptr<osg::UByteArray> mVertexArray[4];

		size_t mNeedVertexCount{0};

		unsigned int mCurrentBuffer{0};
		bool mUsed{false}; // has the mCurrentBuffer been submitted to the rendering thread
	};

} // namespace MyGUI

#endif // MYGUI_OSG_VERTEX_BUFFER_H_
