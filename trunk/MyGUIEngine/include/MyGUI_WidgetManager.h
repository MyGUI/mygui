/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef __MYGUI_WIDGET_MANAGER_H__
#define __MYGUI_WIDGET_MANAGER_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Singleton.h"
#include "MyGUI_IUnlinkWidget.h"
#include "MyGUI_ICroppedRectangle.h"
#include "MyGUI_Widget.h"
#include <set>
#include "MyGUI_BackwardCompatibility.h"

namespace MyGUI
{
	class MYGUI_EXPORT WidgetManager :
		public Singleton<WidgetManager>,
		public MemberObsolete<WidgetManager>
	{
	public:
		WidgetManager();

		void initialise();
		void shutdown();

		Widget* createWidget(WidgetStyle _style, const std::string& _type, const std::string& _skin, const IntCoord& _coord, Widget* _parent, ICroppedRectangle* _cropeedParent, const std::string& _name);

		/** Destroy _widget */
		void destroyWidget(Widget* _widget);
		/** Destroy vector of widgets */
		void destroyWidgets(const VectorWidgetPtr& _widgets);
		/** Destroy Enumerator of widgets */
		void destroyWidgets(EnumeratorWidgetPtr _widgets);

		/** Register unlinker (call unlink if for any destroyed widget)*/
		void registerUnlinker(IUnlinkWidget* _unlink);
		/** Unregister unlinker (call unlink if for any destroyed widget)*/
		void unregisterUnlinker(IUnlinkWidget* _unlink);
		/** Unlink widget */
		void unlinkFromUnlinkers(Widget* _widget);

		/** Check if factory with specified widget type exist */
		bool isFactoryExist(const std::string& _type);

		/*internal:*/
		void _deleteWidget(Widget* _widget);
		void _deleteDelayWidgets();

		const std::string& getCategoryName() const;

	private:
		void notifyEventFrameStart(float _time);

	private:
		bool mIsInitialise;
		std::string mCategoryName;

		// список менеджеров для отписки при удалении
		VectorIUnlinkWidget mVectorIUnlinkWidget;

		// список виджетов для удаления
		VectorWidgetPtr mDestroyWidgets;
	};

} // namespace MyGUI

#endif // __MYGUI_WIDGET_MANAGER_H__
