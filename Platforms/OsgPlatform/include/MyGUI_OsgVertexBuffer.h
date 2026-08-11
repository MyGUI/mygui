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
		// double buffering approach, to avoid the need for synchronization with the draw thread
		osg::ref_ptr<osg::VertexBufferObject> mBuffer[2];
		osg::ref_ptr<osg::UByteArray> mVertexArray[2];

		size_t mNeedVertexCount{0};

		unsigned int mCurrentBuffer{0};
		bool mUsed{false}; // has the mCurrentBuffer been submitted to the rendering thread
	};

} // namespace MyGUI

#endif // MYGUI_OSG_VERTEX_BUFFER_H_
