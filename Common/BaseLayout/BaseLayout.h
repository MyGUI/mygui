/*!
	@file
	@author		Albert Semenov
	@date		07/2008
	@module
*/

#ifndef __BASE_LAYOUT_H__
#define __BASE_LAYOUT_H__

#include <MyGUI.h>
#include "WrapsAttribute.h"

namespace wraps
{

	class BaseLayout
	{
	protected:
		BaseLayout() : mMainWidget(nullptr)
		{
		}

		BaseLayout(const std::string& _layout, MyGUI::Widget* _parent = nullptr) : mMainWidget(nullptr)
		{
			initialise(_layout, _parent);
		}

		template <typename T>
		void assignWidget(T * & _widget, const std::string& _name, bool _throw = true)
		{
			_widget = nullptr;
			for (MyGUI::VectorWidgetPtr::iterator iter = mListWindowRoot.begin(); iter != mListWindowRoot.end(); ++iter)
			{
				MyGUI::Widget* find = (*iter)->findWidget(mPrefix + _name);
				if (nullptr != find)
				{
					T* cast = find->castType<T>(false);
					if (nullptr != cast)
					{
						_widget = cast;
					}
					else if (_throw)
					{
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
		void assignBase(T * & _widget, const std::string& _name, bool _throw = true)
		{
			_widget = nullptr;
			for (MyGUI::VectorWidgetPtr::iterator iter = mListWindowRoot.begin(); iter != mListWindowRoot.end(); ++iter)
			{
				MyGUI::Widget* find = (*iter)->findWidget(mPrefix + _name);
				if (nullptr != find)
				{
					_widget = new T(find);
					mListBase.push_back(_widget);
					return;
				}
			}
			MYGUI_ASSERT( ! _throw, "widget name '" << _name << "' in layout '" << mLayoutName << "' not found.");
		}

		void initialise(const std::string& _layout, MyGUI::Widget* _parent = nullptr)
		{
			const std::string MAIN_WINDOW1 = "_Main";
			const std::string MAIN_WINDOW2 = "Root";
			mLayoutName = _layout;

			// оборачиваем
			if (mLayoutName.empty())
			{
				mMainWidget = _parent;
			}
			// загружаем лейаут на виджет
			else
			{
				mPrefix = MyGUI::utility::toString(this, "_");
				mListWindowRoot = MyGUI::LayoutManager::getInstance().loadLayout(mLayoutName, mPrefix, _parent);

				const std::string mainName1 = mPrefix + MAIN_WINDOW1;
				const std::string mainName2 = mPrefix + MAIN_WINDOW2;
				for (MyGUI::VectorWidgetPtr::iterator iter = mListWindowRoot.begin(); iter != mListWindowRoot.end(); ++iter)
				{
					if ((*iter)->getName() == mainName1 || (*iter)->getName() == mainName2)
					{
						mMainWidget = (*iter);

						snapToParent(mMainWidget);

						break;
					}
				}
				MYGUI_ASSERT(mMainWidget, "root widget name '" << MAIN_WINDOW1 << "' or '" << MAIN_WINDOW2 << "' in layout '" << mLayoutName << "' not found.");
			}
		}

		void shutdown()
		{
			// удаляем все классы
			for (VectorBasePtr::reverse_iterator iter = mListBase.rbegin(); iter != mListBase.rend(); ++iter)
				delete (*iter);
			mListBase.clear();

			// удаляем все рутовые виджеты
			MyGUI::LayoutManager::getInstance().unloadLayout(mListWindowRoot);
			mListWindowRoot.clear();
		}

		template <typename Type>
		void initialiseByAttributes(Type* _owner, MyGUI::Widget* _parent = nullptr)
		{
			initialise(attribute::AttributeLayout<Type>::getData(), _parent);

			typename attribute::AttributeFieldWidgetName<Type>::VectorBindPair& data = attribute::AttributeFieldWidgetName<Type>::getData();
			for (typename attribute::AttributeFieldWidgetName<Type>::VectorBindPair::iterator item = data.begin(); item != data.end(); ++item)
			{
				MyGUI::Widget* value = 0;
				assignWidget(value, item->second, false);

				item->first->set(_owner, value);
			}
		}
	private:
		void snapToParent(MyGUI::Widget* _child)
		{
			if (_child->isUserString("SnapTo"))
			{
				MyGUI::Align align = MyGUI::Align::parse(_child->getUserString("SnapTo"));

				MyGUI::IntCoord coord = _child->getCoord();
				MyGUI::IntSize size = _child->getParentSize();

				if (align.isHStretch())
				{
					coord.left = 0;
					coord.width = size.width;
				}
				else if (align.isLeft())
				{
					coord.left = 0;
				}
				else if (align.isRight())
				{
					coord.left = size.width - coord.width;
				}
				else
				{
					coord.left = (size.width - coord.width) / 2;
				}

				if (align.isVStretch())
				{
					coord.top = 0;
					coord.height = size.height;
				}
				else if (align.isTop())
				{
					coord.top = 0;
				}
				else if (align.isBottom())
				{
					coord.top = size.height - coord.height;
				}
				else
				{
					coord.top = (size.height - coord.height) / 2;
				}

				_child->setCoord(coord);
			}
		}

	public:
		virtual ~BaseLayout()
		{
			shutdown();
		}

	protected:
		MyGUI::Widget* mMainWidget;

	private:
		std::string mPrefix;
		std::string mLayoutName;
		MyGUI::VectorWidgetPtr mListWindowRoot;
		typedef std::vector<BaseLayout*> VectorBasePtr;
		VectorBasePtr mListBase;
	};

} // namespace wraps

#endif // __BASE_LAYOUT_H__
