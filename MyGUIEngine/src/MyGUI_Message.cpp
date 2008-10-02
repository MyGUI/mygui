/*!
	@file
	@author		Albert Semenov
	@date		01/2008
	@module
*/
#include "MyGUI_Message.h"
#include "MyGUI_WidgetSkinInfo.h"
#include "MyGUI_WidgetManager.h"
#include "MyGUI_LayerManager.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_ResourceManager.h"
#include "MyGUI_WidgetOIS.h"
#include "MyGUI_MessageFactory.h"
#include "MyGUI_Gui.h"
#include "MyGUI_ControllerManager.h"
#include "MyGUI_ControllerFadeAlpha.h"
#include "MyGUI_StaticImage.h"

namespace MyGUI
{

	MYGUI_RTTI_CHILD_IMPLEMENT( Message, Window );

	const float MESSAGE_ALPHA_MAX = 0.5f;
	const float MESSAGE_ALPHA_MIN = 0.0f;
	const float MESSAGE_SPEED_COEF = 3.0f;

	Message::Message(const IntCoord& _coord, Align _align, const WidgetSkinInfoPtr _info, ICroppedRectangle * _parent, IWidgetCreator * _creator, const Ogre::String & _name) :
		Window(_coord, _align, _info, _parent, _creator, _name),
		mWidgetText(null),
		mInfoOk(Ok), mInfoCancel(Ok),
		mButton1Index(0),
		mSmoothShow(false),
		mWidgetFade(null),
		mIcon(null),
		mLeftOffset1(0),
		mLeftOffset2(0)
		//mResourceIcons(null)
	{
		// ищем индекс первой кнопки
		size_t but1 = (size_t)Button1;
		but1 >>= 1;
		while (0 != but1) {
			but1 >>= 1;
			mButton1Index++;
		}

		// парсим виджет для текста
		for (VectorWidgetPtr::iterator iter=mWidgetChild.begin(); iter!=mWidgetChild.end(); ++iter) {
			if ((*iter)->_getInternalString() == "Text") {
				MYGUI_DEBUG_ASSERT( ! mWidgetText, "widget already assigned");
				mWidgetText = (*iter);
				mOffsetText.set(mCoord.width - mWidgetText->getWidth(), mCoord.height - mWidgetText->getHeight());
				mLeftOffset2 = mLeftOffset1 = mWidgetText->getLeft();
			}
			else if ((*iter)->_getInternalString() == "Icon") {
				MYGUI_DEBUG_ASSERT( ! mIcon, "widget already assigned");
				mIcon = (*iter)->castType<StaticImage>();
			}
		}
		MYGUI_ASSERT(null != mWidgetText, "Child Text not found in skin (MessageBox must have widget for text)");

		if (mIcon != null) {
			mLeftOffset2 = mIcon->getRight() + 3;
		}

		// парсим свойства
		const MapString & properties = _info->getProperties();
		if (!properties.empty()) {
			MapString::const_iterator iter = properties.find("ButtonSkin");
			if (iter != properties.end()) mButtonSkin = iter->second;
			iter = properties.find("ButtonType");
			if (iter != properties.end()) mButtonType = iter->second;
			iter = properties.find("ButtonSize");
			if (iter != properties.end()) mButtonSize = IntSize::parse(iter->second);
			iter = properties.find("ButtonOffset");
			if (iter != properties.end()) mButtonOffset = IntSize::parse(iter->second);
			iter = properties.find("DefaultLayer");
			if (iter != properties.end()) mDefaultLayer = iter->second;
			iter = properties.find("FadeSkin");
			if (iter != properties.end()) mFadeSkin = iter->second;
			iter = properties.find("FadeLayer");
			if (iter != properties.end()) mFadeLayer = iter->second;
			//iter = properties.find("ResourceIcons");
			//if (iter != properties.end()) {
				//ResourcePtr icons = ResourceManager::getInstance().getResource(Guid::parse(iter->second));
				//mResourceIcons = icons->castType<ResourceImageSet>();
			//}
		}

	}

	void Message::setMessage(const Ogre::UTFString & _message)
	{
		mWidgetText->setCaption(_message);
		updateSize();
	}

