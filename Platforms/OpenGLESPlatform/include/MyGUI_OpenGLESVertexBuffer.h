#pragma once

#include "MyGUI_Prerequest.h"
#include "MyGUI_IVertexBuffer.h"
#ifdef __EMSCRIPTEN__
	#include <vector>
#endif

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
		bool mLocked{false};
#ifdef __EMSCRIPTEN__
		// WebGL cannot map GPU buffers. Reuse CPU storage for lock(), then upload in unlock()
		// without requiring Emscripten's FULL_ES3 mapping emulation.
		std::vector<Vertex> mVertices;
#endif
	};

} // namespace MyGUI
