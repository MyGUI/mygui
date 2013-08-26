/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_EditBox.h"
#include "MyGUI_Gui.h"
#include "MyGUI_ResourceSkin.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_InputManager.h"
#include "MyGUI_ClipboardManager.h"
#include "MyGUI_PointerManager.h"
#include "MyGUI_ISubWidgetText.h"
#include "MyGUI_ScrollBar.h"

#include <ctype.h>

namespace MyGUI
{

	const float EDIT_CURSOR_TIMER  = 0.7f;
	const float EDIT_ACTION_MOUSE_TIMER  = 0.05f;
	const int EDIT_CURSOR_MAX_POSITION = 100000;
	const int EDIT_CURSOR_MIN_POSITION = -100000;
	const size_t EDIT_MAX_UNDO = 128;
	const size_t EDIT_DEFAULT_MAX_TEXT_LENGTH = 2048;
	const float EDIT_OFFSET_HORZ_CURSOR = 10.0f; // дополнительное смещение для курсора
	const int EDIT_ACTION_MOUSE_ZONE = 1500; // область для восприятия мыши за пределом эдита
	const std::string EDIT_CLIPBOARD_TYPE_TEXT = "Text";
	const int EDIT_MOUSE_WHEEL = 50; // область для восприятия мыши за пределом эдита

	EditBox::EditBox() :
		mIsPressed(false),
		mIsFocus(false),
		mCursorActive(false),
		mCursorTimer(0),
		mActionMouseTimer(0),
		mCursorPosition(0),
		mTextLength(0),
		mStartSelect(ITEM_NONE),
		mEndSelect(0),
		mMouseLeftPressed(false),
		mModeReadOnly(false),
		mModePassword(false),
		mModeMultiline(false),
		mModeStatic(false),
		mModeWordWrap(false),
		mTabPrinting(false),
		mCharPassword('*'),
		mOverflowToTheLeft(false),
		mMaxTextLength(EDIT_DEFAULT_MAX_TEXT_LENGTH),
		mClientText(nullptr)
	{
		mChangeContentByResize = true;
	}

	void EditBox::initialiseOverride()
	{
		Base::initialiseOverride();

		mOriginalPointer = getPointer();

		// FIXME нам нужен фокус клавы
		setNeedKeyFocus(true);

		///@wskin_child{EditBox, Widget, Client} Клиентская зона.
		assignWidget(mClient, "Client");
		if (mClient != nullptr)
		{
			mClient->eventMouseSetFocus += newDelegate(this, &EditBox::notifyMouseSetFocus);
			mClient->eventMouseLostFocus += newDelegate(this, &EditBox::notifyMouseLostFocus);
			mClient->eventMouseButtonPressed += newDelegate(this, &EditBox::notifyMousePressed);
			mClient->eventMouseButtonReleased += newDelegate(this, &EditBox::notifyMouseReleased);
			mClient->eventMouseDrag += newDelegate(this, &EditBox::notifyMouseDrag);
			mClient->eventMouseButtonDoubleClick += newDelegate(this, &EditBox::notifyMouseButtonDoubleClick);
			mClient->eventMouseWheel += newDelegate(this, &EditBox::notifyMouseWheel);
			setWidgetClient(mClient);
		}

		///@wskin_child{EditBox, ScrollBar, VScroll} Вертикальная полоса прокрутки.
		assignWidget(mVScroll, "VScroll");
		if (mVScroll != nullptr)
		{
			mVScroll->eventScrollChangePosition += newDelegate(this, &EditBox::notifyScrollChangePosition);
		}

		///@wskin_child{EditBox, ScrollBar, HScroll} Горизонтальная полоса прокрутки.
		assignWidget(mHScroll, "HScroll");
		if (mHScroll != nullptr)
		{
			mHScroll->eventScrollChangePosition += newDelegate(this, &EditBox::notifyScrollChangePosition);
		}

		mClientText = getSubWidgetText();
		if (mClient != nullptr)
		{
			ISubWidgetText* text = mClient->getSubWidgetText();
			if (text)
				mClientText = text;
		}

		updateScrollSize();

		// первоначальная инициализация курсора
		if (mClientText != nullptr)
			mClientText->setCursorPosition(mCursorPosition);

		updateSelectText();
	}

	void EditBox::shutdownOverride()
	{
		mClient = nullptr;
		mClientText = nullptr;
		mVScroll = nullptr;
		mHScroll = nullptr;

		Base::shutdownOverride();
	}

	void EditBox::notifyMouseSetFocus(Widget* _sender, Widget* _old)
	{
		if ((_old == mClient) || (mIsFocus))
			return;

		mIsFocus = true;
		updateEditState();
	}

	void EditBox::notifyMouseLostFocus(Widget* _sender, Widget* _new)
	{
		if ((_new == mClient) || (!mIsFocus))
			return;

		mIsFocus = false;
		updateEditState();
	}

	void EditBox::notifyMousePressed(Widget* _sender, int _left, int _top, MouseButton _id)
	{
		if (mClientText == nullptr)
			return;

		// в статике все недоступно
		if (mModeStatic)
			return;

		IntPoint point = InputManager::getInstance().getLastPressedPosition(MouseButton::Left);
		mCursorPosition = mClientText->getCursorPosition(point);
		mClientText->setCursorPosition(mCursorPosition);
		mClientText->setVisibleCursor(true);
		mCursorTimer = 0;
		updateSelectText();

		if (_id == MouseButton::Left)
			mMouseLeftPressed = true;
	}

	void EditBox::notifyMouseReleased(Widget* _sender, int _left, int _top, MouseButton _id)
	{
		// сбрасываем всегда
		mMouseLeftPressed = false;
	}

	void EditBox::notifyMouseDrag(Widget* _sender, int _left, int _top, MouseButton _id)
	{
		if (_id != MouseButton::Left)
			return;

		if (mClientText == nullptr)
			return;

		// в статике все недоступно
		if (mModeStatic)
			return;

		// останавливаем курсор
		mClientText->setVisibleCursor(true);

		// сбрасываем все таймеры
		mCursorTimer = 0;
		mActionMouseTimer = 0;

		size_t Old = mCursorPosition;
		IntPoint point(_left, _top);
		mCursorPosition = mClientText->getCursorPosition(point);
		if (Old == mCursorPosition)
			return;

		mClientText->setCursorPosition(mCursorPosition);

		// если не было выделения
		if (mStartSelect == ITEM_NONE)
			mStartSelect = Old;

		// меняем выделение
		mEndSelect = (size_t)mCursorPosition;
		if (mStartSelect > mEndSelect)
			mClientText->setTextSelection(mEndSelect, mStartSelect);
		else
			mClientText->setTextSelection(mStartSelect, mEndSelect);

	}

	void EditBox::notifyMouseButtonDoubleClick(Widget* _sender)
	{
		if (mClientText == nullptr)
			return;

		// в статике все недоступно
		if (mModeStatic)
			return;

		const IntPoint& lastPressed = InputManager::getInstance().getLastPressedPosition(MouseButton::Left);

		size_t cursorPosition = mClientText->getCursorPosition(lastPressed);
		mStartSelect = cursorPosition;
		mEndSelect = cursorPosition;

		UString text = this->getOnlyText();
		UString::reverse_iterator iterBack = text.rend() - cursorPosition;
		UString::iterator iterForw = text.begin() + cursorPosition;

		while (iterBack != text.rend())
		{
			if (((*iterBack) < 265) && (ispunct(*iterBack) || isspace(*iterBack)))
				break;
			++iterBack;
			mStartSelect--;
		}
		while (iterForw != text.end())
		{
			if (((*iterForw) < 265) && (ispunct(*iterForw) || isspace(*iterForw)))
				break;
			++iterForw;
			mEndSelect++;
		}

		mClientText->setCursorPosition(mEndSelect);
		mClientText->setTextSelection(mStartSelect, mEndSelect);
	}

