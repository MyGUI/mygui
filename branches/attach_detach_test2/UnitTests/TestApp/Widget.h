#pragma once

#include <MyGUI.h>

namespace demo
{
	class Widget;
	typedef std::vector<Widget*> VectorWidgetPtr;
	class Gui;

	class Widget
	{
		friend class Gui;

	public:
		void initialise();
		void shutdown();

		Widget* createChild();
		void destroyChild(Widget* _widget);
		void destroyAllChilds();

		size_t getChildCount();
		Widget* getChild(size_t _index);

		void detachWidget(Widget* _child);
		void attachWidget(Widget* _child);

		Widget* getParent() { return mParent; }
		Widget* getVisualParent() { return mVisualParent; }

		void changeSkin(const std::string& _skin);

	protected:
		Widget();
		virtual ~Widget();

		virtual void onVisualChildAdded(Widget* _child) { }
		virtual void onVisualChildRemoved(Widget* _child) { }

		virtual void onDestroySkin() { }
		virtual void onCreateSkin(const std::string& _skin) { }

	private:
		void addVisualChild(Widget* _widget);
		void removeVisualChild(Widget* _widget);

		void addChild(Widget* _widget);
		void removeChild(Widget* _widget);

		void addVisualChildToClient(Widget* _widget);
		void removeVisualChildFromClient(Widget* _widget);

		void destroySkin();
		void createSkin(const std::string& _skin);

	private:
		Widget* mVisualParent;
		VectorWidgetPtr mVisualChilds;
		
		Widget* mParent;
		VectorWidgetPtr mChilds;

		Widget* mClient;
	};
}