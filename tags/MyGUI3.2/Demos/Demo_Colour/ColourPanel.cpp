/*!
	@file
	@author		Albert Semenov
	@date		08/2008
*/
#include "Precompiled.h"
#include "ColourPanel.h"

namespace demo
{

	ColourPanel::ColourPanel()
	{
		initialiseByAttributes(this);

		mCurrentColour = MyGUI::Colour::Green;
		mBaseColour = MyGUI::Colour::Green;

		mColourRect->eventMouseButtonPressed += MyGUI::newDelegate(this, &ColourPanel::notifyMouseButtonPressed);
		mColourRect->eventMouseDrag += MyGUI::newDelegate(this, &ColourPanel::notifyMouseDrag);
		mImageColourPicker->eventMouseDrag += MyGUI::newDelegate(this, &ColourPanel::notifyMouseDrag);
		mScrollRange->eventScrollChangePosition += MyGUI::newDelegate(this, &ColourPanel::notifyScrollChangePosition);

		mEditRed->eventEditTextChange += MyGUI::newDelegate(this, &ColourPanel::notifyEditTextChange);
		mEditGreen->eventEditTextChange += MyGUI::newDelegate(this, &ColourPanel::notifyEditTextChange);
		mEditBlue->eventEditTextChange += MyGUI::newDelegate(this, &ColourPanel::notifyEditTextChange);

		mOk->eventMouseButtonClick += MyGUI::newDelegate(this, &ColourPanel::notifyMouseButtonClick);

		mColourRange.push_back(MyGUI::Colour(1, 0, 0));
		mColourRange.push_back(MyGUI::Colour(1, 0, 1));
		mColourRange.push_back(MyGUI::Colour(0, 0, 1));
		mColourRange.push_back(MyGUI::Colour(0, 1, 1));
		mColourRange.push_back(MyGUI::Colour(0, 1, 0));
		mColourRange.push_back(MyGUI::Colour(1, 1, 0));
		mColourRange.push_back(mColourRange[0]);

		createTexture();

		updateFirst();
	}

	ColourPanel::~ColourPanel()
	{
		destroyTexture();
	}

	void ColourPanel::notifyMouseButtonClick(MyGUI::Widget* _sender)
	{
		eventColourAccept(this);
	}

	void ColourPanel::updateFirst()
	{
		notifyScrollChangePosition(nullptr, mScrollRange->getScrollPosition());

		notifyMouseDrag(nullptr,
			mImageColourPicker->getAbsoluteLeft() + (mColourRect->getWidth() / 2),
			mImageColourPicker->getAbsoluteTop() + (mColourRect->getHeight() / 2),
			MyGUI::MouseButton::Left);
	}

	void ColourPanel::createTexture()
	{
		MyGUI::uint size = 32;
		mTexture = MyGUI::RenderManager::getInstance().createTexture("ColourGradient");
		mTexture->createManual(size, size,
			MyGUI::TextureUsage::Static | MyGUI::TextureUsage::Write,
			MyGUI::PixelFormat::R8G8B8A8);

		mColourRect->setImageTexture("ColourGradient");
	}

	void ColourPanel::destroyTexture()
	{
		MyGUI::RenderManager::getInstance().destroyTexture( mTexture );
		mTexture = nullptr;
	}

	void ColourPanel::updateTexture(const MyGUI::Colour& _colour)
	{
		size_t size = 32;

		MyGUI::uint8* pDest = static_cast<MyGUI::uint8*>(mTexture->lock(MyGUI::TextureUsage::Write));

		for (size_t j = 0; j < size; j++)
			for (size_t i = 0; i < size; i++)
			{
				float x = (float)i / size;
				float y = (float)j / size;
				*pDest++ = MyGUI::uint8((1. - y) * (_colour.blue  * x + (1. - x)) * 255); // B
				*pDest++ = MyGUI::uint8((1. - y) * (_colour.green * x + (1. - x)) * 255); // G
				*pDest++ = MyGUI::uint8((1. - y) * (_colour.red   * x + (1. - x)) * 255); // R
				*pDest++ = 255; // A
			}

		// Unlock the pixel buffer
		mTexture->unlock();
	}

