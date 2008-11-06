/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#include "MyGUI_Align.h"

namespace MyGUI
{

	Align::MapAlign Align::mMapAlign;

	Align Align::parse(const std::string & _value)
	{
		static bool first = true;
		if (first) {
			initialise();
			first = false;
		}

		Align flag = 0;
		const std::vector<std::string> & vec = utility::split(_value);
		for (size_t pos=0; pos<vec.size(); pos++) {
			MapAlign::iterator iter = mMapAlign.find(vec[pos]);
			if (iter != mMapAlign.end()) flag |= iter->second;
			else {
				MYGUI_LOG(Warning, "Cannot parse align '" << vec[pos] << "'");
			}
		}
		return flag;
	}

	void Align::initialise()
	{
		mMapAlign.clear();

		// old
		mMapAlign["ALIGN_HCENTER"] = HCenter;
		mMapAlign["ALIGN_VCENTER"] = VCenter;
		mMapAlign["ALIGN_CENTER"] = Center;
		mMapAlign["ALIGN_LEFT"] = Left;
		mMapAlign["ALIGN_RIGHT"] = Right;
		mMapAlign["ALIGN_HSTRETCH"] = HStretch;
		mMapAlign["ALIGN_TOP"] = Top;
		mMapAlign["ALIGN_BOTTOM"] = Bottom;
		mMapAlign["ALIGN_VSTRETCH"] = VStretch;
		mMapAlign["ALIGN_STRETCH"] = Stretch;
		mMapAlign["ALIGN_DEFAULT"] = Default;

		MYGUI_REGISTER_VALUE(mMapAlign, HCenter);
		MYGUI_REGISTER_VALUE(mMapAlign, VCenter);
		MYGUI_REGISTER_VALUE(mMapAlign, Center);
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

	std::string Align::print() const
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
