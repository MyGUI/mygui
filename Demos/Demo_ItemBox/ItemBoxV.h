/*!
	@file
	@author		Albert Semenov
	@date		07/2008
	@module
*/
#ifndef __ITEM_BOXV_H__
#define __ITEM_BOXV_H__

#include <MyGUI.h>
#include "BaseItemBox.h"
#include "CellView.h"

class ItemBoxV : public wraps::BaseLayout
{
public:
	ItemBoxV();

	virtual void initialise();
	wraps::BaseItemBox<CellView> & getItemBox() { return mItemBox; }

private:
	wraps::BaseItemBox<CellView> mItemBox;

};

#endif // __ITEM_BOXV_H__
