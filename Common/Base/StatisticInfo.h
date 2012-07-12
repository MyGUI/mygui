/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/

#ifndef __STATISTIC_INFO_H__
#define __STATISTIC_INFO_H__

#include <MyGUI.h>

namespace diagnostic
{

	class StatisticInfo
	{
	public:

		StatisticInfo() :
			mInfo(nullptr),
			mOffset(20, 20)
		{
			const std::string layer = "Statistic";
			if (!MyGUI::LayerManager::getInstance().isExist(layer))
				return;

			mInfo = MyGUI::Gui::getInstance().createWidget<MyGUI::TextBox>("TextBox", MyGUI::IntCoord(), MyGUI::Align::Default, layer);
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

		template <typename T>
		void change(const std::string& _key, const T& _value)
		{
			for (MyGUI::VectorStringPairs::iterator iter = mParams.begin(); iter != mParams.end(); ++iter)
			{
				if (iter->first == _key)
				{
					iter->second = MyGUI::utility::toString<T>(_value);
					return;
				}
			}
			mParams.push_back(std::make_pair(_key, MyGUI::utility::toString<T>(_value)));
		}

		void remove(const std::string& _key)
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
					if (iter != mParams.begin()) stream << "\n";
					stream << iter->first << " : " << iter->second;
				}

				mInfo->setCaption(stream.str());

				MyGUI::ISubWidgetText* text = mInfo->getSubWidgetText();
				if (text != nullptr)
				{
					const MyGUI::IntSize& size = text->getTextSize() + mInfo->getSize() - text->getSize();
					const MyGUI::IntSize& size_view = MyGUI::RenderManager::getInstance().getViewSize();
					MyGUI::IntCoord coord(size_view.width - size.width - mOffset.left, size_view.height - size.height - mOffset.top, size.width, size.height);
					if (coord != mInfo->getCoord())
						mInfo->setCoord(coord);
				}
			}
		}

		void clear()
		{
			mParams.clear();
		}

		void clear(const std::string& _key)
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
		MyGUI::TextBox* mInfo;
		MyGUI::VectorStringPairs mParams;
		MyGUI::IntPoint mOffset;
	};

} // namespace diagnostic

#endif // __STATISTIC_INFO_H__
