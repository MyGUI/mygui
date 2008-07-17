/*!
	@file
	@author		Albert Semenov
	@date		05/2008
	@module
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include <MyGUI.h>

class DemoKeeper
{
public:
	// структура с данными одного айтема
	struct ItemData
	{
		ItemData(size_t _type, size_t _count) : type(_type), count(_count) {}
		size_t type;
		size_t count;
	};

	// структура для удобного поиска составляющий айтема
	struct WidgetData
	{
		WidgetData() : item1(null), item2(null), item3(null), text1(null), text2(null) {}
		MyGUI::StaticImagePtr item1, item2, item3;
		MyGUI::StaticTextPtr text1, text2;
	};

public:
	void start();
	void end();

private:
	void requestCreateWidgetItem(MyGUI::WidgetPtr _sender, MyGUI::WidgetItemData & _info);
	void requestCoordWidgetItem(MyGUI::WidgetPtr _sender, MyGUI::IntCoord & _coord, bool _drop);
	void requestUpdateWidgetItem(MyGUI::WidgetPtr _sender, MyGUI::WidgetItemData _item, const MyGUI::ItemInfo& _data);

	void eventStartDrop(MyGUI::WidgetPtr _sender, const MyGUI::ItemDropInfo & _info, bool & _result);
	void eventRequestDrop(MyGUI::WidgetPtr _sender, const MyGUI::ItemDropInfo & _info, bool & _result);
	void eventEndDrop(MyGUI::WidgetPtr _sender, const MyGUI::ItemDropInfo & _info, bool _result);
	void eventDropState(MyGUI::WidgetPtr _sender, MyGUI::DropState _state);

	void eventToolTip(MyGUI::WidgetPtr _sender, MyGUI::ToolTipInfo _info);
};

#endif // __DEMO_KEEPER_H__
