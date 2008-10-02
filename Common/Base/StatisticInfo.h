/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
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
			mInfo(null)
		{
			//MyGUI::Gui::getInstance().load("core_skin_debug.xml");

			const std::string layer = "Statistic";
			if ( ! MyGUI::LayerManager::getInstance().isExist(layer)) return;

			const std::string skin = "RectBlue";
			if ( ! MyGUI::SkinManager::getInstance().isExist(skin)) return;

			const std::string font = "DejaVuSans.14";
			if ( ! MyGUI::FontManager::getInstance().isExist(font)) return;

			mInfo = MyGUI::Gui::getInstance().createWidget<MyGUI::Widget>(skin, MyGUI::IntCoord(), MyGUI::Align::Default, layer);
			mInfo->setFontName(font);
			//mInfo->setFontHeight(12);
		}

		~StatisticInfo()
		{
			if (mInfo) {
				MyGUI::Gui::getInstance().destroyChildWidget(mInfo);
				mInfo = null;
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

		template<>
		void change(const std::string & _key, const std::string & _value)
		{
			for (VectorPairString::iterator iter=mParams.begin(); iter!=mParams.end(); ++iter) {
				if (iter->first == _key) {
					iter->second = _value;
					return;
				}
			}
			mParams.push_back(std::make_pair(_key, _value));
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

				MyGUI::ISubWidgetText * text = mInfo->_getSubWidgetText();
				if (text) {
					MyGUI::IntSize size = mInfo->_getSubWidgetText()->getSize();
					size = mInfo->getTextSize() + mInfo->getSize() - size;
					mInfo->setPosition(MyGUI::Gui::getInstance().getViewWidth() - size.width - 20, MyGUI::Gui::getInstance().getViewHeight() - size.height - 20, size.width, size.height);
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
		MyGUI::WidgetPtr mInfo;
		VectorPairString mParams;

	};

} // namespace staticstic

#endif // __STATISTIC_INFO_H__
