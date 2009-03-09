/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#include "precompiled.h"
#include "ColourPanel.h"

namespace demo
{

	ColourPanel::ColourPanel() : BaseLayout("ColourPanel.layout")
	{
		mCurrentColour = Ogre::ColourValue::Green;
		mBaseColour = Ogre::ColourValue::Green;

		createTexture();

		assignWidget(mColourRect, "widget_ColourRect");
		assignWidget(mColourView, "widget_ColourView");
		assignWidget(mImageColourPicker, "image_Picker");
		assignWidget(mEditRed, "edit_Red");
		assignWidget(mEditGreen, "edit_Green");
		assignWidget(mEditBlue, "edit_Blue");
		assignWidget(mScrollRange, "scroll_Range");
		assignWidget(mOk, "button_OK");

		mColourRect->eventMouseButtonPressed = MyGUI::newDelegate(this, &ColourPanel::notifyMouseButtonPressed);
		mColourRect->eventMouseDrag = MyGUI::newDelegate(this, &ColourPanel::notifyMouseDrag);
		mImageColourPicker->eventMouseDrag = MyGUI::newDelegate(this, &ColourPanel::notifyMouseDrag);
		mScrollRange->eventScrollChangePosition = MyGUI::newDelegate(this, &ColourPanel::notifyScrollChangePosition);

		mEditRed->eventEditTextChange = MyGUI::newDelegate(this, &ColourPanel::notifyEditTextChange);
		mEditGreen->eventEditTextChange = MyGUI::newDelegate(this, &ColourPanel::notifyEditTextChange);
		mEditBlue->eventEditTextChange = MyGUI::newDelegate(this, &ColourPanel::notifyEditTextChange);

		mOk->eventMouseButtonClick = MyGUI::newDelegate(this, &ColourPanel::notifyMouseButtonClick);

		MyGUI::ISubWidget * main = mColourView->getSubWidgetMain();
		mRawColourView = main->castType<MyGUI::RawRect>();

		mColourRange.push_back(Ogre::ColourValue(1, 0, 0));
		mColourRange.push_back(Ogre::ColourValue(1, 0, 1));
		mColourRange.push_back(Ogre::ColourValue(0, 0, 1));
		mColourRange.push_back(Ogre::ColourValue(0, 1, 1));
		mColourRange.push_back(Ogre::ColourValue(0, 1, 0));
		mColourRange.push_back(Ogre::ColourValue(1, 1, 0));
		mColourRange.push_back(mColourRange[0]);

		updateFirst();

		std::string _texture = "resourceThatNotExist";
		Ogre::TextureManager & manager = Ogre::TextureManager::getSingleton();
		try {
			manager.load(_texture, Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME);
		}
		catch(Ogre::Exception & ) {
		}

		Ogre::TexturePtr tex = (Ogre::TexturePtr)manager.getByName(_texture); // not nullptr (!)
		bool exist = manager.resourceExists(_texture);
	}

	ColourPanel::~ColourPanel()
	{
		destroyTexture();
	}

	void ColourPanel::notifyMouseButtonClick(MyGUI::WidgetPtr _sender)
	{
		eventColourAccept(this);
	}

	void ColourPanel::updateFirst()
	{
		notifyScrollChangePosition(nullptr, mScrollRange->getScrollPosition());

		notifyMouseDrag(nullptr, mImageColourPicker->getAbsoluteLeft() + 10, mImageColourPicker->getAbsoluteTop() + 10);
	}

	void ColourPanel::createTexture()
	{
		Ogre::uint size = 32;

		//std::string texture(utility::toString(this, "_TextureRenderBox"));

		// Create the texture
		texture = Ogre::TextureManager::getSingleton().createManual(
			"ColourGradient", // name
			Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
			Ogre::TEX_TYPE_2D,      // type
			size, size,         // width & height
			0,                // number of mipmaps
			Ogre::PF_BYTE_BGRA,     // pixel format
			Ogre::TU_DYNAMIC_WRITE_ONLY_DISCARDABLE);
	}

	void ColourPanel::destroyTexture()
	{
		texture.setNull();
	}