	void ColourPanel::notifyMouseDrag(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
	{
		if (_id != MyGUI::MouseButton::Left)
			return;

		MyGUI::Widget* parent = mImageColourPicker->getParent();
		MyGUI::IntPoint point(_left - parent->getAbsoluteLeft(), _top - parent->getAbsoluteTop());

		if (point.left < 0) point.left = 0;
		if (point.top < 0) point.top = 0;
		if (point.left > mColourRect->getWidth()) point.left = mColourRect->getWidth();
		if (point.top > mColourRect->getHeight()) point.top = mColourRect->getHeight();

		mImageColourPicker->setPosition(point.left - (mImageColourPicker->getWidth() / 2), point.top - (mImageColourPicker->getHeight() / 2));

		updateFromPoint(point);
	}

	void ColourPanel::notifyMouseButtonPressed(MyGUI::Widget* _sender, int _left, int _top, MyGUI::MouseButton _id)
	{
		if (_id == MyGUI::MouseButton::Left)
			notifyMouseDrag(nullptr, _left, _top, _id);
	}

	void ColourPanel::updateFromPoint(const MyGUI::IntPoint& _point)
	{
		// get colour by cursor position Altren 09.2008
		float x = 1.0f * _point.left / mColourRect->getWidth();
		float y = 1.0f * _point.top / mColourRect->getHeight();
		if (x > 1) x = 1;
		else if (x < 0) x = 0;
		if (y > 1) y = 1;
		else if (y < 0) y = 0;

		mCurrentColour.red = (1 - y) * (mBaseColour.red * x + MyGUI::Colour::White.red * (1 - x));
		mCurrentColour.green = (1 - y) * (mBaseColour.green * x + MyGUI::Colour::White.green * (1 - x));
		mCurrentColour.blue = (1 - y) * (mBaseColour.blue * x + MyGUI::Colour::White.blue * (1 - x));

		mColourView->setColour(mCurrentColour);

		mEditRed->setCaption(MyGUI::utility::toString((int)(mCurrentColour.red * 255)));
		mEditGreen->setCaption(MyGUI::utility::toString((int)(mCurrentColour.green * 255)));
		mEditBlue->setCaption(MyGUI::utility::toString((int)(mCurrentColour.blue * 255)));
	}

	void ColourPanel::notifyScrollChangePosition(MyGUI::ScrollBar* _sender, size_t _position)
	{
		float sector_size = (float)mScrollRange->getScrollRange() / 6.0f;
		float sector_current = (float)_position / sector_size;

		// current sector
		size_t current = (size_t)sector_current;
		assert(current < 6);
		// смещение до следующего сектора от 0 до 1
		float offfset = (sector_current - (float)current);

		const MyGUI::Colour& from = mColourRange[current];
		const MyGUI::Colour& to = mColourRange[current + 1];

		mBaseColour.red = from.red + offfset * (to.red - from.red);
		mBaseColour.green = from.green + offfset * (to.green - from.green);
		mBaseColour.blue = from.blue + offfset * (to.blue - from.blue);

		updateTexture(mBaseColour);

		MyGUI::IntPoint point(
			mImageColourPicker->getLeft() + (mImageColourPicker->getWidth() / 2),
			mImageColourPicker->getTop() + (mImageColourPicker->getHeight() / 2));

		updateFromPoint(point);
	}

	void ColourPanel::notifyEditTextChange(MyGUI::EditBox* _sender)
	{
		MyGUI::EditBox* edit = static_cast<MyGUI::EditBox*>(_sender);
		size_t cursor = edit->getTextCursor();
		size_t num = MyGUI::utility::parseSizeT(edit->getOnlyText());
		if (num > 255) num = 255;
		edit->setCaption(MyGUI::utility::toString(num));
		if (cursor < edit->getTextLength()) edit->setTextCursor(cursor);

		MyGUI::Colour colour(
			MyGUI::utility::parseFloat(mEditRed->getOnlyText()) / 255.0f,
			MyGUI::utility::parseFloat(mEditGreen->getOnlyText()) / 255.0f,
			MyGUI::utility::parseFloat(mEditBlue->getOnlyText()) / 255.0f);

		updateFromColour(colour);
	}

	void ColourPanel::setColour(const MyGUI::Colour& _colour)
	{
		MyGUI::Colour colour = getSaturate(_colour);
		mEditRed->setCaption(MyGUI::utility::toString((int)(colour.red * 255)));
		mEditGreen->setCaption(MyGUI::utility::toString((int)(colour.green * 255)));
		mEditBlue->setCaption(MyGUI::utility::toString((int)(colour.blue * 255)));

		updateFromColour(colour);
	}

	void ColourPanel::updateFromColour(const MyGUI::Colour& _colour)
	{
		mCurrentColour = _colour;

		std::vector<float> vec;
		vec.push_back(_colour.red);
		vec.push_back(_colour.green);
		vec.push_back(_colour.blue);
		std::sort(vec.begin(), vec.end());

		MyGUI::IntPoint point((int)((1 - vec[0] / vec[2]) * mColourRect->getWidth()), (int)((1 - vec[2]) * mColourRect->getHeight()));
		mImageColourPicker->setPosition(point.left - (mImageColourPicker->getWidth() / 2), point.top - (mImageColourPicker->getHeight() / 2));

		int iMax = (_colour.red == vec[2]) ? 0 : (_colour.green == vec[2]) ? 1 : 2;
		int iMin = (_colour.red == vec[0]) ? 0 : (_colour.green == vec[0]) ? 1 : 2;
		int iAvg = 3 - iMax - iMin;

		if (iMin == iMax) // if gray colour - set base red
		{
			iMax = 0;
			iMin = 1;
			iAvg = 2;
			byIndex(mBaseColour, iMin) = 0.;
			byIndex(mBaseColour, iAvg) = 0.;
			byIndex(mBaseColour, iMax) = 1.;
		}
		else
		{
			byIndex(mBaseColour, iMin) = 0.;
			byIndex(mBaseColour, iAvg) = (vec[1] - vec[0]) / (vec[2] - vec[0]);
			byIndex(mBaseColour, iMax) = 1.;
		}


		int i;
		for (i = 0; i < 6; ++i)
		{
			if ((fabs(byIndex(mColourRange[i], iMin) - byIndex(mBaseColour, iMin)) < 0.001) &&
				(fabs(byIndex(mColourRange[i], iMax) - byIndex(mBaseColour, iMax)) < 0.001) &&
				(fabs(byIndex(mColourRange[i+1], iMin) - byIndex(mBaseColour, iMin)) < 0.001) &&
				(fabs(byIndex(mColourRange[i+1], iMax) - byIndex(mBaseColour, iMax)) < 0.001))
				break;
		}

		float sector_size = (float)mScrollRange->getScrollRange() / 6.0f;
		size_t current = i;

		float offset = byIndex(mBaseColour, iAvg);
		if (byIndex(mColourRange[i+1], iAvg) < byIndex(mColourRange[i], iAvg)) offset = 1 - byIndex(mBaseColour, iAvg);

		size_t pos = size_t((current + offset) * sector_size);

		mScrollRange->setScrollPosition(pos);

		// бонус для обрезки цвета под шкалу
		mBaseColour.red = mColourRange[i].red + offset * (mColourRange[i+1].red - mColourRange[i].red);
		mBaseColour.green = mColourRange[i].green + offset * (mColourRange[i+1].green - mColourRange[i].green);
		mBaseColour.blue = mColourRange[i].blue + offset * (mColourRange[i+1].blue - mColourRange[i].blue);

		updateTexture(mBaseColour);

		mColourView->setColour(mCurrentColour);
	}

	MyGUI::Colour ColourPanel::getSaturate(const MyGUI::Colour& _colour) const
	{
		MyGUI::Colour colour = _colour;
		if (colour.red < 0)
			colour.red = 0;
		else if (colour.red > 1)
			colour.red = 1;

		if (colour.green < 0)
			colour.green = 0;
		else if (colour.green > 1)
			colour.green = 1;

		if (colour.blue < 0)
			colour.blue = 0;
		else if (colour.blue > 1)
			colour.blue = 1;

		return colour;
	}

	float& ColourPanel::byIndex(MyGUI::Colour& _colour, size_t _index)
	{
		if (_index == 0) return _colour.red;
		else if (_index == 1) return _colour.green;
		else if (_index == 2) return _colour.blue;
		else return _colour.alpha;
	}

	const MyGUI::Colour& ColourPanel::getColour() const
	{
		return mCurrentColour;
	}

} // namespace demo
