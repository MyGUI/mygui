#pragma once

#include <MyGUI.h>
#include "WidgetContainer.h"
#include "Skinable.h"

namespace demo
{
	class Gui;

	class Widget :
		public WidgetContainer,
		public Skinable
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

		virtual void detachWidget(Widget* _child);
		virtual void attachWidget(Widget* _child);

		Widget* getParent() { return mParent; }
		Widget* getVisualParent() { return mVisualParent; }
		WidgetContainer* getParentContainer() { return mParentContainer; }

		void changeSkin(const std::string& _skin);

	protected:
		Widget();
		virtual ~Widget();

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
		WidgetContainer* mParentContainer;
		VectorWidgetPtr mChilds;

		Widget* mClient;
	};
}