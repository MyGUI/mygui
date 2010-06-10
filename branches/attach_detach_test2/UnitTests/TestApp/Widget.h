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

		Widget* createWidget(const std::string& _skin);
		void destroyChild(Widget* _widget);
		void destroyAllChilds();

		void changeSkin(const std::string& _skin);

		Widget* getParent() { return mParent; }
		size_t getChildCount();
		Widget* getChild(size_t _index);

		Widget* getVisualParent() { return mVisualParent; }

	protected:
		Widget();
		virtual ~Widget();

		virtual void onVisualChildAdded(Widget* _child) { }
		virtual void onVisualChildRemoved(Widget* _child) { }

		virtual void onChildAdded(Widget* _child) { }
		virtual void onChildRemoved(Widget* _child) { }

		virtual void onDestroySkin() { }
		virtual void onCreateSkin(const std::string& _skin) { }

	private:
		void addVisualChild(Widget* _child);
		void removeVisualChild(Widget* _child);
		void addChild(Widget* _child);
		void removeChild(Widget* _child);

		void addVisualChildToClient(Widget* _child);
		void removeVisualChildFromClient(Widget* _child);
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