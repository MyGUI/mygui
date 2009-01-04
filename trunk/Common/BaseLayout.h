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

	class BaseLayout2
	{
	protected:
		BaseLayout2(const std::string & _layout, MyGUI::WidgetPtr _parent = null);

		template <typename T>
		void assignWidget(T * & _widget, const std::string & _name, bool _throw = true)
		{
			_widget = null;
			for (MyGUI::VectorWidgetPtr::iterator iter=mListWindowRoot.begin(); iter!=mListWindowRoot.end(); ++iter) {
				MyGUI::WidgetPtr find = (*iter)->findWidget(mPrefix + _name);
				if (null != find) {
					T * cast = find->castType<T>(false);
					if (null != cast) {
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
			_widget = null;
			for (MyGUI::VectorWidgetPtr::iterator iter=mListWindowRoot.begin(); iter!=mListWindowRoot.end(); ++iter) {
				MyGUI::WidgetPtr find = (*iter)->findWidget(mPrefix + _name);
				if (null != find) {
					_widget = new T(find);
					mListBase.push_back(_widget);
					return;
				}
			}
			MYGUI_ASSERT( ! _throw, "widget name '" << _name << "' in layout '" << mLayoutName << "' not found.");
		}

	public:
		virtual ~BaseLayout2();

	protected:
		MyGUI::WidgetPtr mMainWidget;

	private:
		std::string mPrefix;
		std::string mLayoutName;
		MyGUI::VectorWidgetPtr mListWindowRoot;
		typedef std::vector<BaseLayout2*> VectorBasePtr;
		VectorBasePtr mListBase;
	};

	class BaseLayout
	{
	public:
		BaseLayout();
		BaseLayout(const std::string & _layout);
		virtual ~BaseLayout() { }

		// ������������ ��� �������� ������� ����
		virtual void initialise();

		// ������������ ��� �������� �������� ����
		virtual void initialise(MyGUI::WidgetPtr _parent);
		void initialise(BaseLayout & _parent) { initialise(_parent.mainWidget()); }

		// ������������ ��� ������������ ������������ ����
		virtual void wrap(MyGUI::WidgetPtr _widget);
		void wrap(BaseLayout & _widget) { wrap(_widget.mainWidget()); }

		virtual void shutdown();

		MyGUI::WidgetPtr operator->() const
		{
			MYGUI_DEBUG_ASSERT( mMainWidget, "error assign main widget");
			return mMainWidget;
		}

		MyGUI::WidgetPtr mainWidget()
		{
			MYGUI_DEBUG_ASSERT( mMainWidget, "error assign main widget");
			return mMainWidget;
		}

	protected:
		void loadLayout(MyGUI::WidgetPtr _parent = null);

		template <typename T>
		void assignWidget(T * & _widget, const std::string & _name, bool _throw = true)
		{
			_widget = null;
			for (MyGUI::VectorWidgetPtr::iterator iter=mListWindowRoot.begin(); iter!=mListWindowRoot.end(); ++iter) {
				MyGUI::WidgetPtr find = (*iter)->findWidget(mPrefix + _name);
				if (null != find) {
					T * cast = find->castType<T>(false);
					if (null != cast) {
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

		void wrapWidget(BaseLayout & _wrap, const std::string & _name, bool _throw = true)
		{
			for (MyGUI::VectorWidgetPtr::iterator iter=mListWindowRoot.begin(); iter!=mListWindowRoot.end(); ++iter) {
				MyGUI::WidgetPtr find = (*iter)->findWidget(mPrefix + _name);
				_wrap.wrap(find);
				return;
			}
			MYGUI_ASSERT( ! _throw, "widget name '" << _name << "' in layout '" << mLayoutName << "' not found.");
		}

		void assignWidget(BaseLayout & _widget, const std::string & _name, bool _throw = true);

	protected:
		std::string mPrefix;
		std::string mLayoutName;
		MyGUI::VectorWidgetPtr mListWindowRoot;
		MyGUI::WidgetPtr mMainWidget;
		MyGUI::WidgetPtr mParentWidget;

	};

} // namespace wraps

#endif // __BASE_LAYOUT_H__