	void EditBox::onMouseDrag(int _left, int _top, MouseButton _id)
	{
		notifyMouseDrag(nullptr, _left, _top, _id);

		Base::onMouseDrag(_left, _top, _id);
	}

	void EditBox::onKeySetFocus(Widget* _old)
	{
		if (!mIsPressed)
		{
			mIsPressed = true;
			updateEditState();

			if (!mModeStatic)
			{
				if (mClientText != nullptr)
				{
					mCursorActive = true;
					Gui::getInstance().eventFrameStart += newDelegate(this, &EditBox::frameEntered);
					mClientText->setVisibleCursor(true);
					mClientText->setSelectBackground(true);
					mCursorTimer = 0;
				}
			}
		}

		Base::onKeySetFocus(_old);
	}

	void EditBox::onKeyLostFocus(Widget* _new)
	{
		if (mIsPressed)
		{
			mIsPressed = false;
			updateEditState();

			if (mClientText != nullptr)
			{
				mCursorActive = false;
				Gui::getInstance().eventFrameStart -= newDelegate(this, &EditBox::frameEntered);
				mClientText->setVisibleCursor(false);
				mClientText->setSelectBackground(false);
			}
		}

		Base::onKeyLostFocus(_new);
	}

	void EditBox::onKeyButtonPressed(KeyCode _key, Char _char)
	{
		if (mClientText == nullptr || mClient == nullptr)
		{
			Base::onKeyButtonPressed(_key, _char);
			return;
		}

		// в статическом режиме ничего не доступно
		if (mModeStatic)
		{
			Base::onKeyButtonPressed(_key, _char);
			return;
		}

		InputManager& input = InputManager::getInstance();

		mClientText->setVisibleCursor(true);
		mCursorTimer = 0.0f;

		if (_key == KeyCode::Escape)
		{
			InputManager::getInstance().setKeyFocusWidget(nullptr);
		}
		else if (_key == KeyCode::Backspace)
		{
			// если нуно то удаляем выделенный текст
			if (!mModeReadOnly)
			{
				// сбрасываем повтор
				commandResetRedo();

				if (!deleteTextSelect(true))
				{
					// прыгаем на одну назад и удаляем
					if (mCursorPosition != 0)
					{
						mCursorPosition--;
						eraseText(mCursorPosition, 1, true);
					}
				}
				// отсылаем событие о изменении
				eventEditTextChange(this);
			}

		}
		else if (_key == KeyCode::Delete)
		{
			if (input.isShiftPressed())
			{
				// сбрасываем повтор
				commandResetRedo();

				commandCut();
			}
			else if (!mModeReadOnly)
			{
				// сбрасываем повтор
				commandResetRedo();

				// если нуно то удаляем выделенный текст
				if (!deleteTextSelect(true))
				{
					if (mCursorPosition != mTextLength)
					{
						eraseText(mCursorPosition, 1, true);
					}
				}
				// отсылаем событие о изменении
				eventEditTextChange(this);
			}

		}
		else if (_key == KeyCode::Insert)
		{
			if (input.isShiftPressed())
			{
				// сбрасываем повтор
				commandResetRedo();

				commandPast();
			}
			else if (input.isControlPressed())
			{
				commandCopy();
			}

		}
		else if ((_key == KeyCode::Return) || (_key == KeyCode::NumpadEnter))
		{
			// работаем только в режиме редактирования
			if (!mModeReadOnly)
			{
				if ((mModeMultiline) && (!input.isControlPressed()))
				{
					// сбрасываем повтор
					commandResetRedo();

					// попытка объединения двух комманд
					size_t size = mVectorUndoChangeInfo.size();
					// непосредственно операции
					deleteTextSelect(true);
					insertText(TextIterator::getTextNewLine(), mCursorPosition, true);
					// проверяем на возможность объединения
					if ((size + 2) == mVectorUndoChangeInfo.size())
						commandMerge();
					// отсылаем событие о изменении
					eventEditTextChange(this);
				}
				// при сингл лайн и и мульти+сонтрол шлем эвент
				else
				{
					eventEditSelectAccept(this);
				}
			}

		}
		else if (_key == KeyCode::ArrowRight)
		{
			if ((mCursorPosition) < mTextLength)
			{
				mCursorPosition ++;
				mClientText->setCursorPosition(mCursorPosition);
				updateSelectText();
			}
			// сбрасываем выделение
			else if (isTextSelection() && !input.isShiftPressed())
			{
				resetSelect();
			}

		}
		else if (_key == KeyCode::ArrowLeft)
		{
			if (mCursorPosition != 0)
			{
				mCursorPosition --;
				mClientText->setCursorPosition(mCursorPosition);
				updateSelectText();
			}
			// сбрасываем выделение
			else if (isTextSelection() && !input.isShiftPressed())
			{
				resetSelect();
			}

		}
		else if (_key == KeyCode::ArrowUp)
		{
			IntPoint point = mClientText->getCursorPoint(mCursorPosition);
			point.top -= mClientText->getFontHeight();
			size_t old = mCursorPosition;
			mCursorPosition = mClientText->getCursorPosition(point);
			// самая верхняя строчка
			if (old == mCursorPosition)
			{
				if (mCursorPosition != 0)
				{
					mCursorPosition = 0;
					mClientText->setCursorPosition(mCursorPosition);
					updateSelectText();
				}
				// сбрасываем выделение
				else if (isTextSelection() && !input.isShiftPressed())
				{
					resetSelect();
				}
			}
			else
			{
				mClientText->setCursorPosition(mCursorPosition);
				updateSelectText();
			}

		}
		else if (_key == KeyCode::ArrowDown)
		{
			IntPoint point = mClientText->getCursorPoint(mCursorPosition);
			point.top += mClientText->getFontHeight();
			size_t old = mCursorPosition;
			mCursorPosition = mClientText->getCursorPosition(point);
			// самая нижняя строчка
			if (old == mCursorPosition)
			{
				if (mCursorPosition != mTextLength)
				{
					mCursorPosition = mTextLength;
					mClientText->setCursorPosition(mCursorPosition);
					updateSelectText();
				}
				// сбрасываем выделение
				else if (isTextSelection() && !input.isShiftPressed())
				{
					resetSelect();
				}
			}
			else
			{
				mClientText->setCursorPosition(mCursorPosition);
				updateSelectText();
			}

		}
		else if (_key == KeyCode::Home)
		{
			// в начало строки
			if (!input.isControlPressed())
			{
				IntPoint point = mClientText->getCursorPoint(mCursorPosition);
				point.left = EDIT_CURSOR_MIN_POSITION;
				size_t old = mCursorPosition;
				mCursorPosition = mClientText->getCursorPosition(point);
				if (old != mCursorPosition)
				{
					mClientText->setCursorPosition(mCursorPosition);
					updateSelectText();
				}
				else if (isTextSelection() && !input.isShiftPressed())
				{
					resetSelect();
				}
			}
			// в начало всего текста
			else
			{
				if (0 != mCursorPosition)
				{
					mCursorPosition = 0;
					mClientText->setCursorPosition(mCursorPosition);
					updateSelectText();
				}
				else if (isTextSelection() && !input.isShiftPressed())
				{
					resetSelect();
				}
			}

		}
		else if (_key == KeyCode::End)
		{
			// в конец строки
			if (!input.isControlPressed())
			{
				IntPoint point = mClientText->getCursorPoint(mCursorPosition);
				point.left = EDIT_CURSOR_MAX_POSITION;
				size_t old = mCursorPosition;
				mCursorPosition = mClientText->getCursorPosition(point);
				if (old != mCursorPosition)
				{
					mClientText->setCursorPosition(mCursorPosition);
					updateSelectText();
				}
				else if (isTextSelection() && !input.isShiftPressed())
				{
					resetSelect();
				}
			}
			// в самый конец
			else
			{
				if (mTextLength != mCursorPosition)
				{
					mCursorPosition = mTextLength;
					mClientText->setCursorPosition(mCursorPosition);
					updateSelectText();
				}
				else if (isTextSelection() && !input.isShiftPressed())
				{
					resetSelect();
				}
			}

		}
		else if (_key == KeyCode::PageUp)
		{
			// на размер окна, но не меньше одной строки
			IntPoint point = mClientText->getCursorPoint(mCursorPosition);
			point.top -= (mClient->getHeight() > mClientText->getFontHeight()) ? mClient->getHeight() : mClientText->getFontHeight();
			size_t old = mCursorPosition;
			mCursorPosition = mClientText->getCursorPosition(point);
			// самая верхняя строчка
			if (old == mCursorPosition)
			{
				if (mCursorPosition != 0)
				{
					mCursorPosition = 0;
					mClientText->setCursorPosition(mCursorPosition);
					updateSelectText();
				}
				// сбрасываем выделение
				else if (isTextSelection() && !input.isShiftPressed())
				{
					resetSelect();
				}
			}
			else
			{
				mClientText->setCursorPosition(mCursorPosition);
				updateSelectText();
			}

		}
		else if (_key == KeyCode::PageDown)
		{
			// на размер окна, но не меньше одной строки
			IntPoint point = mClientText->getCursorPoint(mCursorPosition);
			point.top += (mClient->getHeight() > mClientText->getFontHeight()) ? mClient->getHeight() : mClientText->getFontHeight();
			size_t old = mCursorPosition;
			mCursorPosition = mClientText->getCursorPosition(point);
			// самая нижняя строчка
			if (old == mCursorPosition)
			{
				if (mCursorPosition != mTextLength)
				{
					mCursorPosition = mTextLength;
					mClientText->setCursorPosition(mCursorPosition);
					updateSelectText();
				}
				// сбрасываем выделение
				else if (isTextSelection() && !input.isShiftPressed())
				{
					resetSelect();
				}
			}
			else
			{
				mClientText->setCursorPosition(mCursorPosition);
				updateSelectText();
			}

		}
		else if ((_key == KeyCode::LeftShift) || (_key == KeyCode::RightShift))
		{
			// для правильно выделения
			if (mStartSelect == ITEM_NONE)
			{
				mStartSelect = mEndSelect = mCursorPosition;
			}
		}
		else
		{
			// если не нажат контрл, то обрабатываем как текст
			if (!input.isControlPressed())
			{
				if (!mModeReadOnly && _char != 0)
				{
					// сбрасываем повтор
					commandResetRedo();

					// таб только если нужно
					if (_char != '\t' || mTabPrinting)
					{
						// попытка объединения двух комманд
						size_t size = mVectorUndoChangeInfo.size();
						// непосредственно операции
						deleteTextSelect(true);
						insertText(TextIterator::getTextCharInfo(_char), mCursorPosition, true);
						// проверяем на возможность объединения
						if ((size + 2) == mVectorUndoChangeInfo.size())
							commandMerge();
						// отсылаем событие о изменении
						eventEditTextChange(this);
					}
				}
			}
			else if (_key == KeyCode::C)
			{
				commandCopy();

			}
			else if (_key == KeyCode::X)
			{
				// сбрасываем повтор
				commandResetRedo();

				commandCut();

			}
			else if (_key == KeyCode::V)
			{
				// сбрасываем повтор
				commandResetRedo();

				commandPast();

			}
			else if (_key == KeyCode::A)
			{
				// выделяем весь текст
				setTextSelection(0, mTextLength);

			}
			else if (_key == KeyCode::Z)
			{
				// отмена
				commandUndo();

			}
			else if (_key == KeyCode::Y)
			{
				// повтор
				commandRedo();

			}
		}

		Base::onKeyButtonPressed(_key, _char);
	}