	void ColourPanel::updateTexture(const Ogre::ColourValue _colour)
	{
		size_t size = 32;

		// Get the pixel buffer
		Ogre::HardwarePixelBufferSharedPtr pixelBuffer = texture->getBuffer();
		// Lock the pixel buffer and get a pixel box
		pixelBuffer->lock(Ogre::HardwareBuffer::HBL_DISCARD);
		const Ogre::PixelBox& pixelBox = pixelBuffer->getCurrentLock();

		Ogre::uint8* pDest = static_cast<Ogre::uint8*>(pixelBox.data);

		for (size_t j = 0; j < size; j++)
			for(size_t i = 0; i < size; i++)
			{
				float x = (float)i/size;
				float y = (float)j/size;
				*pDest++ = (1. - y) * (_colour.b * x + (1. - x)) * 255; // B
				*pDest++ = (1. - y) * (_colour.g * x + (1. - x)) * 255; // G
				*pDest++ = (1. - y) * (_colour.r * x + (1. - x)) * 255; // R
				*pDest++ = 255; // A
			}

		// Unlock the pixel buffer
		pixelBuffer->unlock();

		//mColourRect->setImageTexture("ColourGradient");
	}

	void ColourPanel::notifyMouseDrag(MyGUI::WidgetPtr _sender, int _left, int _top)
	{
		MyGUI::WidgetPtr parent = mImageColourPicker->getParent();
		MyGUI::IntPoint point(_left - parent->getAbsoluteLeft(), _top - parent->getAbsoluteTop());

		if (point.left < 0) point.left = 0;
		if (point.top < 0) point.top = 0;
		if (point.left > mColourRect->getWidth()) point.left = mColourRect->getWidth();
		if (point.top > mColourRect->getHeight()) point.top = mColourRect->getHeight();

		mImageColourPicker->setPosition(point.left - (mImageColourPicker->getWidth() / 2), point.top - (mImageColourPicker->getHeight() / 2));

		updateFromPoint(point);
	}

	void ColourPanel::notifyMouseButtonPressed(MyGUI::WidgetPtr _sender, int _left, int _top, MyGUI::MouseButton _id)
	{
		if (_id == MyGUI::MouseButton::Left) notifyMouseDrag(nullptr, _left, _top);
	}

	void ColourPanel::updateFromPoint(const MyGUI::IntPoint & _point)
	{
		// вычисляем цвет по положению курсора Altren 09.2008
		float x = 1. * _point.left / mColourRect->getWidth();
		float y = 1. * _point.top / mColourRect->getHeight();
		if (x > 1) x = 1;
		else if (x < 0) x = 0;
		if (y > 1) y = 1;
		else if (y < 0) y = 0;

		mCurrentColour = (1 - y) * (mBaseColour * x + Ogre::ColourValue::White * (1 - x));
		mRawColourView->setRectColour(mCurrentColour, mCurrentColour, mCurrentColour, mCurrentColour);

		mEditRed->setCaption(MyGUI::utility::toString((int)(mCurrentColour.r * 255)));
		mEditGreen->setCaption(MyGUI::utility::toString((int)(mCurrentColour.g * 255)));
		mEditBlue->setCaption(MyGUI::utility::toString((int)(mCurrentColour.b * 255)));

	}

	void ColourPanel::notifyScrollChangePosition(MyGUI::VScrollPtr _sender, size_t _position)
	{
		float sector_size = (float)mScrollRange->getScrollRange() / 6.0f;
		float sector_current = (float)_position / sector_size;

		// текущий сектор
		size_t current = (size_t)sector_current;
		assert(current < 6);
		// смещение до следующего сектора от 0 до 1
		float offfset = (sector_current - (float)current);

		const Ogre::ColourValue & from = mColourRange[current];
		const Ogre::ColourValue & to = mColourRange[current + 1];

		mBaseColour.r = from.r + offfset * (to.r - from.r);
		mBaseColour.g = from.g + offfset * (to.g - from.g);
		mBaseColour.b = from.b + offfset * (to.b - from.b);

		updateTexture(mBaseColour);

		MyGUI::IntPoint point(
			mImageColourPicker->getLeft() + (mImageColourPicker->getWidth() / 2),
			mImageColourPicker->getTop() + (mImageColourPicker->getHeight() / 2));

		updateFromPoint(point);
	}

