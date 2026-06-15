/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/

#ifndef STATISTIC_INFO_H_
#define STATISTIC_INFO_H_

#include <MyGUI.h>

namespace diagnostic
{

	class StatisticInfo
	{
	public:
		StatisticInfo()
		{
			std::string_view layer = "Statistic";
			if (!MyGUI::LayerManager::getInstance().isExist(layer))
				return;

			mInfo = MyGUI::Gui::getInstance()
						.createWidget<MyGUI::TextBox>("TextBox", MyGUI::IntCoord(), MyGUI::Align::Default, layer);
			mInfo->setTextColour(MyGUI::Colour::White);
			mInfo->setTextShadow(true);
		}

		~StatisticInfo()
		{
			if (mInfo != nullptr)
			{
				MyGUI::Gui::getInstance().destroyChildWidget(mInfo);
				mInfo = nullptr;
			}
		}

		template<typename T>
		void change(std::string_view _key, const T& _value)
		{
			for (auto& param : mParams)
			{
				if (param.first == _key)
				{
					param.second = MyGUI::utility::toString<T>(_value);
					return;
				}
			}
			mParams.emplace_back(_key, MyGUI::utility::toString<T>(_value));
		}

		void remove(std::string_view _key)
		{
			for (MyGUI::VectorStringPairs::iterator iter = mParams.begin(); iter != mParams.end(); ++iter)
			{
				if (iter->first == _key)
				{
					mParams.erase(iter);
					return;
				}
			}
		}

		void update()
		{
			if (mInfo != nullptr)
			{
				std::ostringstream stream;
				for (MyGUI::VectorStringPairs::iterator iter = mParams.begin(); iter != mParams.end(); ++iter)
				{
					if (iter != mParams.begin())
						stream << "\n";
					stream << iter->first << " : " << iter->second;
				}

				mInfo->setCaption(stream.str());

				MyGUI::ISubWidgetText* text = mInfo->getSubWidgetText();
				if (text != nullptr)
				{
					MyGUI::IntSize size = text->getTextSize() + mInfo->getSize() - text->getSize();
					const MyGUI::IntSize& size_view = MyGUI::RenderManager::getInstance().getViewSize();
					MyGUI::IntCoord coord(
						size_view.width - size.width - mOffset.left,
						size_view.height - size.height - mOffset.top,
						size.width,
						size.height);
					if (coord != mInfo->getCoord())
						mInfo->setCoord(coord);
				}
			}
		}

		void clear()
		{
			mParams.clear();
		}

		void clear(std::string_view _key)
		{
			for (MyGUI::VectorStringPairs::iterator iter = mParams.begin(); iter != mParams.end(); ++iter)
			{
				if (iter->first == _key)
				{
					mParams.erase(iter);
					return;
				}
			}
		}

		void setVisible(bool _value)
		{
			if (mInfo != nullptr)
				mInfo->setVisible(_value);
		}

		bool getVisible()
		{
			if (mInfo != nullptr)
				return mInfo->getVisible();
			return false;
		}

		void setOffset(const MyGUI::IntPoint& _value)
		{
			mOffset = _value;
		}

	private:
		MyGUI::TextBox* mInfo{nullptr};
		MyGUI::VectorStringPairs mParams;
		MyGUI::IntPoint mOffset{20, 20};
	};

} // namespace diagnostic

#endif // STATISTIC_INFO_H_
