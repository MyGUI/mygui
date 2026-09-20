/*!
	@file
	@author		Albert Semenov
	@date		07/2024
*/

#include "MyGUI_OsgVertexBuffer.h"

#include <osg/Array>
#include <osg/BufferObject>
#include <osg/GL>
#include <osg/State>
#include <osg/observer_ptr>
#include <algorithm>
#include <cstring>
#include <utility>

namespace MyGUI
{

	// Keep allocation, eviction and context teardown in OSG's buffer manager.
	// Subsequent full-buffer updates use discard/map instead of glBufferSubData.
	class StreamVertexBufferObject : public osg::VertexBufferObject
	{
	public:
		void resizeGLObjectBuffers(unsigned int maxSize) override
		{
			osg::VertexBufferObject::resizeGLObjectBuffers(maxSize);
			mUploaded.resize(maxSize);
		}

		void bind(osg::State* state)
		{
			const unsigned int context = state->getContextID();
			const bool recreated = getGLBufferObject(context) == nullptr;
			osg::GLBufferObject* object = getOrCreateGLBufferObject(context);
			const osg::BufferData* data = getBufferData(0);
			const unsigned int size = data->getTotalDataSize();
			auto& uploaded = mUploaded[context];
			if (recreated || uploaded.object.get() != object || uploaded.size != size)
			{
				// Initialize OSG's size/profile accounting as well as the GL storage.
				object->compileBuffer();
				state->setCurrentVertexBufferObject(object);
			}
			else if (uploaded.modifiedCount != data->getModifiedCount())
			{
				osg::GLExtensions* extensions = state->get<osg::GLExtensions>();
				object->bindBuffer();
				// OSG keeps a high-water allocation size when an array shrinks. Keep
				// that capacity too, so a later compileBuffer() can safely grow back.
				const auto capacity = object->getProfile()._size;
				extensions->glBufferData(GL_ARRAY_BUFFER, capacity, nullptr, GL_STREAM_DRAW);
				bool copied = false;
				if (extensions->glMapBuffer && extensions->glUnmapBuffer)
				{
					if (void* mapped = extensions->glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY))
					{
						std::memcpy(mapped, data->getDataPointer(), size);
						copied = extensions->glUnmapBuffer(GL_ARRAY_BUFFER) == GL_TRUE;
					}
				}
				// Mapping is optional; a failed unmap means the contents were lost.
				if (!copied)
				{
					extensions->glBufferData(GL_ARRAY_BUFFER, capacity, nullptr, GL_STREAM_DRAW);
					extensions->glBufferSubData(GL_ARRAY_BUFFER, 0, size, data->getDataPointer());
				}
				state->setCurrentVertexBufferObject(object);
			}
			else if (state->getCurrentVertexBufferObject() != object)
			{
				object->bindBuffer();
				state->setCurrentVertexBufferObject(object);
			}
			// Track the uploaded array revision ourselves: OSG's non-virtual
			// compileBuffer() would otherwise repeat its subdata upload on bind.
			uploaded.object = object;
			uploaded.modifiedCount = data->getModifiedCount();
			uploaded.size = size;
		}

	private:
		struct Uploaded
		{
			osg::observer_ptr<osg::GLBufferObject> object;
			unsigned int modifiedCount{};
			unsigned int size{};
		};
		osg::buffered_object<Uploaded> mUploaded;
	};

	void bindOsgVertexBuffer(osg::State* _state, osg::VertexBufferObject* _buffer)
	{
		if (auto* stream = dynamic_cast<StreamVertexBufferObject*>(_buffer))
			stream->bind(_state);
		else
			_state->bindVertexBufferObject(_buffer->getOrCreateGLBufferObject(_state->getContextID()));
	}

	OsgVertexBuffer::OsgVertexBuffer()
	{
	}

	OsgVertexBuffer::~OsgVertexBuffer()
	{
	}

	void OsgVertexBuffer::setVertexCount(size_t _count)
	{
		if (_count == mNeedVertexCount)
			return;

		mNeedVertexCount = _count;
	}

	size_t OsgVertexBuffer::getVertexCount() const
	{
		return mNeedVertexCount;
	}

	Vertex* OsgVertexBuffer::lock()
	{
		// Return nullptr so the caller does not write through an
		// empty array; RenderItem already checks the return value for this.
		if (mNeedVertexCount == 0)
			return nullptr;

		osg::UByteArray* array = mCurrent.array;
		// Pending drawables retain the array. Never overwrite vertices still in flight.
		if (!array || array->referenceCount() > 1)
		{
			if (array)
				mRetiredBuffers.push_back(std::move(mCurrent));
			auto free = std::find_if(
				mRetiredBuffers.begin(),
				mRetiredBuffers.end(),
				[](const Storage& storage) { return storage.array->referenceCount() == 1; });
			if (free != mRetiredBuffers.end())
			{
				mCurrent = std::move(*free);
				mRetiredBuffers.erase(free);
				array = mCurrent.array;
			}
			else
				array = create();
		}
		if (array->size() != mNeedVertexCount * sizeof(Vertex))
		{
			array->resize(mNeedVertexCount * sizeof(Vertex));
		}
		trimRetiredBuffers();

		return static_cast<Vertex*>(static_cast<void*>(&(*array)[0]));
	}

	void OsgVertexBuffer::unlock()
	{
		if (mCurrent.array.valid())
		{
			mCurrent.array->dirty();
			mCurrent.buffer->dirty();
		}
	}

	osg::UByteArray* OsgVertexBuffer::create()
	{
		mCurrent.array = new osg::UByteArray(static_cast<unsigned int>(mNeedVertexCount * sizeof(Vertex)));

		mCurrent.buffer = new StreamVertexBufferObject;
		mCurrent.buffer->setDataVariance(osg::Object::DYNAMIC);
		mCurrent.buffer->setUsage(GL_STREAM_DRAW);
		// The VBO does not own the array; the pool and submitted batches do.
		mCurrent.buffer->setArray(0, mCurrent.array.get());

		return mCurrent.array;
	}

	void OsgVertexBuffer::trimRetiredBuffers()
	{
		// Bound idle capacity after a temporary backlog. In-flight buffers remain
		// eligible for reuse once their last batch releases them.
		constexpr size_t spareByteLimit = 1024 * 1024;
		size_t spareBytes = 0;
		size_t spareCount = 0;
		for (auto it = mRetiredBuffers.begin(); it != mRetiredBuffers.end();)
		{
			if (it->array->referenceCount() == 1)
			{
				const size_t bytes = it->array->capacity();
				if (spareCount == 2 || bytes > spareByteLimit - spareBytes)
				{
					it = mRetiredBuffers.erase(it);
					continue;
				}
				++spareCount;
				spareBytes += bytes;
			}
			++it;
		}
	}

	osg::Array* OsgVertexBuffer::getVertexArray()
	{
		return mCurrent.array;
	}

	osg::VertexBufferObject* OsgVertexBuffer::getVertexBuffer()
	{
		return mCurrent.buffer;
	}

} // namespace MyGUI
