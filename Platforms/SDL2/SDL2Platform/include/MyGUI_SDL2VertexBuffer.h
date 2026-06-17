/*!
	@file
	@author		A. Eduardo Garcia Hdez
	@date		03/2022
*/

#ifndef MYGUI_SDL2_VERTEX_BUFFER_H_
#define MYGUI_SDL2_VERTEX_BUFFER_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_IVertexBuffer.h"

#include <SDL.h>
#include <vector>

namespace MyGUI
{

	class SDL2VertexBuffer : public IVertexBuffer
	{
	public:
		SDL2VertexBuffer();
		~SDL2VertexBuffer() override;

		void setVertexCount(size_t _count) override;
		size_t getVertexCount() const override;

		Vertex* lock() override;
		void unlock() override;

	/*internal:*/
		SDL_Vertex* getVertices() {
			return mVertices.data();
		}

	private:
		void create();
		void destroy();
		void resize();

	private:
		size_t mVertexCount;
		size_t mNeedVertexCount;
		size_t mSizeInBytes;

		Vertex *mInternalData;

		std::vector<SDL_Vertex> mVertices;
	};

} // namespace MyGUI

#endif // MYGUI_SDL2_VERTEX_BUFFER_H_
