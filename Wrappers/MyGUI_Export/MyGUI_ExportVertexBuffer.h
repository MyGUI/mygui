/*!
	@file
	@author		Albert Semenov
	@date		08/2012
*/

#ifndef _e31e1880_266b_4159_8bf6_bdc04ea3332e_
#define _e31e1880_266b_4159_8bf6_bdc04ea3332e_

#include "MyGUI_Prerequest.h"
#include "MyGUI_IVertexBuffer.h"

namespace MyGUI
{

	class ExportVertexBuffer :
		public IVertexBuffer
	{
	public:
		ExportVertexBuffer();
		~ExportVertexBuffer() override;

		void setVertexCount(size_t _count) override;
		size_t getVertexCount() override;

		Vertex* lock() override;
		void unlock() override;

		/*internal:*/
		size_t getId();
		void setChange(bool _value);
		bool getChange() const;

	private:
		void createVertexBuffer();
		void destroyVertexBuffer();
		void resizeVertexBuffer();

	private:
		size_t mVertexCount;
		size_t mNeedVertexCount;
		size_t mSizeInBytes;
		size_t mId;
		Vertex* mVertex;
		bool mChange;
	};

}

#endif
