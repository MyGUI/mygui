using System.Collections.Generic;
using System.Diagnostics;

namespace MyGUI
{
	//* UserData is parent of Widget class. Used to store any user data and strings inside widget 
	public partial class UserData
	{
		//* Set user string 
		public void setUserString(string _key, string _value)
		{
			mMapUserString.Add(_key, _value);
		}

		//* Get user string or "" if not found 
		public string getUserString(string _key)
		{
			string result = null;
			if (mMapUserString.TryGetValue(_key, out result))
				return result;
			return "";
		}

		//* Get map of all user strings 
		public Dictionary<string, string> getUserStrings()
		{
			return mMapUserString;
		}

		//* Delete user string 
		public bool clearUserString(string _key)
		{
			if (mMapUserString.ContainsKey(_key))
			{
				mMapUserString.Remove(_key);
				return true;
			}
			return false;
		}

		//* Return true if user string with such key exist 
		public bool isUserString(string _key)
		{
			return mMapUserString.ContainsKey(_key);
		}

		//* Delete all user strings 
		public void clearUserStrings()
		{
			mMapUserString.Clear();
		}

		//* Set any user data to store inside widget 
		public void setUserData(object _data)
		{
			mUserData = _data;
		}

		//* Get user data and cast it to ValueType 
		public object getUserData()
		{
			return mUserData;
		}

		public ValueType getUserData<ValueType>(bool _throw) where ValueType : class
		{
			if (mUserData is ValueType)
				return (ValueType)mUserData;
			Trace.Assert(_throw);
			return null;
		}

		//internal:
		public void _setInternalData(object _data)
		{
			mInternalData = _data;
		}

		public object _getInternalData()
		{
			return mInternalData;
		}

		public ValueType _getInternalData<ValueType>(bool _throw) where ValueType : class
		{
			if (mInternalData is ValueType)
				return (ValueType)mInternalData;
			Trace.Assert(_throw);
			return null;
		}

		// пользовательские данные
		private Dictionary<string, string> mMapUserString = new Dictionary<string, string>();
		private object mUserData;

		// для внутренниего использования
		private object mInternalData;
	}
}



