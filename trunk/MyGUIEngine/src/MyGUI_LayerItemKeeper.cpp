/*!
	@file
	@author		Albert Semenov
	@date		02/2008
	@module
*/

#include "MyGUI_LayerItemKeeper.h"
#include "MyGUI_RenderItem.h"
#include "MyGUI_LayerItem.h"

namespace MyGUI
{

	LayerItemKeeper::LayerItemKeeper() :
		mCountUsing(0)
	{
	}

	LayerItemKeeper::~LayerItemKeeper()
	{
		for (VectorRenderItem::iterator iter=mFirstRenderItems.begin(); iter!=mFirstRenderItems.end(); ++iter) {
			delete (*iter);
		}
		mFirstRenderItems.clear();

		for (VectorRenderItem::iterator iter=mSecondRenderItems.begin(); iter!=mSecondRenderItems.end(); ++iter) {
			delete (*iter);
		}
		mSecondRenderItems.clear();
	}

	void LayerItemKeeper::_render(bool _update)
	{
		for (VectorRenderItem::iterator iter=mFirstRenderItems.begin(); iter!=mFirstRenderItems.end(); ++iter) {
			(*iter)->_render(_update);
		}
		for (VectorRenderItem::iterator iter=mSecondRenderItems.begin(); iter!=mSecondRenderItems.end(); ++iter) {
			(*iter)->_render(_update);
		}
	}

	RenderItem * LayerItemKeeper::addToRenderItem(const std::string& _texture, bool _first, bool _separate)
	{
		// ��� ��������� ������� ����� �������
		if (_first) {
			if (mFirstRenderItems.empty()) {

				// ������� ����� �����
				RenderItem * item = new RenderItem(_texture, this);
				mFirstRenderItems.push_back(item);

				return item;
			}

			// ���� ��������� ����� ������, �� ���� �� ���������
			if (mFirstRenderItems.back()->getNeedVertexCount() == 0) {
				// ������ ����� ���� ������� ������, ����� ����� ������ �� ������
				for (VectorRenderItem::iterator iter=mFirstRenderItems.begin(); iter!=mFirstRenderItems.end(); ++iter) {
					if ((*iter)->getNeedVertexCount() == 0) {
						(*iter)->setTextureName(_texture);
						return (*iter);
					}
				}
			}

			// �� �� ��������
			if (mFirstRenderItems.back()->getTextureName() == _texture) {
				return mFirstRenderItems.back();
			}

			// ������� ����� �����
			RenderItem * item = new RenderItem(_texture, this);
			mFirstRenderItems.push_back(item);

			return item;
		}

		// ��� ������ ������� ������� �������
		for (VectorRenderItem::iterator iter=mSecondRenderItems.begin(); iter!=mSecondRenderItems.end(); ++iter) {
			// ���� ����� �� ��������, ���� ������ �����
			if ((*iter)->getTextureName() == _texture) {
				return (*iter);
			}
			else if ((*iter)->getNeedVertexCount() == 0) {
				(*iter)->setTextureName(_texture);
				return (*iter);
			}
			
		}
		// �� �������� �������� �����
		mSecondRenderItems.push_back(new RenderItem(_texture, this));
		return mSecondRenderItems.back();
	}

	LayerItem * LayerItemKeeper::_findLayerItem(int _left, int _top, LayerItem* &_root)
	{
		for (VectorLayerItem::iterator iter=mPeekLayerItems.begin(); iter!=mPeekLayerItems.end(); ++iter) {
			LayerItem * item = (*iter)->_findLayerItem(_left, _top);
			if (null != item) {
				_root = (*iter);
				return item;
			}
		}

		return null;
	}

	void LayerItemKeeper::_update()
	{
		// ������ ������������� �� ������ ������

		if (mFirstRenderItems.size() > 1) {
			// �������� ������� ������ ����� ���� ������
			VectorRenderItem::iterator iter1 = mFirstRenderItems.begin();
			VectorRenderItem::iterator iter2 = iter1 + 1;
			while (iter2 != mFirstRenderItems.end()) {
				if ((*iter1)->getNeedVertexCount() == 0) {
					RenderItem * tmp = (*iter1);
					(*iter1) = (*iter2);
					(*iter2) = tmp;
				}
				iter1 = iter2;
				++iter2;
			}
		}
	}

} // namespace MyGUI
