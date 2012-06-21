
namespace MyGUI
{
	public class CroppedRectangle
	{
		public CroppedRectangle()
		{
			mIsMargin = false;
			mCroppedParent = null;
		}

		//* Get parent ICroppedRectangle 
		public CroppedRectangle getCroppedParent()
		{
			return mCroppedParent;
		}

		//* Set position 
		public virtual void setPosition(IntPoint _value)
		{
			mCoord.left = _value.left;
			mCoord.top = _value.top;
		}

		//* Set size 
		public virtual void setSize(IntSize _value)
		{
			mCoord.width = _value.width;
			mCoord.height = _value.height;
		}

		//* Set coordinates (position and size) 
		public virtual void setCoord(IntCoord _value)
		{
			mCoord = _value;
		}

		//* Get position 
		public IntPoint getPosition()
		{
			return mCoord.point();
		}

		//* Get size 
		public IntSize getSize()
		{
			return mCoord.size();
		}

		// Get coordinates (position and size) 
		public IntCoord getCoord()
		{
			return mCoord;
		}

		// Get position in screen coordinates 
		public IntPoint getAbsolutePosition()
		{
			return mAbsolutePosition;
		}

		// Get rectangle in screen coordinates 
		public IntRect getAbsoluteRect()
		{
			return new IntRect(mAbsolutePosition.left, mAbsolutePosition.top, mAbsolutePosition.left + mCoord.width, mAbsolutePosition.top + mCoord.height);
		}

		// Get coordinate in screen coordinates 
		public IntCoord getAbsoluteCoord()
		{
			return new IntCoord(mAbsolutePosition.left, mAbsolutePosition.top, mCoord.width, mCoord.height);
		}

		// Get X in screen coordinates 
		public int getAbsoluteLeft()
		{
			return mAbsolutePosition.left;
		}

		// Get Y in screen coordinates 
		public int getAbsoluteTop()
		{
			return mAbsolutePosition.top;
		}

		// Get left x-coordinate 
		public int getLeft()
		{
			return mCoord.left;
		}

		// Get right x-coordinate 
		public int getRight()
		{
			return mCoord.right();
		}

		// Get top y-coordinate 
		public int getTop()
		{
			return mCoord.top;
		}

		// Get bottom y-coordinate 
		public int getBottom()
		{
			return mCoord.bottom();
		}

		// Get width 
		public int getWidth()
		{
			return mCoord.width;
		}

		// Get height 
		public int getHeight()
		{
			return mCoord.height;
		}

		//internal:
		// True if rectangle is cropped by parent rectangle 
		public bool _isMargin()
		{
			return mIsMargin;
		}

		public int _getViewLeft()
		{
			return mCoord.left + mMargin.left;
		}

		public int _getViewRight()
		{
			return mCoord.right() - mMargin.right;
		}

		public int _getViewTop()
		{
			return mCoord.top + mMargin.top;
		}

		public int _getViewBottom()
		{
			return mCoord.bottom() - mMargin.bottom;
		}

		public int _getViewWidth()
		{
			return mCoord.width - mMargin.left - mMargin.right;
		}

		public int _getViewHeight()
		{
			return mCoord.height - mMargin.top - mMargin.bottom;
		}

		public void _setCroppedParent(CroppedRectangle _parent)
		{
			mCroppedParent = _parent;
		}

		public IntRect _getMargin()
		{
			return mMargin;
		}

		public int _getMarginLeft()
		{
			return mMargin.left;
		}

		public int _getMarginRight()
		{
			return mMargin.right;
		}

		public int _getMarginTop()
		{
			return mMargin.top;
		}

		public int _getMarginBottom()
		{
			return mMargin.bottom;
		}

		protected bool _checkMargin()
		{
			bool margin = false;
			//вылезли ли налево
			if (getLeft() < mCroppedParent.mMargin.left)
			{
				mMargin.left = mCroppedParent.mMargin.left - getLeft();
				margin = true;
			}
			else
			{
				mMargin.left = 0;
			}

			//вылезли ли направо
			if (getRight() > mCroppedParent.getWidth() - mCroppedParent.mMargin.right)
			{
				mMargin.right = getRight() - (mCroppedParent.getWidth() - mCroppedParent.mMargin.right);
				margin = true;
			}
			else
			{
				mMargin.right = 0;
			}

			//вылезли ли вверх
			if (getTop() < mCroppedParent.mMargin.top)
			{
				mMargin.top = mCroppedParent.mMargin.top - getTop();
				margin = true;
			}
			else
			{
				mMargin.top = 0;
			}

			//вылезли ли вниз
			if (getBottom() > mCroppedParent.getHeight() - mCroppedParent.mMargin.bottom)
			{
				mMargin.bottom = getBottom() - (mCroppedParent.getHeight() - mCroppedParent.mMargin.bottom);
				margin = true;
			}
			else
			{
				mMargin.bottom = 0;
			}

			return margin;
		}

		protected bool _checkOutside() // проверка на полный выход за границу
		{
			// совсем уехали вниз -  совсем уехали вверх -  совсем уехали направо -  совсем уехали налево
			return ((getRight() < mCroppedParent.mMargin.left) || (getLeft() > mCroppedParent.getWidth() - mCroppedParent.mMargin.right) || (getBottom() < mCroppedParent.mMargin.top) || (getTop() > mCroppedParent.getHeight() - mCroppedParent.mMargin.bottom));
		}

		protected IntRect mMargin; // перекрытие
		protected IntCoord mCoord; // координаты
		protected IntPoint mAbsolutePosition; // обсолютные координаты

		protected bool mIsMargin;
		protected CroppedRectangle mCroppedParent;
	}

} // namespace MyGUI