	void EditBox::frameEntered(float _frame)
	{
		if (mClientText == nullptr)
			return;

		// в статике все недоступно
		if (mModeStatic)
			return;

		if (mCursorActive)
		{
			mCursorTimer += _frame;

			if (mCursorTimer > EDIT_CURSOR_TIMER)
			{
				mClientText->setVisibleCursor(!mClientText->isVisibleCursor());
				while (mCursorTimer > EDIT_CURSOR_TIMER)
					mCursorTimer -= EDIT_CURSOR_TIMER;
			}
		}

		// сдвигаем курсор по положению мыши
		if (mMouseLeftPressed)
		{
			mActionMouseTimer += _frame;

			if (mActionMouseTimer > EDIT_ACTION_MOUSE_TIMER)
			{
				IntPoint mouse = InputManager::getInstance().getMousePositionByLayer();
				const IntRect& view = mClient->getAbsoluteRect();
				mouse.left -= view.left;
				mouse.top -= view.top;
				IntPoint point;

				bool action = false;

				// вверх на одну строчку
				if ((mouse.top < 0) && (mouse.top > -EDIT_ACTION_MOUSE_ZONE))
				{
					if ((mouse.left > 0) && (mouse.left <= mClient->getWidth()))
					{
						point = mClientText->getCursorPoint(mCursorPosition);
						point.top -= mClientText->getFontHeight();
						action = true;
					}
				}
				// вниз на одну строчку
				else if ((mouse.top > mClient->getHeight()) && (mouse.top < (mClient->getHeight() + EDIT_ACTION_MOUSE_ZONE)))
				{
					if ((mouse.left > 0) && (mouse.left <= mClient->getWidth()))
					{
						point = mClientText->getCursorPoint(mCursorPosition);
						point.top += mClientText->getFontHeight();
						action = true;
					}
				}

				// влево на небольшое расстояние
				if ((mouse.left < 0) && (mouse.left > -EDIT_ACTION_MOUSE_ZONE))
				{
					point = mClientText->getCursorPoint(mCursorPosition);
					point.left -= (int)EDIT_OFFSET_HORZ_CURSOR;
					action = true;
				}
				// вправо на небольшое расстояние
				else if ((mouse.left > mClient->getWidth()) && (mouse.left < (mClient->getWidth() + EDIT_ACTION_MOUSE_ZONE)))
				{
					point = mClientText->getCursorPoint(mCursorPosition);
					point.left += (int)EDIT_OFFSET_HORZ_CURSOR;
					action = true;
				}

				if (action)
				{
					size_t old = mCursorPosition;
					mCursorPosition = mClientText->getCursorPosition(point);

					if (old != mCursorPosition)
					{
						mClientText->setCursorPosition(mCursorPosition);

						mEndSelect = (size_t)mCursorPosition;
						if (mStartSelect > mEndSelect)
							mClientText->setTextSelection(mEndSelect, mStartSelect);
						else
							mClientText->setTextSelection(mStartSelect, mEndSelect);

						// пытаемся показать курсор
						updateViewWithCursor();
					}
				}
				// если в зону не попадает то сбрасываем
				else
				{
					mActionMouseTimer = 0;
				}

				while (mActionMouseTimer > EDIT_ACTION_MOUSE_TIMER)
					mActionMouseTimer -= EDIT_ACTION_MOUSE_TIMER;
			}

		} // if (mMouseLeftPressed)
	}