	Message::ViewInfo Message::addButtonName(const Ogre::UTFString & _name)
	{
		if (mVectorButton.size() >= 7) {
			MYGUI_LOG(Warning, "Too many buttons in message box, ignored");
			return None;
		}
		// бит, номер кнопки + смещение до Button1
		ViewInfo info = (ViewInfo)(MYGUI_FLAG(mVectorButton.size() + mButton1Index));

		// запоминаем кнопки для отмены и подтверждения
		if (mVectorButton.empty()) mInfoOk = info;
		mInfoCancel = info;

		WidgetPtr button = createWidgetT(mButtonType, mButtonSkin, IntCoord(), Align::Left | Align::Bottom);
		button->eventMouseButtonClick = newDelegate(this, &Message::notifyButtonClick);
		button->setCaption(_name);
		button->_setInternalData((int)info);
		mVectorButton.push_back(button);

		updateSize();
		return info;
	}

	void Message::setButton(ViewInfo _info)
	{
		clearButton();
		size_t current = 0;
		size_t info = (size_t)_info;

		while ((0 != info) && (current < mButton1Index)) {
			if (0 != (info & 1)) {
				// если бит есть то ставим кнопку
				addButtonName(factory::MessageFactory::_getButtonName(current));
				// корректируем ее номер
				mVectorButton.back()->_setInternalData((int)MYGUI_FLAG(current));
			}
			info >>= 1;
			current ++;
		}

		updateSize();
	}

	void Message::notifyButtonClick(MyGUI::WidgetPtr _sender)
	{
		_destroyMessage((ViewInfo)_sender->_getInternalData());
	}

	void Message::clearButton()
	{
		for (VectorWidgetPtr::iterator iter=mVectorButton.begin(); iter!=mVectorButton.end(); ++iter) {
			WidgetManager::getInstance().destroyWidget(*iter);
		}
		mVectorButton.clear();
	}

	void Message::_onKeyButtonPressed(KeyCode _key, Char _char)
	{
		Window::_onKeyButtonPressed(_key, _char);
		if ((_key == KC_RETURN) || (_key == KC_NUMPADENTER)) _destroyMessage(mInfoOk);
		else if (_key == KC_ESCAPE) _destroyMessage(mInfoCancel);
	}

	void Message::_destroyMessage(ViewInfo _result)
	{
		eventMessageBoxEnd(this, _result);
		if (null != mWidgetFade) {
			if (mSmoothShow) {
				ControllerFadeAlpha * controller = new ControllerFadeAlpha(MESSAGE_ALPHA_MIN, MESSAGE_SPEED_COEF, false);
				controller->eventPostAction = newDelegate(action::actionWidgetDestroy);
				ControllerManager::getInstance().addItem(mWidgetFade, controller);
			}
			else WidgetManager::getInstance().destroyWidget(mWidgetFade);
		}
		if (mSmoothShow) destroySmooth();
		else WidgetManager::getInstance().destroyWidget(this);
	}

	void Message::setSmoothShow(bool _smooth)
	{
		mSmoothShow = _smooth;
		if (mSmoothShow) showSmooth(true);
	}

	void Message::setWindowFade(bool _fade)
	{
		return; //пока пропустим

		if (_fade) {
			if (null == mWidgetFade) {
				Gui & gui = Gui::getInstance();
				mWidgetFade = gui.createWidgetT(Widget::getClassTypeName(), mFadeSkin, IntCoord(0, 0, (int)gui.getViewWidth(), (int)gui.getViewHeight()), Align::Stretch, mFadeLayer);
				if (mSmoothShow) {
					mWidgetFade->hide();
					ControllerFadeAlpha * controller = new ControllerFadeAlpha(MESSAGE_ALPHA_MAX, MESSAGE_SPEED_COEF, false);
					ControllerManager::getInstance().addItem(mWidgetFade, controller);
				}
				else mWidgetFade->setAlpha(MESSAGE_ALPHA_MAX);
			}
		}
		else {
			if (null != mWidgetFade) {
				WidgetManager::getInstance().destroyWidget(mWidgetFade);
				mWidgetFade = null;
			}
		}
	}

