using System;
using System.Collections.Generic;
using System.IO;
using System.Runtime.InteropServices;
using System.Xml;

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

		public Gui()
		{
			//mDelegateNativeByWrapper = new HandleDelegateNativeByWrapper(OnRequestNativeByWrapper);
			//ExportGui_SetGetNativeByWrapper(mDelegateNativeByWrapper);

			//mDelegateWrapperCreator = new HandleDelegateWrapperCreator(OnRequestWrapperCreator);
			//ExportGui_SetCreatorWrapps(mDelegateWrapperCreator);

			InitialiseWidgetCreator();
		}

		#endregion

		#region CreateWidget

		public T CreateWidget<T>(string _skin, IntCoord _coord, Align _align, string _layer) where T : class
		{
			T type = System.Activator.CreateInstance<T>();
			BaseWidget widget = type as BaseWidget;
			widget.CreateWidgetImpl(null, WidgetStyle.Overlapped, _skin, _coord, _align, _layer, "");
			return type;
		}

		public T CreateWidget<T>(string _skin, IntCoord _coord, Align _align, string _layer, string _name) where T : class
		{
			T type = System.Activator.CreateInstance<T>();
			BaseWidget widget = type as BaseWidget;
			widget.CreateWidgetImpl(null, WidgetStyle.Overlapped, _skin, _coord, _align, _layer, _name);
			return type;
		}

		public Widget CreateWidgetT(Widget _parent, WidgetStyle _style, string _type, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
		{
			return (Widget)mMapCreator[_type](_parent, _style, _skin, _coord, _align, _layer, _name);
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

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern IntPtr ExportGui_GetPath(
			[MarshalAs(UnmanagedType.LPStr)] string _filename);

		public List<Widget> LoadLayout(string _file, Widget _parent, string _prefix)
		{
			List<Widget> widgets = new List<Widget>();
			string filename = Marshal.PtrToStringAnsi(ExportGui_GetPath(_file));
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
				if ("MyGUI" != node.Name)
					continue;
				if ("Layout" != node.Attributes["type"].Value)
					continue;

				foreach (XmlNode widget_node in node)
				{
					if ("Widget" != widget_node.Name)
						continue;
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

				if (type == "Sheet")
					type = "TabItem";
			}

			if ((0 == type.Length) || (0 == skin.Length))
			{
				return;
			}

			if ((0 != name.Length) && (0 != _prefix.Length))
				name = _prefix + name;

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
				_parent,
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
						if ("key" == attribute.Name)
							key = attribute.Value;
						else if ("value" == attribute.Name)
							value = attribute.Value;
					}
					if ((0 == key.Length) || (0 == value.Length))
						continue;

					SetProperty(widget, key, value);
				}
				else if ("UserString" == node.Name)
				{
					string key = "", value = "";
					foreach (XmlAttribute attribute in node.Attributes)
					{
						if ("key" == attribute.Name)
							key = attribute.Value;
						else if ("value" == attribute.Name)
							value = attribute.Value;
					}
					if ((0 == key.Length) || (0 == value.Length))
						continue;
					widget.SetUserString(key, value);

					if (EventParserUserData != null)
						EventParserUserData(widget, key, value);
				}
			}

		}

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportGui_SetProperty(
			IntPtr _widget,
			[MarshalAs(UnmanagedType.LPStr)]  string _key,
			[MarshalAs(UnmanagedType.LPWStr)]  string _value);

		public static void SetProperty(Widget _widget, string _key, string _value)
		{
			ExportGui_SetProperty(_widget.Native, _key, _value);
		}

		#endregion

		#region LoadResource

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportGui_LoadResource(
			[MarshalAs(UnmanagedType.LPStr)]  string _source
			);

		public void LoadResource(string _source)
		{
			ExportGui_LoadResource(_source);
		}

		#endregion

		#region EventParserUserData

		public delegate void HandleParserUserData(Widget _widget, string _key, string _value);
		public event Gui.HandleParserUserData EventParserUserData;

		#endregion

		#region WrapperCreator

		private delegate BaseWidget HandleWrapWidget(BaseWidget _parent, IntPtr _widget);
		private Dictionary<string, HandleWrapWidget> mMapWrapper = new Dictionary<string, HandleWrapWidget>();

		private delegate BaseWidget HandleCreateWidget(BaseWidget _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name);
		private Dictionary<string, HandleCreateWidget> mMapCreator = new Dictionary<string, HandleCreateWidget>();

		//[UnmanagedFunctionPointer(CallingConvention.StdCall)]
		//public delegate IntPtr HandleDelegateWrapperCreator([MarshalAs(UnmanagedType.LPStr)]string _type, [MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]BaseWidget _parent, IntPtr _widget);

		//[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		//private static extern void ExportGui_SetCreatorWrapps(HandleDelegateWrapperCreator _delegate);

		//HandleDelegateWrapperCreator mDelegateWrapperCreator;

		/*BaseWidget OnRequestWrapperCreator(string _type, BaseWidget _parent, IntPtr _widget)
		{
			return mMapWrapper[_type](_parent, _widget);
		}*/

		internal static BaseWidget CreateWrapper(string _type, BaseWidget _parent, IntPtr _widget)
		{
			return Gui.Instance.mMapWrapper[_type](_parent, _widget);
		}

		#endregion

		#region GetNativeByWrapper

		//[UnmanagedFunctionPointer(CallingConvention.StdCall)]
		//public delegate IntPtr HandleDelegateNativeByWrapper([MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]BaseWidget _wrapper);

		//[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		//private static extern void ExportGui_SetGetNativeByWrapper(HandleDelegateNativeByWrapper _delegate);

		//HandleDelegateNativeByWrapper mDelegateNativeByWrapper;

		/*private IntPtr OnRequestNativeByWrapper(BaseWidget _wrapper)
		{
			return _wrapper != null ? _wrapper.Native : IntPtr.Zero;
		}*/

		#endregion

		#region Log

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportGui_Log(
			[MarshalAs(UnmanagedType.LPStr)] string _section,
			[MarshalAs(UnmanagedType.I4)] LogLevel _level,
			[MarshalAs(UnmanagedType.LPStr)] string _message
			);

		public void Log(string _section, LogLevel _level, string _message)
		{
			ExportGui_Log(_section, _level, _message);
		}

		#endregion

		#region ExportTest

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportGui_ExportTest(
			);

		public void ExportTest()
		{
			ExportGui_ExportTest();
		}

		#endregion
	}
}