	void EditBox::setTextCursor(size_t _index)
	{
		// сбрасываем выделение
		resetSelect();

		// новая позиция
		if (_index > mTextLength)
			_index = mTextLength;

		if (mCursorPosition == _index)
			return;

		mCursorPosition = _index;

		// обновляем по позиции
		if (mClientText != nullptr)
			mClientText->setCursorPosition(mCursorPosition);

		updateSelectText();
	}

	void EditBox::setTextSelection(size_t _start, size_t _end)
	{
		if (_start > mTextLength)
			_start = mTextLength;
		if (_end > mTextLength)
			_end = mTextLength;

		mStartSelect = _start;
		mEndSelect = _end;

		if (mClientText != nullptr)
		{
			if (mStartSelect > mEndSelect)
				mClientText->setTextSelection(mEndSelect, mStartSelect);
			else
				mClientText->setTextSelection(mStartSelect, mEndSelect);
		}

		if (mCursorPosition == mEndSelect)
			return;
		// курсор на конец выделения
		mCursorPosition = mEndSelect;

		// обновляем по позиции
		if (mClientText != nullptr)
			mClientText->setCursorPosition(mCursorPosition);
	}

	bool EditBox::deleteTextSelect(bool _history)
	{
		if (!isTextSelection())
			return false;

		// начало и конец выделения
		size_t start = getTextSelectionStart();
		size_t end =  getTextSelectionEnd();

		eraseText(start, end - start, _history);

		return true;
	}

	void EditBox::resetSelect()
	{
		if (mStartSelect != ITEM_NONE)
		{
			mStartSelect = ITEM_NONE;
			if (mClientText != nullptr)
				mClientText->setTextSelection(0, 0);
		}
	}

	void EditBox::commandPosition(size_t _undo, size_t _redo, size_t _length, VectorChangeInfo* _info)
	{
		if (_info != nullptr)
			_info->push_back(TextCommandInfo(_undo, _redo, _length));
	}

	void EditBox::commandMerge()
	{
		if (mVectorUndoChangeInfo.size() < 2)
			return; // на всякий
		// сохраняем последние набор отмен
		VectorChangeInfo info = mVectorUndoChangeInfo.back();
		mVectorUndoChangeInfo.pop_back();

		// объединяем последовательности
		for (VectorChangeInfo::iterator iter = info.begin(); iter != info.end(); ++iter)
		{
			mVectorUndoChangeInfo.back().push_back((*iter));
		}
	}

	bool EditBox::commandUndo()
	{
		if (mVectorUndoChangeInfo.empty())
			return false;

		// сбрасываем выделение
		resetSelect();

		// сохраняем последние набор отмен
		VectorChangeInfo info = mVectorUndoChangeInfo.back();
		// перекидываем последний набор отмен
		mVectorUndoChangeInfo.pop_back();
		mVectorRedoChangeInfo.push_back(info);

		// берем текст для издевательств
		UString text = getRealString();

		// восстанавливаем последовательность
		for (VectorChangeInfo::reverse_iterator iter = info.rbegin(); iter != info.rend(); ++iter)
		{
			if ((*iter).type == TextCommandInfo::COMMAND_INSERT)
				text.erase((*iter).start, (*iter).text.size());
			else if ((*iter).type == TextCommandInfo::COMMAND_ERASE)
				text.insert((*iter).start, (*iter).text);
			else
			{
				mCursorPosition = (*iter).undo;
				mTextLength = (*iter).length;
			}
		}

		// возвращаем текст
		setRealString(text);

		// обновляем по позиции
		if (mClientText != nullptr)
			mClientText->setCursorPosition(mCursorPosition);
		updateSelectText();

		// отсылаем событие о изменении
		eventEditTextChange(this);

		return true;
	}

	bool EditBox::commandRedo()
	{
		if (mVectorRedoChangeInfo.empty())
			return false;

		// сбрасываем выделение
		resetSelect();

		// сохраняем последние набор отмен
		VectorChangeInfo info = mVectorRedoChangeInfo.back();
		// перекидываем последний набор отмен
		mVectorRedoChangeInfo.pop_back();
		mVectorUndoChangeInfo.push_back(info);

		// берем текст для издевательств
		UString text = getRealString();

		// восстанавливаем последовательность
		for (VectorChangeInfo::iterator iter = info.begin(); iter != info.end(); ++iter)
		{
			if ((*iter).type == TextCommandInfo::COMMAND_INSERT)
				text.insert((*iter).start, (*iter).text);
			else if ((*iter).type == TextCommandInfo::COMMAND_ERASE)
				text.erase((*iter).start, (*iter).text.size());
			else
			{
				mCursorPosition = (*iter).redo;
				mTextLength = (*iter).length;
			}

		}

		// возвращаем текст
		setRealString(text);

		// обновляем по позиции
		if (mClientText != nullptr)
			mClientText->setCursorPosition(mCursorPosition);
		updateSelectText();

		// отсылаем событие о изменении
		eventEditTextChange(this);

		return true;
	}

	void EditBox::saveInHistory(VectorChangeInfo* _info)
	{
		if (_info == nullptr)
			return;
		// если нет информации об изменении
		if ( _info->empty())
			return;
		if ((_info->size() == 1) && (_info->back().type == TextCommandInfo::COMMAND_POSITION))
			return;

		mVectorUndoChangeInfo.push_back(*_info);
		// проверяем на максимальный размер
		if (mVectorUndoChangeInfo.size() > EDIT_MAX_UNDO)
			mVectorUndoChangeInfo.pop_front();
	}

	// возвращает текст
	UString EditBox::getTextInterval(size_t _start, size_t _count)
	{
		// подстраховка
		if (_start > mTextLength) _start = mTextLength;
		// конец диапазона
		size_t end = _start + _count;

		// итератор нашей строки
		TextIterator iterator(getRealString());

		// дефолтный цвет
		UString colour = mClientText == nullptr ? "" : TextIterator::convertTagColour(mClientText->getTextColour());

		// нужно ли вставлять цвет
		bool need_colour = true;

		// цикл прохода по строке
		while (iterator.moveNext())
		{
			// текущаяя позиция
			size_t pos = iterator.getPosition();

			// еще рано
			if (pos < _start)
			{
				// берем цвет из позиции и запоминаем
				iterator.getTagColour(colour);

				continue;
			}

			// проверяем на надобность начального тега
			else if (pos == _start)
			{
				need_colour = ! iterator.getTagColour(colour);
				// сохраняем место откуда начинается
				iterator.saveStartPoint();

			}

			// а теперь просто до конца диапазона
			else if (pos == end)
				break;

		}

		// возвращаем строку
		if (need_colour)
			return colour + iterator.getFromStart();
		return iterator.getFromStart();
	}

