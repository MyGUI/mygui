/*!
	@file
	@author		Albert Semenov
	@date		07/2008
	@module
*/
#ifndef __BASE_LAYOUT_H__
#define __BASE_LAYOUT_H__

#include <MyGUI.h>

namespace wraps
{

	class BaseLayout
	{
	protected:
		BaseLayout(const std::string & _layout, MyGUI::WidgetPtr _parent = nullptr) : mMainWidget(nullptr)
		{
			initialise(_layout, _parent);
		}

		template <typename T>
		void assignWidget(T * & _widget, const std::string & _name, bool _throw = true)
		{
			_widget = nullptr;
			for (MyGUI::VectorWidgetPtr::iterator iter=mListWindowRoot.begin(); iter!=mListWindowRoot.end(); ++iter) {
				MyGUI::WidgetPtr find = (*iter)->findWidget(mPrefix + _name);
				if (nullptr != find) {
					T * cast = find->castType<T>(false);
					if (nullptr != cast) {
						_widget = cast;
					}
					else if (_throw) {
							MYGUI_EXCEPT("Error cast : dest type = '" << T::getClassTypeName()
							<< "' source name = '" << find->getName()
							<< "' source type = '" << find->getTypeName() << "' in layout '" << mLayoutName << "'");
					}
					return;

				}
			}
			MYGUI_ASSERT( ! _throw, "widget name '" << _name << "' in layout '" << mLayoutName << "' not found.");
		}

		template <typename T>
		void assignBase(T * & _widget, const std::string & _name, bool _throw = true)
		{
			_widget = nullptr;
			for (MyGUI::VectorWidgetPtr::iterator iter=mListWindowRoot.begin(); iter!=mListWindowRoot.end(); ++iter) {
				MyGUI::WidgetPtr find = (*iter)->findWidget(mPrefix + _name);
				if (nullptr != find) {
					_widget = new T(find);
					mListBase.push_back(_widget);
					return;
				}
			}
			MYGUI_ASSERT( ! _throw, "widget name '" << _name << "' in layout '" << mLayoutName << "' not found.");
		}

		void initialise(const std::string & _layout, MyGUI::WidgetPtr _parent = nullptr)
		{
			const std::string MAIN_WINDOW = "_Main";
			mLayoutName = _layout;

			// оборачиваем
			if (mLayoutName.empty()) {
				mMainWidget = _parent;

			}
			// загружаем лейаут на виджет
			else {
				mPrefix = MyGUI::utility::toString(this, "_");
				mListWindowRoot = MyGUI::LayoutManager::getInstance().loadLayout(mLayoutName, mPrefix, _parent);

				const std::string main_name = mPrefix + MAIN_WINDOW;
				for (MyGUI::VectorWidgetPtr::iterator iter=mListWindowRoot.begin(); iter!=mListWindowRoot.end(); ++iter) {
					if ((*iter)->getName() == main_name) {
						mMainWidget = (*iter);
						break;
					}
				}
				MYGUI_ASSERT(mMainWidget, "root widget name '" << MAIN_WINDOW << "' in layout '" << mLayoutName << "' not found.");
			}
		}

		void shutdown()
		{
			// удаляем все классы
			for (VectorBasePtr::iterator iter=mListBase.begin(); iter!=mListBase.end(); ++iter) {
				delete (*iter);
			}
			mListBase.clear();

			// удаляем все рутовые виджеты
			MyGUI::LayoutManager::getInstance().unloadLayout(mListWindowRoot);
			mListWindowRoot.clear();
		}

	public:
		virtual ~BaseLayout()
		{
			shutdown();
		}

	protected:
		MyGUI::WidgetPtr mMainWidget;

	private:
		std::string mPrefix;
		std::string mLayoutName;
		MyGUI::VectorWidgetPtr mListWindowRoot;
		typedef std::vector<BaseLayout*> VectorBasePtr;
		VectorBasePtr mListBase;
	};

} // namespace wraps

#endif // __BASE_LAYOUT_H__
