using System;
using System.Runtime.InteropServices;
using System.Collections.Generic;
using System.Xml;
using System.IO;

namespace MyGUI.Sharp
{
    public partial class Gui
    {

        #region Instance

        private static Gui mInstance = new Gui();

        public static Gui Instance
        {
            get { return mInstance; }
        }

        #endregion

        #region KeyFocus

        [DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        private static extern bool ExportGui_GetKeyFocus();

        public bool KeyFocus
        {
            get { return ExportGui_GetKeyFocus(); }
        }

        #endregion

        #region KeyFocusWidget

        [DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.Interface)]
        private static extern Widget ExportGui_GetKeyFocusWidget();
        [DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void ExportGui_SetKeyFocusWidget(IntPtr _widget);

        public Widget KeyFocusWidget
        {
            get { return ExportGui_GetKeyFocusWidget(); }
            set { ExportGui_SetKeyFocusWidget(value == null ? IntPtr.Zero : value.GetNative()); }
        }

        #endregion

        #region MouseFocus

        [DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
        private static extern bool ExportGui_GetMouseFocus();

        public bool MouseFocus
        {
            get { return ExportGui_GetMouseFocus(); }
        }

        #endregion

        #region MouseFocusWidget

        [DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.Interface)]
        private static extern Widget ExportGui_GetMouseFocusWidget();

        public Widget MouseFocusWidget
        {
            get { return ExportGui_GetMouseFocusWidget(); }
        }

        #endregion

        #region ResetKeyFocus

        [DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void ExportGui_ResetKeyFocusWidget( );

        public void ResetKeyFocus()
        {
            ExportGui_ResetKeyFocusWidget();
        }

        #endregion

        #region AddWidgetModal

        [DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void ExportGui_AddWidgetModal(IntPtr _widget);

        public void AddWidgetModal(Widget _widget)
        {
            ExportGui_AddWidgetModal(_widget.GetNative());
        }

        #endregion

        #region RemoveWidgetModal

        [DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void ExportGui_RemoveWidgetModal(IntPtr _widget);

        public void RemoveWidgetModal(Widget _widget)
        {
            ExportGui_RemoveWidgetModal(_widget.GetNative());
        }

        #endregion

        #region AttachToLayer

        [DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void ExportGui_AttachToLayer(
            [MarshalAs(UnmanagedType.LPStr)]  string _layer,
            IntPtr _widget);

        public void AttachToLayer(string _layer, Widget _widget)
        {
            ExportGui_AttachToLayer(_layer, _widget.GetNative());
        }

        #endregion

        #region CreateWidget

        public T CreateWidget<T>(string _skin, IntCoord _coord, Align _align, string _layer) where T : class
        {
            T type = System.Activator.CreateInstance<T>();
            BaseWidget widget = type as BaseWidget;
            widget.CreateWidget(IntPtr.Zero, WidgetStyle.Overlapped, _skin, _coord, _align, _layer, "");
            return type;
        }

        public T CreateWidget<T>(string _skin, IntCoord _coord, Align _align, string _layer, string _name) where T : class
        {
            T type = System.Activator.CreateInstance<T>();
            BaseWidget widget = type as BaseWidget;
            widget.CreateWidget(IntPtr.Zero, WidgetStyle.Overlapped, _skin, _coord, _align, _layer, _name);
            return type;
        }

        #endregion

        #region LoadLayout

        public List<Widget> LoadLayout(string _file)
        {
            return LoadLayout(_file, null, "");
        }

        public List<Widget> LoadLayout(string _file, string _prefix)
        {
            return LoadLayout(_file, null, _prefix);
        }

        public List<Widget> LoadLayout(string _file, Widget _parent)
        {
            return LoadLayout(_file, _parent, "");
        }

        [DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern IntPtr ExportGui_GetPath(
            [MarshalAs(UnmanagedType.LPStr)]  string _filename );

        public List<Widget> LoadLayout(string _file, Widget _parent, string _prefix)
        {
            List<Widget> widgets = new List<Widget>();
            string filename = Marshal.PtrToStringAnsi( ExportGui_GetPath(_file) );
            if (filename == "")
                return widgets;

            XmlDocument mfDocument = new XmlDocument();
            try
            {
                mfDocument.Load(filename);
            }
            catch (FileNotFoundException)
            {
                return widgets;
            }

            foreach (XmlNode node in mfDocument)
            {
                if ("MyGUI" != node.Name) continue;
                if ("Layout" != node.Attributes["type"].Value) continue;

                foreach (XmlNode widget_node in node)
                {
                    if ("Widget" != widget_node.Name) continue;
                    ParseWidget(widgets, widget_node, _parent, null, _prefix);
                }
            }

            return widgets;
        }

        private void ParseWidget(List<Widget> _widgets, XmlNode _node, Widget _parent, Widget _root, string _prefix)
        {
            string style = "", type = "", skin = "", name = "", layer = "", align = "", position = "";

            foreach (XmlAttribute attribute in _node.Attributes)
            {
                switch (attribute.Name)
                {
                    case "style": style = attribute.Value; break;
                    case "type": type = attribute.Value; break;
                    case "skin": skin = attribute.Value; break;
                    case "name": name = attribute.Value; break;
                    case "layer": layer = attribute.Value; break;
                    case "align": align = attribute.Value; break;
                    case "position": position = attribute.Value; break;
                }

                if (type == "Sheet") type = "TabItem";
            }

            if ((0 == type.Length) || (0 == skin.Length))
            {
                return;
            }

            if ((0 != name.Length) && (0 != _prefix.Length)) name = _prefix + name;

            string[] coord = position.Split();

            WidgetStyle wstyle = WidgetStyle.Overlapped;
            if (_parent != null)
            {
                layer = "";
                wstyle = (style != string.Empty) ? (WidgetStyle)Enum.Parse(typeof(WidgetStyle), style) : WidgetStyle.Child;
            }

            Align walign = Align.Default;
            if (align != string.Empty)
            {
                walign = Align.Center;
                string[] mass = align.Split();
                foreach (string item in mass)
                    walign |= (Align)Enum.Parse(typeof(Align), item);
            }

            Widget widget = (Widget)mMapCreator[type](
                _parent == null ? IntPtr.Zero : _parent.GetNative(),
                wstyle,
                skin,
                ((coord.Length == 4) ? new IntCoord(int.Parse(coord[0]), int.Parse(coord[1]), int.Parse(coord[2]), int.Parse(coord[3])) : new IntCoord()),
                walign,
                layer,
                name);

            if (null == _root)
            {
                _root = widget;
                _widgets.Add(_root);
            }

            foreach (XmlNode node in _node)
            {
                if ("Widget" == node.Name)
                {
                    ParseWidget(_widgets, node, widget, _root, _prefix);
                }
                else if ("Property" == node.Name)
                {
                    string key = "", value = "";
                    foreach (XmlAttribute attribute in node.Attributes)
                    {
                        if ("key" == attribute.Name) key = attribute.Value;
                        else if ("value" == attribute.Name) value = attribute.Value;
                    }
                    if ((0 == key.Length) || (0 == value.Length)) continue;

                    SetProperty(widget, key, value);
                }
                else if ("UserString" == node.Name)
                {
                    string key = "", value = "";
                    foreach (XmlAttribute attribute in node.Attributes)
                    {
                        if ("key" == attribute.Name) key = attribute.Value;
                        else if ("value" == attribute.Name) value = attribute.Value;
                    }
                    if ((0 == key.Length) || (0 == value.Length)) continue;
                    widget.SetUserString(key, value);

                    if (EventParserUserData != null) EventParserUserData(widget, key, value);
                }
            }

        }

        [DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void ExportGui_SetProperty(
            IntPtr _widget,
            [MarshalAs(UnmanagedType.LPStr)]  string _key ,
            [MarshalAs(UnmanagedType.LPStr)]  string _value );

        void SetProperty(Widget _widget, string _key, string _value)
        {
            ExportGui_SetProperty(_widget.GetNative(), _key, _value);
        }

        #endregion

        #region LoadResource

        [DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void ExportGui_LoadResource(
            [MarshalAs(UnmanagedType.LPStr)]  string _source
            );

        public void LoadResource(string _source)
        {
            ExportGui_LoadResource(_source);
        }

        #endregion

        #region UpWidget

        [DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        private static extern void ExportGui_UpWidget(IntPtr _widget);

        public void UpWidget(Widget _widget)
        {
            ExportGui_UpWidget(_widget.GetNative());
        }

        #endregion

        #region EventParserUserData

        public delegate void HandleParserUserData(Widget _widget, string _key, string _value);
        public event Gui.HandleParserUserData EventParserUserData;

        #endregion

        #region WrapperCreator

        delegate BaseWidget HandleWrapWidget(BaseWidget _parent, IntPtr _widget);
        static Dictionary<string, HandleWrapWidget> mMapWrapper = new Dictionary<string, HandleWrapWidget>();

        delegate BaseWidget HandleCreateWidget(IntPtr _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name);
        static Dictionary<string, HandleCreateWidget> mMapCreator = new Dictionary<string, HandleCreateWidget>();

        class WrapperCreator
        {
            [return: MarshalAs(UnmanagedType.Interface)]
            public delegate BaseWidget HandleDelegate([MarshalAs(UnmanagedType.LPStr)]string _type, [MarshalAs(UnmanagedType.Interface)]BaseWidget _parent, IntPtr _widget);

            [DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
            private static extern void ExportGui_SetCreatorWrapps(HandleDelegate _delegate);

            static HandleDelegate mDelegate;

            public WrapperCreator()
            {
                mDelegate = new HandleDelegate(OnRequest);
                ExportGui_SetCreatorWrapps(mDelegate);

                InitialiseWidgetCreator();
            }

            BaseWidget OnRequest(string _type, BaseWidget _parent, IntPtr _widget)
            {
                return mMapWrapper[_type](_parent, _widget);
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

    }
}
