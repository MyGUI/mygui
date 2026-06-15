#pragma once

#include "MyGUI_Prerequest.h"
#include "MyGUI_IVertexBuffer.h"

namespace MyGUI
{

	class OpenGLESVertexBuffer : public IVertexBuffer
	{
	public:
		~OpenGLESVertexBuffer() override;

		void setVertexCount(size_t _count) override;
		size_t getVertexCount() const override;

		Vertex* lock() override;
		void unlock() override;

		/*internal:*/
		unsigned int getBufferID() const
		{
			return mVAOID;
		}

	private:
		void create();
		void destroy();
		void resize();

	private:
		unsigned int mVAOID{0};
		unsigned int mBufferID{0};
		size_t mVertexCount{0};
		size_t mNeedVertexCount{0};
		size_t mSizeInBytes{0};
	};

} // namespace MyGUI