	// выделяет цветом диапазон
	void EditBox::_setTextColour(size_t _start, size_t _count, const Colour& _colour, bool _history)
	{
		// история изменений
		VectorChangeInfo* history = nullptr;
		if (_history)
			history = new VectorChangeInfo();

		// конец диапазона
		size_t end = _start + _count;

		// итератор нашей строки
		TextIterator iterator(getRealString(), history);

		// дефолтный цвет
		UString colour = mClientText == nullptr ? "" : TextIterator::convertTagColour(mClientText->getTextColour());

		// цикл прохода по строке
		while (iterator.moveNext())
		{
			// текущаяя позиция
			size_t pos = iterator.getPosition();

			// берем цвет из позиции и запоминаем
			iterator.getTagColour(colour);

			// еще рано
			if (pos < _start)
				continue;

			// ставим начальный тег
			else if (pos == _start)
				iterator.setTagColour(_colour);

			// внутри диапазона очищаем все
			else if (pos < end)
				iterator.clearTagColour();

			// на конец ставим последний найденный или дефолтный
			else if (pos == end)
			{
				iterator.setTagColour(colour);
				// и выходим из цикла
				break;
			}

		}

		// сохраняем позицию для восстановления курсора
		commandPosition(_start, _start + _count, mTextLength, history);

		// запоминаем в историю
		if (_history)
		{
			saveInHistory(history);
			delete history;
		}
		// сбрасываем историю
		else
			commandResetHistory();

		// и возвращаем строку на место
		setRealString(iterator.getText());
	}

	void EditBox::setTextSelectColour(const Colour& _colour, bool _history)
	{
		// нужно выделение
		if ( !isTextSelection())
			return;
		// начало и конец выделения
		size_t start = getTextSelectionStart();
		size_t end =  getTextSelectionEnd();
		_setTextColour(start, end - start, _colour, _history);
	}

	UString EditBox::getTextSelection()
	{
		if ( !isTextSelection())
			return "";
		size_t start = getTextSelectionStart();
		size_t end =  getTextSelectionEnd();
		return getTextInterval(start, end - start);
	}

	void EditBox::setEditPassword(bool _password)
	{
		if (mModePassword == _password)
			return;
		mModePassword = _password;

		if (mModePassword)
		{
			if (mClientText != nullptr)
			{
				mPasswordText = mClientText->getCaption();
				mClientText->setCaption(UString(mTextLength, '*'));
			}
		}
		else
		{
			if (mClientText != nullptr)
			{
				mClientText->setCaption(mPasswordText);
				mPasswordText.clear();
			}
		}
		// обновляем по размерам
		updateView();
		// сбрасываем историю
		commandResetHistory();
	}

	void EditBox::setText(const UString& _caption, bool _history)
	{
		// сбрасываем выделение
		resetSelect();

		// история изменений
		VectorChangeInfo* history = nullptr;
		if (_history)
			history = new VectorChangeInfo();

		// итератор нашей строки
		TextIterator iterator(getRealString(), history);

		// вставляем текст
		iterator.setText(_caption, mModeMultiline || mModeWordWrap);

		if (mOverflowToTheLeft)
		{
			iterator.cutMaxLengthFromBeginning(mMaxTextLength);
		}
		else
		{
			// обрезаем по максимальной длинне
			iterator.cutMaxLength(mMaxTextLength);
		}

		// запоминаем размер строки
		size_t old = mTextLength;
		// новая позиция и положение на конец вставки
		mCursorPosition = mTextLength = iterator.getSize();

		// сохраняем позицию для восстановления курсора
		commandPosition(0, mTextLength, old, history);

		// запоминаем в историю
		if (_history)
		{
			saveInHistory(history);
			delete history;
		}
		// сбрасываем историю
		else
			commandResetHistory();

		// и возвращаем строку на место
		setRealString(iterator.getText());

		// обновляем по позиции
		if (mClientText != nullptr)
			mClientText->setCursorPosition(mCursorPosition);
		updateSelectText();
	}

	void EditBox::insertText(const UString& _text, size_t _start, bool _history)
	{
		// сбрасываем выделение
		resetSelect();

		// если строка пустая, или размер максимален
		if (_text.empty())
			return;

		if ((mOverflowToTheLeft == false) && (mTextLength == mMaxTextLength))
			return;

		// история изменений
		VectorChangeInfo* history = nullptr;
		if (_history)
			history = new VectorChangeInfo();

		// итератор нашей строки
		TextIterator iterator(getRealString(), history);

		// дефолтный цвет
		UString colour = mClientText == nullptr ? "" : TextIterator::convertTagColour(mClientText->getTextColour());
		// нужен ли тег текста
		// потом переделать через TextIterator чтобы отвязать понятие тег от эдита
		bool need_colour = ( (_text.size() > 6) && (_text[0] == L'#') && (_text[1] != L'#') );

		// цикл прохода по строке
		while (iterator.moveNext())
		{
			// текущаяя позиция
			size_t pos = iterator.getPosition();

			// текущий цвет
			if (need_colour)
				iterator.getTagColour(colour);

			// если дошли то выходим
			if (pos == _start)
				break;
		}

		// если нужен цвет то вставляем
		if (need_colour)
			iterator.setTagColour(colour);

		// а теперь вставляем строку
		iterator.insertText(_text, mModeMultiline || mModeWordWrap);

		if (mOverflowToTheLeft)
		{
			iterator.cutMaxLengthFromBeginning(mMaxTextLength);
		}
		else
		{
			// обрезаем по максимальной длинне
			iterator.cutMaxLength(mMaxTextLength);
		}

		// запоминаем размер строки
		size_t old = mTextLength;
		// новая позиция и положение на конец вставки
		mTextLength = iterator.getSize();
		mCursorPosition += mTextLength - old;

		// сохраняем позицию для восстановления курсора
		commandPosition(_start, _start + mTextLength - old, old, history);

		// запоминаем в историю
		if (_history)
		{
			saveInHistory(history);
			delete history;
		}
		// сбрасываем историю
		else
			commandResetHistory();

		// и возвращаем строку на место
		setRealString(iterator.getText());

		// обновляем по позиции
		if (mClientText != nullptr)
			mClientText->setCursorPosition(mCursorPosition);
		updateSelectText();
	}

	void EditBox::eraseText(size_t _start, size_t _count, bool _history)
	{
		// чета маловато
		if (_count == 0)
			return;

		// сбрасываем выделение
		resetSelect();

		// история изменений
		VectorChangeInfo* history = nullptr;
		if (_history)
			history = new VectorChangeInfo();

		// итератор нашей строки
		TextIterator iterator(getRealString(), history);

		// дефолтный цвет
		UString colour;
		// конец диапазона
		size_t end = _start + _count;
		bool need_colour = false;

		// цикл прохода по строке
		while (iterator.moveNext())
		{
			// текущаяя позиция
			size_t pos = iterator.getPosition();

			// еще рано
			if (pos < _start)
			{
				// берем цвет из позиции и запоминаем
				iterator.getTagColour(colour);
				continue;
			}

			// сохраняем место откуда начинается
			else if (pos == _start)
			{
				// если до диапазона был цвет, то нужно закрыть тег
				if (!colour.empty())
				{
					need_colour = true;
					colour.clear();
				}
				// берем цвет из позиции и запоминаем
				iterator.getTagColour(colour);
				iterator.saveStartPoint();
			}

			// внутри диапазона
			else if (pos < end)
			{
				// берем цвет из позиции и запоминаем
				iterator.getTagColour(colour);
			}

			// окончание диапазона
			else if (pos == end)
			{
				// нужно ставить тег или нет
				if (!colour.empty())
					need_colour = true;
				if (iterator.getTagColour(colour))
					need_colour = false;

				break;
			}

		}

		// удаляем диапазон
		iterator.eraseFromStart();
		// и вставляем последний цвет
		if (need_colour)
			iterator.setTagColour(colour);

		// сохраняем позицию для восстановления курсора
		commandPosition(_start + _count, _start, mTextLength, history);

		// на месте удаленного
		mCursorPosition = _start;
		mTextLength -= _count;

		// запоминаем в историю
		if (_history)
		{
			saveInHistory(history);
			delete history;
		}
		// сбрасываем историю
		else
			commandResetHistory();

		// и возвращаем строку на место
		setRealString(iterator.getText());

		// обновляем по позиции
		if (mClientText != nullptr)
			mClientText->setCursorPosition(mCursorPosition);
		updateSelectText();
	}

