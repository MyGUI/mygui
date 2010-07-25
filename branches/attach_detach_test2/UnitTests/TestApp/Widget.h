#pragma once

#include <MyGUI.h>
#include "WidgetContainer.h"
#include "Skinable.h"

namespace MyGUI
{
	class Gui2;

	class Control :
		public ControlContainer,
		public Skinable
	{
		friend class Gui2;

	public:
		void initialise();
		void shutdown();

		Control* createChild();
		void destroyChild(Control* _widget);
		void destroyAllChilds();

		size_t getChildCount();
		Control* getChild(size_t _index);

		virtual void detachControl(Control* _child);
		virtual void attachControl(Control* _child);

		Control* getParent() { return mParent; }
		//Control* getVisualParent() { return mVisualParent; }
		ControlContainer* getParentContainer() { return mParentContainer; }

		void changeSkin(const std::string& _skinName);

		void setCoord(const IntCoord& _value);

	protected:
		Control();
		virtual ~Control();

	private:
		void addVisualChild(Control* _widget);
		void removeVisualChild(Control* _widget);

		void addChild(Control* _widget);
		void removeChild(Control* _widget);

		void addVisualChildToClient(Control* _widget);
		void removeVisualChildFromClient(Control* _widget);

		void destroySkin();
		void createSkin(ResourceSkin* _info);

	private:
		Control* mVisualParent;
		VectorControlPtr mVisualChilds;
		
		Control* mParent;
		ControlContainer* mParentContainer;
		VectorControlPtr mChilds;

		Control* mClient;

		IntCoord mCoord;
	};
}