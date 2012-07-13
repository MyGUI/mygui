/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/
#ifndef _9ddc5ebb_b8bc_434c_8584_6b203c298deb_
#define _9ddc5ebb_b8bc_434c_8584_6b203c298deb_

#include "Control.h"
#include "SeparatorUtility.h"

namespace tools
{
	class SeparatorControl :
		public Control
	{
	public:
		SeparatorControl();
		virtual ~SeparatorControl();

		virtual void Initialise(Control* _parent, MyGUI::Widget* _place, const std::string& _layoutName);

	private:
		void CreateSeparators();
		void MoveSeparator(MyGUI::Widget* _separator, const MyGUI::IntPoint& _point, SeparatorDirection _modeDirection = Any);

		void notifyMouseButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);
		void notifyMouseDrag(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id);
		void notifyChangeCoord(MyGUI::Widget* _sender);

	private:
		typedef std::vector<MyGUI::Widget*> VectorWidget;
		VectorWidget mChilds;
		bool mHorizontal;
	};
}

#endif
