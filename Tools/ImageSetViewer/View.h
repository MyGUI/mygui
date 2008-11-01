/*!
	@file
	@author		Albert Semenov
	@date		10/2008
	@module
*/
#ifndef __VIEW_H__
#define __VIEW_H__

#include <MyGUI.h>
#include "BaseLayout.h"

namespace editor
{

	class View : public wraps::BaseLayout
	{
	public:
		View();

		virtual void initialise();

	private:
		void notifyComboAccept(MyGUI::WidgetPtr _sender, size_t _index);

		void initialiseImages();
		void selectResource(size_t _index);

		void updateView(MyGUI::ResourceImageSetPtr _image);
		void addGroup(MyGUI::ResourceImageSetPtr _image, MyGUI::GroupImage & _group, MyGUI::IntSize & _size);
		void addIndex(MyGUI::ResourceImageSetPtr _image, MyGUI::GroupImage & _group, MyGUI::IndexImage & _index, MyGUI::IntSize & _size);

	private:

		MyGUI::ComboBoxPtr mComboResource;

		MyGUI::StaticTextPtr mTextGuid;

		MyGUI::ScrollViewPtr mImageView;

	};

} // namespace editor

#endif // __VIEW_H__