	void EditBox::commandCut()
	{
		// вырезаем в буфер обмена
		if (isTextSelection() && (!mModePassword))
		{
			ClipboardManager::getInstance().setClipboardData(EDIT_CLIPBOARD_TYPE_TEXT, getTextSelection());
			if (!mModeReadOnly)
			{
				deleteTextSelect(true);
				// отсылаем событие о изменении
				eventEditTextChange(this);
			}
		}
		else
			ClipboardManager::getInstance().clearClipboardData(EDIT_CLIPBOARD_TYPE_TEXT);
	}

	void EditBox::commandCopy()
	{
		// копируем в буфер обмена
		if (isTextSelection() && (!mModePassword))
			ClipboardManager::getInstance().setClipboardData(EDIT_CLIPBOARD_TYPE_TEXT, getTextSelection());
		else
			ClipboardManager::getInstance().clearClipboardData(EDIT_CLIPBOARD_TYPE_TEXT);
	}

	void EditBox::commandPast()
	{
		// копируем из буфера обмена
		std::string clipboard = ClipboardManager::getInstance().getClipboardData(EDIT_CLIPBOARD_TYPE_TEXT);
		if ((!mModeReadOnly) && (!clipboard.empty()))
		{
			// попытка объединения двух комманд
			size_t size = mVectorUndoChangeInfo.size();
			// непосредственно операции
			deleteTextSelect(true);
			insertText(clipboard, mCursorPosition, true);
			// проверяем на возможность объединения
			if ((size + 2) == mVectorUndoChangeInfo.size())
				commandMerge();
			// отсылаем событие о изменении
			eventEditTextChange(this);
		}
	}

	const UString& EditBox::getRealString()
	{
		if (mModePassword)
			return mPasswordText;
		else if (mClientText == nullptr)
			return mPasswordText;

		return mClientText->getCaption();
	}

	void EditBox::setRealString(const UString& _caption)
	{
		if (mModePassword)
		{
			mPasswordText = _caption;
			if (mClientText != nullptr)
				mClientText->setCaption(UString(mTextLength, (UString::code_point)mCharPassword));
		}
		else
		{
			if (mClientText != nullptr)
				mClientText->setCaption(_caption);
		}
	}

	void EditBox::setPasswordChar(Char _char)
	{
		mCharPassword = _char;
		if (mModePassword)
		{
			if (mClientText != nullptr)
				mClientText->setCaption(UString(mTextLength, (UString::code_point)mCharPassword));
		}
	}

	void EditBox::updateEditState()
	{
		if (!getInheritedEnabled())
		{
			_setWidgetState("disabled");
		}
		else if (mIsPressed)
		{
			if (mIsFocus)
				_setWidgetState("pushed");
			else
				_setWidgetState("normal_checked");
		}
		else if (mIsFocus)
		{
			_setWidgetState("highlighted");
		}
		else
		{
			_setWidgetState("normal");
		}
	}

	void EditBox::setPosition(const IntPoint& _point)
	{
		Base::setPosition(_point);
	}

	void EditBox::eraseView()
	{
		// если перенос, то сбрасываем размер текста
		if (mModeWordWrap)
		{
			if (mClientText != nullptr)
				mClientText->setWordWrap(true);
		}

		updateView();
	}

	void EditBox::setSize(const IntSize& _size)
	{
		Base::setSize(_size);

		eraseView();
	}

	void EditBox::setCoord(const IntCoord& _coord)
	{
		Base::setCoord(_coord);

		eraseView();
	}

	void EditBox::setCaption(const UString& _value)
	{
		setText(_value, false);
	}

	const UString& EditBox::getCaption()
	{
		return getRealString();
	}

	void EditBox::updateSelectText()
	{
		if (!mModeStatic)
		{
			InputManager& input = InputManager::getInstance();
			if ((input.isShiftPressed()) && (mStartSelect != ITEM_NONE))
			{
				// меняем выделение
				mEndSelect = (size_t)mCursorPosition;
				if (mClientText != nullptr)
				{
					if (mStartSelect > mEndSelect)
						mClientText->setTextSelection(mEndSelect, mStartSelect);
					else
						mClientText->setTextSelection(mStartSelect, mEndSelect);
				}

			}
			else if (mStartSelect != ITEM_NONE)
			{
				// сбрасываем шифт
				mStartSelect = ITEM_NONE;
				if (mClientText != nullptr)
					mClientText->setTextSelection(0, 0);
			}
		}

		// пытаемся показать курсор
		updateViewWithCursor();
	}

	void EditBox::setTextAlign(Align _value)
	{
		Base::setTextAlign(_value);

		if (mClientText != nullptr)
			mClientText->setTextAlign(_value);

		// так как мы сами рулим смещениями
		updateView();
	}

	void EditBox::setTextColour(const Colour& _value)
	{
		Base::setTextColour(_value);

		if (mClientText != nullptr)
			mClientText->setTextColour(_value);
	}

	IntCoord EditBox::getTextRegion()
	{
		if (mClientText != nullptr)
			return mClientText->getCoord();
		return Base::getTextRegion();
	}

	IntSize EditBox::getTextSize()
	{
		if (mClientText != nullptr)
			return mClientText->getTextSize();
		return Base::getTextSize();
	}

	void EditBox::notifyScrollChangePosition(ScrollBar* _sender, size_t _position)
	{
		if (mClientText == nullptr)
			return;

		if (_sender == mVScroll)
		{
			IntPoint point = mClientText->getViewOffset();
			point.top = _position;
			mClientText->setViewOffset(point);
		}
		else if (_sender == mHScroll)
		{
			IntPoint point = mClientText->getViewOffset();
			point.left = _position;
			mClientText->setViewOffset(point);
		}
	}

	void EditBox::notifyMouseWheel(Widget* _sender, int _rel)
	{
		if (mClientText == nullptr)
			return;

		if (mVRange != 0)
		{
			IntPoint point = mClientText->getViewOffset();
			int offset = point.top;
			if (_rel < 0)
				offset += EDIT_MOUSE_WHEEL;
			else
				offset -= EDIT_MOUSE_WHEEL;

			if (offset < 0)
				offset = 0;
			else if (offset > (int)mVRange)
				offset = mVRange;

			if (offset != point.top)
			{
				point.top = offset;
				if (mVScroll != nullptr)
					mVScroll->setScrollPosition(offset);
				mClientText->setViewOffset(point);
			}
		}
		else if (mHRange != 0)
		{
			IntPoint point = mClientText->getViewOffset();
			int offset = point.left;
			if (_rel < 0)
				offset += EDIT_MOUSE_WHEEL;
			else
				offset -= EDIT_MOUSE_WHEEL;

			if (offset < 0)
				offset = 0;
			else if (offset > (int)mHRange)
				offset = mHRange;

			if (offset != point.left)
			{
				point.left = offset;
				if (mHScroll != nullptr)
					mHScroll->setScrollPosition(offset);
				mClientText->setViewOffset(point);
			}
		}
	}

