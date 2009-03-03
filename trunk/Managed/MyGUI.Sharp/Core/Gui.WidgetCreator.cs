using System;
using System.Runtime.InteropServices;

namespace MyGUI.Sharp
{

    public partial class Gui
    {

        #region WrapperCreator

        class WrapperCreator
        {
            [return: MarshalAs(UnmanagedType.Interface)]
            public delegate BaseWidget HandleDelegate([MarshalAs(UnmanagedType.Interface)]BaseWidget _parent, [MarshalAs(UnmanagedType.LPStr)]string _type, IntPtr _widget);

            [DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
            private static extern void ExportGui_SetCreatorWrapps(HandleDelegate _delegate);

            static HandleDelegate mDelegate;

            public WrapperCreator()
            {
                mDelegate = new HandleDelegate(OnRequest);
                ExportGui_SetCreatorWrapps(mDelegate);
            }

            BaseWidget OnRequest(BaseWidget _parent, string _type, IntPtr _widget)
            {
                return WrapWidget(_type, _parent, _widget);
            }
        }

        static WrapperCreator mWrapperCreator = new WrapperCreator();

        #endregion

        #region GetNativeByWrapper

        class GetNativeByWrapper
        {
            public delegate IntPtr HandleDelegate([MarshalAs(UnmanagedType.Interface)]BaseWidget _wrapper);

            [DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
            private static extern void ExportGui_SetGetNativeByWrapper(HandleDelegate _delegate);

            static HandleDelegate mDelegate;

            public GetNativeByWrapper()
            {
                mDelegate = new HandleDelegate(OnRequest);
                ExportGui_SetGetNativeByWrapper(mDelegate);
            }

            IntPtr OnRequest(BaseWidget _wrapper)
            {
                return _wrapper == null ? IntPtr.Zero : _wrapper.GetNative();
            }
        }

        static GetNativeByWrapper mGetNativeByWrapper = new GetNativeByWrapper();

        #endregion

        #region WidgetCreator

        static BaseWidget WrapWidget(string _type, BaseWidget _parent, IntPtr _widget)
        {
            if (_type == "Button")
                return new Button(_parent, _widget);
            else if (_type == "Canvas")
                return new Canvas(_parent, _widget);
            else if (_type == "ComboBox")
                return new ComboBox(_parent, _widget);
            else if (_type == "DDContainer")
                return new DDContainer(_parent, _widget);
            else if (_type == "Edit")
                return new EditBox(_parent, _widget);
            else if (_type == "HScroll")
                return new HScrollBar(_parent, _widget);
            else if (_type == "ItemBox")
                return new ItemBox(_parent, _widget);
            else if (_type == "List")
                return new ListBox(_parent, _widget);
            else if (_type == "MenuBar")
                return new MenuBar(_parent, _widget);
            else if (_type == "MenuCtrl")
                return new MenuCtrl(_parent, _widget);
            else if (_type == "Message")
                return new MessageBox(_parent, _widget);
            else if (_type == "MultiList")
                return new MultiListBox(_parent, _widget);
            else if (_type == "PopupMenu")
                return new PopupMenu(_parent, _widget);
            else if (_type == "Progress")
                return new ProgressBar(_parent, _widget);
            else if (_type == "RenderBox")
                return new RenderBox(_parent, _widget);
            else if (_type == "ScrollView")
                return new ScrollView(_parent, _widget);
            else if (_type == "StaticImage")
                return new StaticImage(_parent, _widget);
            else if (_type == "StaticText")
                return new StaticText(_parent, _widget);
            else if (_type == "Tab")
                return new TabBar(_parent, _widget);
            else if (_type == "VScroll")
                return new VScrollBar(_parent, _widget);
            else if (_type == "Widget")
                return new Widget(_parent, _widget);
            else if (_type == "Window")
                return new Window(_parent, _widget);
            else if (_type == "MenuItem")
                return new MenuItem(_parent, _widget);
            else if (_type == "TabItem")
                return new TabItem(_parent, _widget);

            return null;
        }

        #endregion

    }

}
