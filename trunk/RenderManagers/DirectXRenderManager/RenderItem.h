/*!
	@file
	@author		Albert Semenov
	@date		04/2008
	@module
*/

#ifndef __RENDER_ITEM_H__
#define __RENDER_ITEM_H__

#include "MyGUI_IRenderItem.h"
#include "MyGUI_IDrawItem.h"
#include <string>
#include <vector>

namespace MyGUI
{

	class RenderItem : public IRenderItem
	{
	public:
		RenderItem(const std::string& _texture);
		virtual ~RenderItem() { }

		// запрашиваем вершины
		void addDrawItem(IDrawItem* _item, size_t _count);

		// текстура для которой нас создали
		const std::string& getTexture() { return mTexture; }

		// рисуем все что нас напихали
		void doRender(bool _update);

		int* getBuffer();

	private:
		std::string mTexture;
		size_t mCount;
		bool mUpdate;

		int* mData;
		int* mCurrentData;

		typedef std::pair<IDrawItem*, size_t> PairDrawData;
		typedef std::vector<PairDrawData> VectorPairDrawData;
		VectorPairDrawData mDataInfo;

	};

} // namespace MyGUI

#endif // __MYGUI_I_RENDER_ITEM_H__