	void EditBox::setEditWordWrap(bool _value)
	{
		mModeWordWrap = _value;
		if (mClientText != nullptr)
			mClientText->setWordWrap(mModeWordWrap);

		eraseView();
	}

	void EditBox::setFontName(const std::string& _value)
	{
		Base::setFontName(_value);

		if (mClientText != nullptr)
			mClientText->setFontName(_value);

		eraseView();
	}

	void EditBox::setFontHeight(int _value)
	{
		Base::setFontHeight(_value);

		if (mClientText != nullptr)
			mClientText->setFontHeight(_value);

		eraseView();
	}

	void EditBox::updateView()
	{
		updateScrollSize();
		updateScrollPosition();
	}

	void EditBox::updateViewWithCursor()
	{
		updateScrollSize();
		updateCursorPosition();
		updateScrollPosition();
	}

	void EditBox::updateCursorPosition()
	{
		if (mClientText == nullptr || mClient == nullptr)
			return;

		// размер контекста текста
		IntSize textSize = mClientText->getTextSize();

		// текущее смещение контекста текста
		IntPoint point = mClientText->getViewOffset();
		// расчетное смещение
		IntPoint offset = point;

		// абсолютные координаты курсора
		IntRect cursor = mClientText->getCursorRect(mCursorPosition);
		cursor.right ++;

		// абсолютные координаты вью
		const IntRect& view = mClient->getAbsoluteRect();

		// проверяем и показываем курсор
		if (!view.inside(cursor))
		{
			// горизонтальное смещение
			if (textSize.width > view.width())
			{
				if (cursor.left < view.left)
				{
					offset.left = point.left - (view.left - cursor.left);
					// добавляем смещение, только если курсор не перепрыгнет
					if ((float(view.width()) - EDIT_OFFSET_HORZ_CURSOR) > EDIT_OFFSET_HORZ_CURSOR)
						offset.left -= int(EDIT_OFFSET_HORZ_CURSOR);
				}
				else if (cursor.right > view.right)
				{
					offset.left = point.left + (cursor.right - view.right);
					// добавляем смещение, только если курсор не перепрыгнет
					if ((float(view.width()) - EDIT_OFFSET_HORZ_CURSOR) > EDIT_OFFSET_HORZ_CURSOR)
						offset.left += int(EDIT_OFFSET_HORZ_CURSOR);
				}
			}

			// вертикальное смещение
			if (textSize.height > view.height())
			{
				int delta = 0;
				if (cursor.height() > view.height())
				{
					// if text is bigger than edit height then place it in center
					delta = ((cursor.bottom - view.bottom) - (view.top - cursor.top)) / 2;
				}
				else if (cursor.top < view.top)
				{
					delta = - (view.top - cursor.top);
				}
				else if (cursor.bottom > view.bottom)
				{
					delta = (cursor.bottom - view.bottom);
				}
				offset.top = point.top + delta;
			}

		}

		if (offset != point)
		{
			mClientText->setViewOffset(offset);
			// обновить скролы
			if (mVScroll != nullptr)
				mVScroll->setScrollPosition(offset.top);
			if (mHScroll != nullptr)
				mHScroll->setScrollPosition(offset.left);
		}
	}

	void EditBox::setContentPosition(const IntPoint& _point)
	{
		if (mClientText != nullptr)
			mClientText->setViewOffset(_point);
	}

	IntSize EditBox::getViewSize()
	{
		if (mClientText != nullptr)
			return mClientText->getSize();
		return ScrollViewBase::getViewSize();
	}

	IntSize EditBox::getContentSize()
	{
		if (mClientText != nullptr)
			return mClientText->getTextSize();
		return ScrollViewBase::getContentSize();
	}

	size_t EditBox::getVScrollPage()
	{
		if (mClientText != nullptr)
			return (size_t)mClientText->getFontHeight();
		return ScrollViewBase::getVScrollPage();
	}

	size_t EditBox::getHScrollPage()
	{
		if (mClientText != nullptr)
			return (size_t)mClientText->getFontHeight();
		return ScrollViewBase::getHScrollPage();
	}

	IntPoint EditBox::getContentPosition()
	{
		if (mClientText != nullptr)
			return mClientText->getViewOffset();
		return ScrollViewBase::getContentPosition();
	}

	Align EditBox::getContentAlign()
	{
		if (mClientText != nullptr)
			return mClientText->getTextAlign();
		return ScrollViewBase::getContentAlign();
	}

	void EditBox::setTextIntervalColour(size_t _start, size_t _count, const Colour& _colour)
	{
		_setTextColour(_start, _count, _colour, false);
	}

	size_t EditBox::getTextSelectionStart() const
	{
		return (mStartSelect == ITEM_NONE) ? ITEM_NONE : (mStartSelect > mEndSelect ? mEndSelect : mStartSelect);
	}

	size_t EditBox::getTextSelectionEnd() const
	{
		return (mStartSelect == ITEM_NONE) ? ITEM_NONE : (mStartSelect > mEndSelect ? mStartSelect : mEndSelect);
	}

	bool EditBox::isTextSelection() const
	{
		return (mStartSelect != ITEM_NONE) && (mStartSelect != mEndSelect);
	}

	void EditBox::deleteTextSelection()
	{
		deleteTextSelect(false);
	}

	void EditBox::setTextSelectionColour(const Colour& _colour)
	{
		setTextSelectColour(_colour, false);
	}

	size_t EditBox::getTextSelectionLength() const
	{
		return mEndSelect - mStartSelect;
	}

	void EditBox::setOnlyText(const UString& _text)
	{
		setText(TextIterator::toTagsString(_text), false);
	}

	UString EditBox::getOnlyText()
	{
		return TextIterator::getOnlyText(getRealString());
	}

	void EditBox::insertText(const UString& _text, size_t _index)
	{
		insertText(_text, _index, false);
	}

	void EditBox::addText(const UString& _text)
	{
		insertText(_text, ITEM_NONE, false);
	}

	void EditBox::eraseText(size_t _start, size_t _count)
	{
		eraseText(_start, _count, false);
	}

	void EditBox::setEditReadOnly(bool _value)
	{
		mModeReadOnly = _value;
		// сбрасываем историю
		commandResetHistory();
	}

	void EditBox::setEditMultiLine(bool _value)
	{
		mModeMultiline = _value;
		// на всякий, для убирания переносов
		if (!mModeMultiline)
		{
			setText(getRealString(), false);
		}
		// обновляем по размерам
		else
		{
			updateView();
		}
		// сбрасываем историю
		commandResetHistory();
	}

	void EditBox::setEditStatic(bool _value)
	{
		mModeStatic = _value;
		resetSelect();

		if (mClient != nullptr)
		{
			if (mModeStatic)
				mClient->setPointer("");
			else
				mClient->setPointer(mOriginalPointer);
		}
	}

	void EditBox::setPasswordChar(const UString& _value)
	{
		if (!_value.empty())
			setPasswordChar(_value[0]);
	}

	void EditBox::setVisibleVScroll(bool _value)
	{
		mVisibleVScroll = _value;
		updateView();
	}

