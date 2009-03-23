/*!
	@file
	@author		Albert Semenov
	@date		10/2008
	@module
*/
#ifndef __VIEW_H__
#define __VIEW_H__

#include <MyGUI.h>
#include "BaseLayout/BaseLayout.h"

namespace editor
{

	class View : public wraps::BaseLayout
	{
	public:
		View();

	private:
		void initialiseImages();

		void updateView(MyGUI::ResourceImageSetPtr _image);
		void addGroup(MyGUI::ResourceImageSetPtr _image, const MyGUI::GroupImage & _group, MyGUI::IntSize & _size);
		void addAnimGroup(MyGUI::ResourceImageSetPtr _image, const MyGUI::GroupImage & _group, MyGUI::IntSize & _size);
		void addIndex(MyGUI::ResourceImageSetPtr _image, const MyGUI::GroupImage & _group, const MyGUI::IndexImage & _index, MyGUI::IntSize & _size);

		void notifyFrameStart(float _time);
		void notifyListChangePosition(MyGUI::MultiListPtr _sender, size_t _index);

	private:

		MyGUI::MultiListPtr mMultiList;
		MyGUI::ScrollViewPtr mImageView;

		MyGUI::EditPtr mEditResourceName;
		MyGUI::EditPtr mEditResourceID;
		MyGUI::EditPtr mEditFileName;

		struct PairAnim
		{
			PairAnim(MyGUI::StaticTextPtr _text, MyGUI::StaticImagePtr _image, const MyGUI::GroupImage & _group) :
				text(_text),
				image(_image),
				group(_group),
				index(0)
			{
			}

			MyGUI::StaticTextPtr text;
			MyGUI::StaticImagePtr image;
			MyGUI::GroupImage group;
			size_t index;
		};
		typedef std::vector<PairAnim> VectorPairAnim;
		VectorPairAnim mVectorAnimImages;

		float m_CurrentTime;

	};

} // namespace editor

#endif // __VIEW_H__
