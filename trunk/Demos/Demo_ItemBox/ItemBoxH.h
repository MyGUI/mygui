/*!
	@file
	@author		Albert Semenov
	@date		07/2008
	@module
*/
#ifndef __ITEM_BOXH_H__
#define __ITEM_BOXH_H__

#include <MyGUI.h>
#include "BaseItemBox.h"
#include "CellView.h"

class ItemBoxH : public wraps::BaseLayout
{
public:
	ItemBoxH();

	virtual void initialise();
	wraps::BaseItemBox<CellView> & getItemBox() { return mItemBox; }

private:
	wraps::BaseItemBox<CellView> mItemBox;

};

#endif // __ITEM_BOXH_H__