	void Message::setMessageImage(size_t _image)
	{
		if (null == mIcon) return;
		if (mIcon->getItemResource() != null) {
			mIcon->setItemName(getIconName(_image));
		}
		else {
			mIcon->setImageIndex(_image);
		}
	}

	const char * Message::getIconName(size_t _index)
	{
		static const int CountIcons = 4;
		static const char * IconNames[CountIcons + 1] = {"Info", "Quest", "Error", "Warning", ""};
		if (_index >= CountIcons) return IconNames[CountIcons];
		return IconNames[_index];
	}

	MyGUI::MessagePtr Message::_createMessage(const Ogre::UTFString & _caption, const Ogre::UTFString & _message, const std::string & _skin, const std::string & _layer, bool _modal, EventMessageEnd * _delegate, ViewInfo _info, const std::string & _button1, const std::string & _button2, const std::string & _button3, const std::string & _button4)
	{
		Gui * gui = Gui::getInstancePtr();
		if (null == gui) return null;

		// ищем индекс первой иконки
		size_t image = (size_t)_info;
		size_t tmp = (size_t)Icon1;
		while (true) {
			tmp >>= 1;
			if (tmp == 0) break;
			image >>= 1;
		}
		tmp = 0;
		while (0 != image) {
			image >>= 1;
			tmp++;
		}
		image = tmp == 0 ? ITEM_NONE : tmp - 1;

		MessagePtr mess = gui->createWidget<Message>(_skin.empty() ? factory::MessageFactory::_getDefaultSkin() : _skin, IntCoord(), Align::Default, _layer);
		mess->setMessageImage(image);
		mess->setSmoothShow(true);
		mess->setCaption(_caption);
		mess->setMessage(_message);
		if (_modal) mess->setWindowFade(true);
		if (null != _delegate) mess->eventMessageBoxEnd = _delegate;
		if (None != _info) mess->setButton(_info);

		if (false == _button1.empty()) {
			mess->addButtonName(_button1);
			if (false == _button2.empty()) {
				mess->addButtonName(_button2);
				if (false == _button3.empty()) {
					mess->addButtonName(_button3);
				}
			}
		}

		if (_layer.empty()) LayerManager::getInstance().attachToLayerKeeper(mess->getDefaultLayer(), mess);
		if (_modal) InputManager::getInstance().addWidgetModal(mess);

		return mess;
	}

	void Message::updateSize()
	{
		IntSize size = mWidgetText->getTextSize();
		// минимум высота иконки
		if ((null != mIcon) && (mIcon->getImageIndex() != ITEM_NONE)) {
			if (size.height < mIcon->getHeight()) size.height = mIcon->getHeight();
			size.width += mIcon->getSize().width;
		}
		size += mOffsetText;
		size.width += 3;

		int width = ((int)mVectorButton.size() * mButtonSize.width) + (((int)mVectorButton.size()+1) * mButtonOffset.width);
		if (size.width < width) size.width = width;

		int offset = (size.width - width)/2;
		offset += mButtonOffset.width;

		IntSize view((int)Gui::getInstance().getViewWidth(), (int)Gui::getInstance().getViewHeight());
		setPosition((view.width-size.width)/2, (view.height-size.height)/2, size.width, size.height);

		if (null != mIcon) {
			if (mIcon->getImageIndex() != ITEM_NONE) mWidgetText->setPosition(mLeftOffset2, mWidgetText->getTop(), mWidgetText->getWidth(), mWidgetText->getHeight());
			else mWidgetText->setPosition(mLeftOffset1, mWidgetText->getTop(), mWidgetText->getWidth(), mWidgetText->getHeight());
		}

		for (VectorWidgetPtr::iterator iter=mVectorButton.begin(); iter!=mVectorButton.end(); ++iter) {
			(*iter)->setPosition(offset, mCoord.height - mButtonOffset.height, mButtonSize.width, mButtonSize.height);
			offset += mButtonOffset.width + mButtonSize.width;
		}
	}

} // namespace MyGUI
