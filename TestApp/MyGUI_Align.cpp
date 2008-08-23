/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#include "MyGUI_Align.h"

namespace MyGUI
{

	Align2::MapAlign2 Align2::mMapAlign;

	Align2 Align2::parse(const std::string & _value)
	{
		static bool first = true;
		if (first) {
			initialise();
			first = false;
		}

		Align2 flag = 0;
		const std::vector<std::string> & vec = utility::split(_value);
		for (size_t pos=0; pos<vec.size(); pos++) {
			MapAlign2::iterator iter = mMapAlign.find(vec[pos]);
			if (iter != mMapAlign.end()) flag |= iter->second;
			else {
				MYGUI_LOG(Warning, "Cannot parse align '" << vec[pos] << "'");
			}
		}
		return flag;
	}

	void Align2::initialise()
	{
		mMapAlign.clear();

		// old
		MYGUI_REGISTER_VALUE(mMapAlign, ALIGN_HCENTER);
		MYGUI_REGISTER_VALUE(mMapAlign, ALIGN_VCENTER);
		MYGUI_REGISTER_VALUE(mMapAlign, ALIGN_CENTER);
		MYGUI_REGISTER_VALUE(mMapAlign, ALIGN_CENTER_PARENT);
		MYGUI_REGISTER_VALUE(mMapAlign, ALIGN_LEFT);
		MYGUI_REGISTER_VALUE(mMapAlign, ALIGN_RIGHT);
		MYGUI_REGISTER_VALUE(mMapAlign, ALIGN_HSTRETCH);
		MYGUI_REGISTER_VALUE(mMapAlign, ALIGN_TOP);
		MYGUI_REGISTER_VALUE(mMapAlign, ALIGN_BOTTOM);
		MYGUI_REGISTER_VALUE(mMapAlign, ALIGN_VSTRETCH);
		MYGUI_REGISTER_VALUE(mMapAlign, ALIGN_STRETCH);
		MYGUI_REGISTER_VALUE(mMapAlign, ALIGN_DEFAULT);

		// new
		MYGUI_REGISTER_VALUE(mMapAlign, HCenter);
		MYGUI_REGISTER_VALUE(mMapAlign, VCenter);
		MYGUI_REGISTER_VALUE(mMapAlign, Center);
		MYGUI_REGISTER_VALUE(mMapAlign, CenterParent);
		MYGUI_REGISTER_VALUE(mMapAlign, Left);
		MYGUI_REGISTER_VALUE(mMapAlign, Right);
		MYGUI_REGISTER_VALUE(mMapAlign, HStretch);
		MYGUI_REGISTER_VALUE(mMapAlign, Top);
		MYGUI_REGISTER_VALUE(mMapAlign, Bottom);
		MYGUI_REGISTER_VALUE(mMapAlign, VStretch);
		MYGUI_REGISTER_VALUE(mMapAlign, Stretch);
		MYGUI_REGISTER_VALUE(mMapAlign, Default);
		MYGUI_REGISTER_VALUE(mMapAlign, LeftTop);
		MYGUI_REGISTER_VALUE(mMapAlign, RightTop);
		MYGUI_REGISTER_VALUE(mMapAlign, RightBottom);
		MYGUI_REGISTER_VALUE(mMapAlign, LeftBottom);
	}

	std::string Align2::print() const
	{
		std::string ret;

		if (align & Left) {
			if (align & Right) ret = "HStretch";
			else ret = "Left";
		}
		else if (align & Right) ret = "Right";
		else ret = "HCenter";

		if (align & Top) {
			if (align & Bottom) ret += " VStretch";
			else ret += " Top";
		}
		else if (align & Bottom) ret += " Bottom";
		else ret += " VCenter";

		return ret;
	}

} // namespace MyGUI