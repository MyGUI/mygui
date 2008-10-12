/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include "Base/BaseManager.h"

namespace demo
{

	class DemoKeeper : public base::BaseManager
	{
	public:
		virtual void createScene();
		virtual void destroyScene();

	/*protected:
		void notifyStartDrop(MyGUI::WidgetPtr _sender, const MyGUI::ItemDropInfo & _info, bool & _result);
		void notifyRequestDrop(MyGUI::WidgetPtr _sender, const MyGUI::ItemDropInfo & _info, bool & _result);
		void notifyEndDrop(MyGUI::WidgetPtr _sender, const MyGUI::ItemDropInfo & _info, bool _result);
		void notifyDropState(MyGUI::WidgetPtr _sender, MyGUI::DropState _state);
		void requestDropWidgetInfo(MyGUI::WidgetPtr _sender, MyGUI::VectorDropWidgetInfo & _items);
		void notifyUpdateDropState(MyGUI::WidgetPtr _sender, MyGUI::VectorDropWidgetInfo & _items, const MyGUI::DropWidgetState & _state);
		void notifyToolTip(MyGUI::WidgetPtr _sender, const MyGUI::ToolTipInfo & _info);

	private:
		MyGUI::StaticImagePtr mDropImage;
		MyGUI::IntCoord mDropDimension;
*/
	};

} // namespace demo

#endif // __DEMO_KEEPER_H__