	void ColourPanel::notifyEditTextChange(MyGUI::EditPtr _sender)
	{
		MyGUI::EditPtr edit = static_cast<MyGUI::EditPtr>(_sender);
		size_t cursor = edit->getTextCursor();
		size_t num = MyGUI::utility::parseSizeT(edit->getCaption());
		if (num > 255) num = 255;
		edit->setCaption(MyGUI::utility::toString(num));
		if (cursor < edit->getTextLength()) edit->setTextCursor(cursor);

		Ogre::ColourValue colour(
			MyGUI::utility::parseFloat(mEditRed->getCaption()) / 255.0f,
			MyGUI::utility::parseFloat(mEditGreen->getCaption()) / 255.0f,
			MyGUI::utility::parseFloat(mEditBlue->getCaption()) / 255.0f);
		updateFromColour(colour);
	}

	void ColourPanel::setColour(const Ogre::ColourValue & _colour)
	{
		Ogre::ColourValue colour(_colour.saturateCopy());
		mEditRed->setCaption(MyGUI::utility::toString((int)(colour.r * 255)));
		mEditGreen->setCaption(MyGUI::utility::toString((int)(colour.g * 255)));
		mEditBlue->setCaption(MyGUI::utility::toString((int)(colour.b * 255)));

		updateFromColour(colour);
	}

	void ColourPanel::updateFromColour(const Ogre::ColourValue & _colour)
	{
		mCurrentColour = _colour;

		std::vector<float> vec;
		vec.push_back(_colour.r);
		vec.push_back(_colour.g);
		vec.push_back(_colour.b);
		std::sort(vec.begin(), vec.end());

		MyGUI::IntPoint point((1 - vec[0]/vec[2]) * mColourRect->getWidth(), (1 - vec[2]) * mColourRect->getHeight());
		mImageColourPicker->setPosition(point.left - (mImageColourPicker->getWidth() / 2), point.top - (mImageColourPicker->getHeight() / 2));

		int iMax = (_colour.r == vec[2]) ? 0 : (_colour.g == vec[2]) ? 1 : 2;
		int iMin = (_colour.r == vec[0]) ? 0 : (_colour.g == vec[0]) ? 1 : 2;
		int iAvg = 3 - iMax - iMin;

		if (iMin == iMax) // if gray colour - set base red
		{
			iMax = 0;
			iMin = 1;
			iAvg = 2;
			mBaseColour[iMin] = 0.;
			mBaseColour[iAvg] = 0.;
			mBaseColour[iMax] = 1.;
		}
		else
		{
			mBaseColour[iMin] = 0.;
			mBaseColour[iAvg] = (vec[1] - vec[0]) / (vec[2] - vec[0]);
			mBaseColour[iMax] = 1.;
		}


		int i;
		for (i = 0; i<6; ++i)
		{
			if ((fabs(mColourRange[i][iMin] - mBaseColour[iMin]) < 0.001) &&
				(fabs(mColourRange[i][iMax] - mBaseColour[iMax]) < 0.001) &&
				(fabs(mColourRange[i+1][iMin] - mBaseColour[iMin]) < 0.001) &&
				(fabs(mColourRange[i+1][iMax] - mBaseColour[iMax]) < 0.001))
				break;
		}

		/*
		float offfset = (sector_current - (float)current);
		mBaseColour.r = from.r + offfset * (to.r - from.r);
		mBaseColour.g = from.g + offfset * (to.g - from.g);
		mBaseColour.b = from.b + offfset * (to.b - from.b);
	*/


		float sector_size = (float)mScrollRange->getScrollRange() / 6.0f;
		size_t current = i;

		float offset = mBaseColour[iAvg];
		if (mColourRange[i+1][iAvg] < mColourRange[i][iAvg]) offset = 1 - mBaseColour[iAvg];

		size_t pos = (current + offset) * sector_size;

		mScrollRange->setScrollPosition(pos);

		// бонус для обрезки цвета под шкалу
		mBaseColour.r = mColourRange[i].r + offset * (mColourRange[i+1].r - mColourRange[i].r);
		mBaseColour.g = mColourRange[i].g + offset * (mColourRange[i+1].g - mColourRange[i].g);
		mBaseColour.b = mColourRange[i].b + offset * (mColourRange[i+1].b - mColourRange[i].b);

		updateTexture(mBaseColour);
	}

 } // namespace demo