	void EditBox::setVisibleHScroll(bool _value)
	{
		mVisibleHScroll = _value;
		updateView();
	}

	size_t EditBox::getVScrollRange() const
	{
		return mVRange + 1;
	}

	size_t EditBox::getVScrollPosition()
	{
		return mClientText == nullptr ? 0 : mClientText->getViewOffset().top;
	}

	void EditBox::setVScrollPosition(size_t _index)
	{
		if (mClientText == nullptr)
			return;

		if (_index > mVRange)
			_index = mVRange;

		IntPoint point = mClientText->getViewOffset();
		point.top = _index;

		mClientText->setViewOffset(point);
		// обновить скролы
		if (mVScroll != nullptr)
			mVScroll->setScrollPosition(point.top);
	}

	size_t EditBox::getHScrollRange() const
	{
		return mHRange + 1;
	}

	size_t EditBox::getHScrollPosition()
	{
		return mClientText == nullptr ? 0 : mClientText->getViewOffset().left;
	}

	void EditBox::setHScrollPosition(size_t _index)
	{
		if (mClientText == nullptr)
			return;

		if (_index > mHRange)
			_index = mHRange;

		IntPoint point = mClientText->getViewOffset();
		point.left = _index;

		mClientText->setViewOffset(point);
		// обновить скролы
		if (mHScroll != nullptr)
			mHScroll->setScrollPosition(point.left);
	}

	bool EditBox::getInvertSelected()
	{
		return mClientText == nullptr ? false : mClientText->getInvertSelected();
	}

	void EditBox::setInvertSelected(bool _value)
	{
		if (mClientText != nullptr)
			mClientText->setInvertSelected(_value);
	}

	void EditBox::setPropertyOverride(const std::string& _key, const std::string& _value)
	{
		/// @wproperty{EditBox, CursorPosition, size_t} Позиция курсора.
		if (_key == "CursorPosition")
			setTextCursor(utility::parseValue<size_t>(_value));

		/// @wproperty{EditBox, TextSelect, size_t size_t} Выделение текста.
		else if (_key == "TextSelect")
			setTextSelection(utility::parseValue< types::TSize<size_t> >(_value).width, utility::parseValue< types::TSize<size_t> >(_value).height);

		/// @wproperty{EditBox, ReadOnly, bool} Режим только для чтения, в этом режиме нельзя изменять текст но которовать можно.
		else if (_key == "ReadOnly")
			setEditReadOnly(utility::parseValue<bool>(_value));

		/// @wproperty{EditBox, Password, bool} Режим ввода пароля, все символы заменяются на звездочки или другие указаные символы.
		else if (_key == "Password")
			setEditPassword(utility::parseValue<bool>(_value));

		/// @wproperty{EditBox, MultiLine, bool} Режим много строчного ввода.
		else if (_key == "MultiLine")
			setEditMultiLine(utility::parseValue<bool>(_value));

		/// @wproperty{EditBox, PasswordChar, string} Символ для замены в режиме пароля.
		else if (_key == "PasswordChar")
			setPasswordChar(_value);

		/// @wproperty{EditBox, MaxTextLength, size_t} Максимальное длина текста.
		else if (_key == "MaxTextLength")
			setMaxTextLength(utility::parseValue<size_t>(_value));

		/// @wproperty{EditBox, OverflowToTheLeft, bool} Режим обрезки текста в начале, после того как его колличество достигает максимального значения.
		else if (_key == "OverflowToTheLeft")
			setOverflowToTheLeft(utility::parseValue<bool>(_value));

		/// @wproperty{EditBox, Static, bool} Статический режим, поле ввода никак не реагирует на пользовательский ввод.
		else if (_key == "Static")
			setEditStatic(utility::parseValue<bool>(_value));

		/// @wproperty{EditBox, VisibleVScroll, bool} Видимость вертикальной полосы прокрутки.
		else if (_key == "VisibleVScroll")
			setVisibleVScroll(utility::parseValue<bool>(_value));

		/// @wproperty{EditBox, VisibleHScroll, bool} Видимость горизонтальной полосы прокрутки.
		else if (_key == "VisibleHScroll")
			setVisibleHScroll(utility::parseValue<bool>(_value));

		/// @wproperty{EditBox, WordWrap, bool} Режим переноса по словам.
		else if (_key == "WordWrap")
			setEditWordWrap(utility::parseValue<bool>(_value));

		/// @wproperty{EditBox, TabPrinting, bool} Воспринимать нажатие на Tab как символ табуляции.
		else if (_key == "TabPrinting")
			setTabPrinting(utility::parseValue<bool>(_value));

		/// @wproperty{EditBox, InvertSelected, bool} При выделении цвета инвертируются.
		else if (_key == "InvertSelected")
			setInvertSelected(utility::parseValue<bool>(_value));

		else
		{
			Base::setPropertyOverride(_key, _value);
			return;
		}

		eventChangeProperty(this, _key, _value);
	}

	size_t EditBox::getTextCursor() const
	{
		return mCursorPosition;
	}

	size_t EditBox::getTextLength() const
	{
		return mTextLength;
	}

	void EditBox::setOverflowToTheLeft(bool _value)
	{
		mOverflowToTheLeft = _value;
	}

	bool EditBox::getOverflowToTheLeft() const
	{
		return mOverflowToTheLeft;
	}

	void EditBox::setMaxTextLength(size_t _value)
	{
		mMaxTextLength = _value;
	}

	size_t EditBox::getMaxTextLength() const
	{
		return mMaxTextLength;
	}

	bool EditBox::getEditReadOnly() const
	{
		return mModeReadOnly;
	}

	bool EditBox::getEditPassword() const
	{
		return mModePassword;
	}

	bool EditBox::getEditMultiLine() const
	{
		return mModeMultiline;
	}

	bool EditBox::getEditStatic() const
	{
		return mModeStatic;
	}

	Char EditBox::getPasswordChar() const
	{
		return mCharPassword;
	}

	bool EditBox::getEditWordWrap() const
	{
		return mModeWordWrap;
	}

	void EditBox::setTabPrinting(bool _value)
	{
		mTabPrinting = _value;
	}

	bool EditBox::getTabPrinting() const
	{
		return mTabPrinting;
	}

	void EditBox::setPosition(int _left, int _top)
	{
		setPosition(IntPoint(_left, _top));
	}

	void EditBox::setSize(int _width, int _height)
	{
		setSize(IntSize(_width, _height));
	}

	void EditBox::setCoord(int _left, int _top, int _width, int _height)
	{
		setCoord(IntCoord(_left, _top, _width, _height));
	}

	bool EditBox::isVisibleVScroll() const
	{
		return mVisibleVScroll;
	}

	bool EditBox::isVisibleHScroll() const
	{
		return mVisibleHScroll;
	}

	void EditBox::commandResetRedo()
	{
		mVectorRedoChangeInfo.clear();
	}

	void EditBox::commandResetHistory()
	{
		mVectorRedoChangeInfo.clear();
		mVectorUndoChangeInfo.clear();
	}

	void EditBox::setTextShadowColour(const Colour& _value)
	{
		Base::setTextShadowColour(_value);

		if (mClientText != nullptr)
			mClientText->setShadowColour(_value);
	}

	void EditBox::setTextShadow(bool _value)
	{
		Base::setTextShadow(_value);

		if (mClientText != nullptr)
			mClientText->setShadow(_value);
	}

} // namespace MyGUI
