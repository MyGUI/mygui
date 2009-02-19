/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*//*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __STATISTIC_INFO_H__
#define __STATISTIC_INFO_H__

#include <MyGUI.h>

namespace statistic
{

	class StatisticInfo
	{

	public:
		typedef std::pair<std::string, std::string> PairString;
		typedef std::vector<PairString> VectorPairString;

		StatisticInfo() :
			mInfo(nullptr)
		{
			//MyGUI::Gui::getInstance().load("core_skin_debug.xml");

			const std::string layer = "Statistic";
			if ( ! MyGUI::LayerManager::getInstance().isExist(layer)) return;

			const std::string skin = "StatisticRect";
			if ( ! MyGUI::SkinManager::getInstance().isExist(skin)) return;

			mInfo = MyGUI::Gui::getInstance().createWidget<MyGUI::StaticText>(skin, MyGUI::IntCoord(), MyGUI::Align::Default, layer);

			// если не найдется то поставится Default
			const std::string font = "DejaVuSans.14";
			mInfo->setFontName(font);

		}

		~StatisticInfo()
		{
			if (mInfo) {
				MyGUI::Gui::getInstance().destroyChildWidget(mInfo);
				mInfo = nullptr;
			}
		}

		template <typename T>
		void change(const std::string & _key, const T & _value)
		{
			for (VectorPairString::iterator iter=mParams.begin(); iter!=mParams.end(); ++iter) {
				if (iter->first == _key) {
					iter->second = MyGUI::utility::toString<T>(_value);
					return;
				}
			}
			mParams.push_back(std::make_pair(_key, MyGUI::utility::toString<T>(_value)));
		}

		void remove(const std::string & _key)
		{
			for (VectorPairString::iterator iter=mParams.begin(); iter!=mParams.end(); ++iter) {
				if (iter->first == _key) {
					mParams.erase(iter);
					return;
				}
			}
		}

		void update()
		{
			if (mInfo) {
				std::ostringstream stream;
				for (VectorPairString::iterator iter=mParams.begin(); iter!=mParams.end(); ++iter) {
					if (iter != mParams.begin()) stream << "\n";
					stream << iter->first << "  :  " << iter->second;
				}

				mInfo->setCaption(stream.str());

				MyGUI::ISubWidgetText * text = mInfo->getSubWidgetText();
				if (text) {
					const MyGUI::IntSize& size = text->getTextSize() + mInfo->getSize() - text->getSize();
					mInfo->setCoord(MyGUI::Gui::getInstance().getViewWidth() - size.width - 20, MyGUI::Gui::getInstance().getViewHeight() - size.height - 20, size.width, size.height);
				}
			}
		}

		void clear()
		{
			mParams.clear();
		}

		void clear(const std::string & _key)
		{
			for (VectorPairString::iterator iter=mParams.begin(); iter!=mParams.end(); ++iter) {
				if (iter->first == _key) {
					mParams.erase(iter);
					return;
				}
			}
		}

	private:
		MyGUI::StaticTextPtr mInfo;
		VectorPairString mParams;

	};

} // namespace staticstic

#endif // __STATISTIC_INFO_H__
