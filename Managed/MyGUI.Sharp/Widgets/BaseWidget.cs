using System;
using System.Collections.Generic;
using System.Text;
using System.Runtime.InteropServices;

namespace MyGUI.Sharp
{

    public abstract class BaseWidget : IDisposable
    {

        #region Export

        [DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr ExportGui_CreateWidget(
            [MarshalAs(UnmanagedType.Interface)]BaseWidget _wrapper,
            IntPtr _parent,
            WidgetStyle _style,
            [MarshalAs(UnmanagedType.LPStr)]string _type,
            [MarshalAs(UnmanagedType.LPStr)]string _skin,
            [InAttribute] ref IntCoord _coord,
            Align _align,
            [MarshalAs(UnmanagedType.LPStr)]string _layer,
            [MarshalAs(UnmanagedType.LPStr)]string _name);

        [DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void ExportGui_DestroyWidget(IntPtr _widget);

        [DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void ExportGui_WrapWidget([MarshalAs(UnmanagedType.Interface)]BaseWidget _wrapper, IntPtr _widget);

        [DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void ExportGui_UnwrapWidget(IntPtr _widget);

        #endregion

        #region Fields

        protected IntPtr mNative;
        bool mIsWrap;
        BaseWidget mParent;
        List<BaseWidget> mChilds = new List<BaseWidget>();
        object mUserData;
        static List<BaseWidget> mRoots = new List<BaseWidget>();

        #endregion 

        #region WrapperCreator

        class WrapperCreator
        {
            [return: MarshalAs(UnmanagedType.Interface)]
            public delegate BaseWidget HandleWrappWidget([MarshalAs(UnmanagedType.Interface)]BaseWidget _parent, [MarshalAs(UnmanagedType.LPStr)]string _type, IntPtr _widget);

            [DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
            private static extern void ExportGui_SetCreatorWrapps(HandleWrappWidget _delegate);

            static HandleWrappWidget mWrapperCreator;

            public WrapperCreator()
            {
                mWrapperCreator = new HandleWrappWidget(RequestCreateWrapper);
                ExportGui_SetCreatorWrapps(mWrapperCreator);
            }

            BaseWidget RequestCreateWrapper(BaseWidget _parent, string _type, IntPtr _widget)
            {
                return MyGUI.Sharp.Widgets.WidgetCreator.CreateWidget(_parent, _type, _widget);
            }
        }

        static WrapperCreator mWrapper = new WrapperCreator();

        #endregion

        #region BaseWidget

        public BaseWidget()
        {
            mNative = IntPtr.Zero;
            mIsWrap = true;
        }

        public BaseWidget(BaseWidget _parent, IntPtr _native)
        {
            if (_native != null)
            {
                mNative = _native;
                mParent = _parent;
                if (mParent == null) mRoots.Add(this);
                else mParent.mChilds.Add(this);
                ExportGui_WrapWidget(this, mNative);
                mIsWrap = true;
            }
        }

        public BaseWidget(IntPtr _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
        {
            mNative = ExportGui_CreateWidget(this, _parent, _style, GetWidgetType(), _skin, ref _coord, _align, _layer, _name);
			mIsWrap = false;
        }

        public object UserData
        {
            get { return mUserData; }
            set { mUserData = value; }
        }

        void DestroyChilds()
		{
			while (mChilds.Count > 0)
            {
				BaseWidget child = mChilds[0];
				child.Dispose();
				child = null;
			}
		}

        protected abstract string GetWidgetType();

        internal IntPtr GetNative() { return mNative; }

        #endregion

        #region IDisposable Members

        public void Dispose()
        {
            if (mNative != IntPtr.Zero)
            {
                DestroyChilds();
                if (mParent == null)
                {
                    mRoots.Remove(this);
                }
                else
                {
					mParent.mChilds.Remove(this);
					mParent = null;
                }
				if ( ! mIsWrap )
                    ExportGui_DestroyWidget(mNative);
                else
                    ExportGui_UnwrapWidget(mNative);
                mNative = IntPtr.Zero;
            }
        }

        #endregion

    }

}
