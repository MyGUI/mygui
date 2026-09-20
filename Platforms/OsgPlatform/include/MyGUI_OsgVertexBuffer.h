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
#include <vector>

namespace osg
{

	class VertexBufferObject;
	class State;

}

namespace MyGUI
{

	// Upload and bind one retained MyGUI vertex array in the current draw context.
	void bindOsgVertexBuffer(osg::State* _state, osg::VertexBufferObject* _buffer);

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

	private:
		osg::UByteArray* create();
		void trimRetiredBuffers();

	private:
		struct Storage
		{
			osg::ref_ptr<osg::VertexBufferObject> buffer;
			osg::ref_ptr<osg::UByteArray> array;
		};
		Storage mCurrent;
		std::vector<Storage> mRetiredBuffers;
		size_t mNeedVertexCount{0};
	};

} // namespace MyGUI

#endif // MYGUI_OSG_VERTEX_BUFFER_H_
