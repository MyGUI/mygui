using System;
using System.Collections.Generic;
using System.Runtime.InteropServices;

namespace MyGUI.Sharp
{
	public abstract class BaseWidget :
		IDisposable
	{
		#region Export

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern IntPtr ExportGui_CreateWidget(
			IntPtr _parent,
			WidgetStyle _style,
			[MarshalAs(UnmanagedType.LPStr)]string _type,
			[MarshalAs(UnmanagedType.LPStr)]string _skin,
			[In] ref IntCoord _coord,
			Align _align,
			[MarshalAs(UnmanagedType.LPStr)]string _layer,
			[MarshalAs(UnmanagedType.LPStr)]string _name);

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportGui_DestroyWidget(IntPtr _widget);

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportGui_WrapWidget(IntPtr _widget);

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportGui_UnwrapWidget(IntPtr _widget);

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern IntPtr ExportGui_GetManagedParent(IntPtr _widget);

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		[return: MarshalAs(UnmanagedType.LPStr)]
		private static extern string ExportGui_GetWidgetType(IntPtr _widget);

		#endregion

		#region BaseWidget

		internal void WrapWidget(BaseWidget _parent, IntPtr _native)
		{
			mParent = _parent;
			mNative = _native;

			ExportGui_WrapWidget(mNative);
			mWidgets.Add(mNative, this);
			mIsWrap = true;

			if (mParent != null)
				mParent.mChilds.Add(this);
			else
				mRoots.Add(this);
		}

		internal void CreateWidgetImpl(BaseWidget _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
		{
			mParent = _parent;
			IntPtr parent = _parent != null ? _parent.Native : IntPtr.Zero;
			mNative = ExportGui_CreateWidget(parent, _style, GetWidgetType(), _skin, ref _coord, _align, _layer, _name);

			ExportGui_WrapWidget(mNative);
			mWidgets.Add(mNative, this);
			mIsWrap = false;

			if (mParent != null)
				mParent.mChilds.Add(this);
			else
				mRoots.Add(this);
		}

		public object UserData
		{
			get { return mUserData; }
			set { mUserData = value; }
		}

		private void DestroyChilds()
		{
			while (mChilds.Count > 0)
			{
				BaseWidget child = mChilds[0];
				child.Dispose();
				child = null;
			}
		}

		protected abstract string GetWidgetType();

		public static BaseWidget GetByNative(IntPtr _value)
		{
			if (_value == IntPtr.Zero)
				return null;

			BaseWidget result;
			if (!mWidgets.TryGetValue(_value, out result))
			{
				string widgetType = ExportGui_GetWidgetType(_value);
				IntPtr parentPtr = ExportGui_GetManagedParent(_value);
				BaseWidget parent;
				mWidgets.TryGetValue(parentPtr, out parent);

				return Gui.CreateWrapper(widgetType, parent, _value);
			}
			return result;
		}

		#endregion

		#region CreateWidget

		public T CreateWidget<T>(string _skin, IntCoord _coord, Align _align) where T : class
		{
			T type = System.Activator.CreateInstance<T>();
			BaseWidget widget = type as BaseWidget;
			widget.CreateWidgetImpl(this, WidgetStyle.Child, _skin, _coord, _align, "", "");
			return type;
		}

		public T CreateWidget<T>(string _skin, IntCoord _coord, Align _align, string _name) where T : class
		{
			T type = System.Activator.CreateInstance<T>();
			BaseWidget widget = type as BaseWidget;
			widget.CreateWidgetImpl(this, WidgetStyle.Child, _skin, _coord, _align, "", _name);
			return type;
		}

		public T CreateWidget<T>(WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer) where T : class
		{
			T type = System.Activator.CreateInstance<T>();
			BaseWidget widget = type as BaseWidget;
			widget.CreateWidgetImpl(this, _style, _skin, _coord, _align, _layer, "");
			return type;
		}

		public T CreateWidget<T>(WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name) where T : class
		{
			T type = System.Activator.CreateInstance<T>();
			BaseWidget widget = type as BaseWidget;
			widget.CreateWidgetImpl(this, _style, _skin, _coord, _align, _layer, _name);
			return type;
		}

		#endregion

		#region IDisposable Members

		public void Dispose()
		{
			if (mNative != IntPtr.Zero)
			{
				DestroyChilds();
				if (mParent != null)
				{
					mParent.mChilds.Remove(this);
					mParent = null;
				}
				else
				{
					mRoots.Remove(this);
				}

				if (!mIsWrap)
					ExportGui_DestroyWidget(mNative);

				ExportGui_UnwrapWidget(mNative);
				mWidgets.Remove(mNative);

				mNative = IntPtr.Zero;
			}
		}

		#endregion

		#region Properties

		public IntPtr Native
		{
			get { return mNative; }
		}

		#endregion

		#region Fields

		private IntPtr mNative;
		private bool mIsWrap = true;
		private BaseWidget mParent;
		private List<BaseWidget> mChilds = new List<BaseWidget>();
		private object mUserData;

		private static List<BaseWidget> mRoots = new List<BaseWidget>();
		private static Dictionary<IntPtr, BaseWidget> mWidgets = new Dictionary<IntPtr, BaseWidget>();

		#endregion
	}
}
