/*!
	@file
	@author		Albert Semenov
	@date		10/2008
*/
#ifndef __VIEW_H__
#define __VIEW_H__

#include <MyGUI.h>
#include "BaseLayout/BaseLayout.h"

namespace editor
{

	class View :
		public wraps::BaseLayout
	{
	public:
		View();

	private:
		void initialiseImages();

		void updateView(MyGUI::ResourceImageSetPtr _image);
		void addGroup(MyGUI::ResourceImageSetPtr _image, const MyGUI::GroupImage& _group, MyGUI::IntSize& _size);
		void addAnimGroup(MyGUI::ResourceImageSetPtr _image, const MyGUI::GroupImage& _group, MyGUI::IntSize& _size);
		void addIndex(MyGUI::ResourceImageSetPtr _image, const MyGUI::GroupImage& _group, const MyGUI::IndexImage& _index, MyGUI::IntSize& _size);

		void notifyFrameStart(float _time);
		void notifyListChangePosition(MyGUI::MultiListBox* _sender, size_t _index);

		void updateColumn();
		void notifyWindowChangeCoord(MyGUI::Window* _sender);

	private:

		MyGUI::MultiListBox* mMultiList;
		MyGUI::ScrollView* mImageView;

		MyGUI::EditBox* mEditResourceName;
		MyGUI::EditBox* mEditFileName;

		struct PairAnim
		{
			PairAnim(MyGUI::TextBox* _text, MyGUI::ImageBox* _image, const MyGUI::GroupImage& _group) :
				text(_text),
				image(_image),
				group(_group),
				index(0)
			{
			}

			MyGUI::TextBox* text;
			MyGUI::ImageBox* image;
			MyGUI::GroupImage group;
			size_t index;
		};

		typedef std::vector<PairAnim> VectorPairAnim;
		VectorPairAnim mVectorAnimImages;

		float m_CurrentTime;
	};

} // namespace editor

#endif // __VIEW_H__
