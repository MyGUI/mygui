/*!
	@file
	@author		Georgiy Evmenov
	@date		08/2008
*/
#ifndef __EDITOR_TOOL_TIP_H__
#define __EDITOR_TOOL_TIP_H__

#include "BaseLayout/BaseLayout.h"
#include "SkinInfo.h"

namespace tools
{
	class EditorToolTip :
		public wraps::BaseLayout,
		public MyGUI::Singleton<EditorToolTip>
	{
	public:
		EditorToolTip();
		virtual ~EditorToolTip();

		void initialise();
		void shutdown();

		void show(const SkinInfo& _data);
		void hide();
		void move(const MyGUI::IntPoint& _point);

	private:
		void setPosition(const MyGUI::IntPoint& _point);

	private:
		MyGUI::EditBox* mText;
		MyGUI::Widget* mLastWidget;

		int mMinWidth;
		int mMinHeight;
	};

} // namespace tools

#endif // __EDITOR_TOOL_TIP_H__
