/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_RENDER_ITEM_H_
#define MYGUI_RENDER_ITEM_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_ISubWidget.h"
#include "MyGUI_IVertexBuffer.h"
#include "MyGUI_VertexData.h"
#include "MyGUI_IRenderTarget.h"

namespace MyGUI
{

	using DrawItemInfo = std::pair<ISubWidget*, size_t>;
	using VectorDrawItem = std::vector<DrawItemInfo>;

	class MYGUI_EXPORT RenderItem
	{
	public:
		RenderItem();
		virtual ~RenderItem();

		void renderToTarget(IRenderTarget* _target, bool _update);

		void setTexture(ITexture* _value);
		ITexture* getTexture() const;

		void setManualRender(bool _value);
		bool getManualRender() const;

		void addDrawItem(ISubWidget* _item, size_t _count);
		void removeDrawItem(ISubWidget* _item);
		void reallockDrawItem(ISubWidget* _item, size_t _count);

		void outOfDate();
		bool isOutOfDate() const;

		size_t getNeedVertexCount() const;
		size_t getVertexCount() const;

		bool getCurrentUpdate() const;
		Vertex* getCurrentVertexBuffer() const;

		void setLastVertexCount(size_t _count);

		IRenderTarget* getRenderTarget();

		void setNeedCompression(bool _compression);
		bool getNeedCompression() const;

	private:
		ITexture* mTexture{nullptr};

		size_t mNeedVertexCount{0};

		bool mOutOfDate{false};
		VectorDrawItem mDrawItems;

		// колличество отрендренных реально вершин
		size_t mCountVertex{0};

		bool mCurrentUpdate{true};
		Vertex* mCurrentVertex{nullptr};
		size_t mLastVertexCount{0};

		IVertexBuffer* mVertexBuffer{nullptr};
		IRenderTarget* mRenderTarget{nullptr};

		bool mNeedCompression{false};
		bool mManualRender{false};
	};

} // namespace MyGUI

#endif // MYGUI_RENDER_ITEM_H_
