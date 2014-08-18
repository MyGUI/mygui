/*!
	@file
	@author		Generate utility by Albert Semenov
	@date		01/2009
	@module
*/

using System;
using System.Runtime.InteropServices;

namespace MyGUI.Sharp
{
    public partial class Widget :
		BaseWidget
    {
        #region Widget

        protected override string GetWidgetType() { return "Widget"; }

        internal static BaseWidget RequestWrapWidget(BaseWidget _parent, IntPtr _widget)
        {
			Widget widget = new Widget();
			widget.WrapWidget(_parent, _widget);
            return widget;
        }

        internal static BaseWidget RequestCreateWidget(BaseWidget _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
        {
			Widget widget = new Widget();
			widget.CreateWidgetImpl(_parent, _style, _skin, _coord, _align, _layer, _name);
            return widget;
        }
        
		#endregion
	
		
		//InsertPoint
		#region Event ChangeCoord

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidgetEvent_AdviseChangeCoord(IntPtr _native, bool _advise);

		public delegate void HandleChangeCoord(
			Widget _sender);
			
		private HandleChangeCoord mEventChangeCoord;
		public event HandleChangeCoord EventChangeCoord
		{
			add
			{
				if (ExportEventChangeCoord.mDelegate == null)
				{
					ExportEventChangeCoord.mDelegate = new ExportEventChangeCoord.ExportHandle(OnExportChangeCoord);
					ExportEventChangeCoord.ExportWidgetEvent_DelegateChangeCoord(ExportEventChangeCoord.mDelegate);
				}

				if (mEventChangeCoord == null)
					ExportWidgetEvent_AdviseChangeCoord(Native, true);
				mEventChangeCoord += value;
			}
			remove
			{
				mEventChangeCoord -= value;
				if (mEventChangeCoord == null)
					ExportWidgetEvent_AdviseChangeCoord(Native, false);
			}
		}

		private struct ExportEventChangeCoord
		{
			[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportWidgetEvent_DelegateChangeCoord(ExportHandle _delegate);
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				IntPtr _sender);
				
			public static ExportHandle mDelegate;
		}

		private static void OnExportChangeCoord(
			IntPtr _sender)
		{
			Widget sender = (Widget)BaseWidget.GetByNative(_sender);

			if (sender.mEventChangeCoord != null)
				sender.mEventChangeCoord(
					sender);
		}

		#endregion
		#region Event ChangeProperty

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidgetEvent_AdviseChangeProperty(IntPtr _native, bool _advise);

		public delegate void HandleChangeProperty(
			Widget _sender,
			string _key,
			string _value);
			
		private HandleChangeProperty mEventChangeProperty;
		public event HandleChangeProperty EventChangeProperty
		{
			add
			{
				if (ExportEventChangeProperty.mDelegate == null)
				{
					ExportEventChangeProperty.mDelegate = new ExportEventChangeProperty.ExportHandle(OnExportChangeProperty);
					ExportEventChangeProperty.ExportWidgetEvent_DelegateChangeProperty(ExportEventChangeProperty.mDelegate);
				}

				if (mEventChangeProperty == null)
					ExportWidgetEvent_AdviseChangeProperty(Native, true);
				mEventChangeProperty += value;
			}
			remove
			{
				mEventChangeProperty -= value;
				if (mEventChangeProperty == null)
					ExportWidgetEvent_AdviseChangeProperty(Native, false);
			}
		}

		private struct ExportEventChangeProperty
		{
			[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportWidgetEvent_DelegateChangeProperty(ExportHandle _delegate);
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				IntPtr _sender,
				[MarshalAs(UnmanagedType.LPStr)] string _key,
				[MarshalAs(UnmanagedType.LPStr)] string _value);
				
			public static ExportHandle mDelegate;
		}

		private static void OnExportChangeProperty(
			IntPtr _sender,
			string _key,
			string _value)
		{
			Widget sender = (Widget)BaseWidget.GetByNative(_sender);

			if (sender.mEventChangeProperty != null)
				sender.mEventChangeProperty(
					sender ,
					_key,
					_value);
		}

		#endregion
		#region Method SetWidgetStyle

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetWidgetStyle__style__layer(IntPtr _native,
			[MarshalAs(UnmanagedType.I4)] WidgetStyle _style,
			[MarshalAs(UnmanagedType.LPStr)] string _layer);

		public void SetWidgetStyle(
			WidgetStyle _style,
			string _layer)
		{
			ExportWidget_SetWidgetStyle__style__layer(Native,
				_style,
				_layer);
		}

		#endregion
		#region Method ChangeWidgetSkin

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_ChangeWidgetSkin__skinName(IntPtr _native,
			[MarshalAs(UnmanagedType.LPStr)] string _skinName);

		public void ChangeWidgetSkin(
			string _skinName)
		{
			ExportWidget_ChangeWidgetSkin__skinName(Native,
				_skinName);
		}

		#endregion
		#region Method AttachToWidget

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_AttachToWidget__parent__style__layer(IntPtr _native,
			IntPtr _parent,
			[MarshalAs(UnmanagedType.I4)] WidgetStyle _style,
			[MarshalAs(UnmanagedType.LPStr)] string _layer);

		public void AttachToWidget(
			Widget _parent,
			WidgetStyle _style,
			string _layer)
		{
			ExportWidget_AttachToWidget__parent__style__layer(Native,
				_parent.Native,
				_style,
				_layer);
		}

		#endregion
		#region Method DetachFromWidget

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_DetachFromWidget__layer(IntPtr _native,
			[MarshalAs(UnmanagedType.LPStr)] string _layer);

		public void DetachFromWidget(
			string _layer)
		{
			ExportWidget_DetachFromWidget__layer(Native,
				_layer);
		}

		#endregion
		#region Method SetEnabledSilent

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetEnabledSilent__value(IntPtr _native,
			[MarshalAs(UnmanagedType.U1)] bool _value);

		public void SetEnabledSilent(
			bool _value)
		{
			ExportWidget_SetEnabledSilent__value(Native,
				_value);
		}

		#endregion
		#region Method FindWidget

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportWidget_FindWidget__name(IntPtr _native,
			[MarshalAs(UnmanagedType.LPStr)] string _name);

		public Widget FindWidget(
			string _name)
		{
			return (Widget)BaseWidget.GetByNative(ExportWidget_FindWidget__name(Native,
				_name));
		}

		#endregion
		#region Method GetChildAt

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportWidget_GetChildAt__index(IntPtr _native,
			uint _index);

		public Widget GetChildAt(
			uint _index)
		{
			return (Widget)BaseWidget.GetByNative(ExportWidget_GetChildAt__index(Native,
				_index));
		}

		#endregion
		#region Method SetColour

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetColour__value(IntPtr _native,
			[In] ref Colour _value);

		public void SetColour(
			Colour _value)
		{
			ExportWidget_SetColour__value(Native,
				ref _value);
		}

		#endregion
		#region Method SetRealCoord

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetRealCoord__left__top__width__height(IntPtr _native,
			float _left,
			float _top,
			float _width,
			float _height);

		public void SetRealCoord(
			float _left,
			float _top,
			float _width,
			float _height)
		{
			ExportWidget_SetRealCoord__left__top__width__height(Native,
				_left,
				_top,
				_width,
				_height);
		}

		#endregion
		#region Method SetRealSize

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetRealSize__width__height(IntPtr _native,
			float _width,
			float _height);

		public void SetRealSize(
			float _width,
			float _height)
		{
			ExportWidget_SetRealSize__width__height(Native,
				_width,
				_height);
		}

		#endregion
		#region Method SetRealPosition

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetRealPosition__left__top(IntPtr _native,
			float _left,
			float _top);

		public void SetRealPosition(
			float _left,
			float _top)
		{
			ExportWidget_SetRealPosition__left__top(Native,
				_left,
				_top);
		}

		#endregion
		#region Method SetRealCoord

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetRealCoord__value(IntPtr _native,
			[In] ref FloatCoord _value);

		public void SetRealCoord(
			FloatCoord _value)
		{
			ExportWidget_SetRealCoord__value(Native,
				ref _value);
		}

		#endregion
		#region Method SetRealSize

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetRealSize__value(IntPtr _native,
			[In] ref FloatSize _value);

		public void SetRealSize(
			FloatSize _value)
		{
			ExportWidget_SetRealSize__value(Native,
				ref _value);
		}

		#endregion
		#region Method SetRealPosition

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetRealPosition__value(IntPtr _native,
			[In] ref FloatPoint _value);

		public void SetRealPosition(
			FloatPoint _value)
		{
			ExportWidget_SetRealPosition__value(Native,
				ref _value);
		}

		#endregion
		#region Method SetCoord

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetCoord__left__top__width__height(IntPtr _native,
			int _left,
			int _top,
			int _width,
			int _height);

		public void SetCoord(
			int _left,
			int _top,
			int _width,
			int _height)
		{
			ExportWidget_SetCoord__left__top__width__height(Native,
				_left,
				_top,
				_width,
				_height);
		}

		#endregion
		#region Method SetSize

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetSize__width__height(IntPtr _native,
			int _width,
			int _height);

		public void SetSize(
			int _width,
			int _height)
		{
			ExportWidget_SetSize__width__height(Native,
				_width,
				_height);
		}

		#endregion
		#region Method SetPosition

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetPosition__left__top(IntPtr _native,
			int _left,
			int _top);

		public void SetPosition(
			int _left,
			int _top)
		{
			ExportWidget_SetPosition__left__top(Native,
				_left,
				_top);
		}

		#endregion
		#region Property WidgetStyle

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern WidgetStyle ExportWidget_GetWidgetStyle(IntPtr _native);

		public WidgetStyle WidgetStyle
		{
			get { return ExportWidget_GetWidgetStyle(Native); }
		}

		#endregion
		#region Property ClientWidget

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportWidget_GetClientWidget(IntPtr _native);

		public Widget ClientWidget
		{
			get { return (Widget)BaseWidget.GetByNative(ExportWidget_GetClientWidget(Native)); }
		}

		#endregion
		#region Property ClientCoord

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportWidget_GetClientCoord(IntPtr _native);

		public IntCoord ClientCoord
		{
			get { return (IntCoord)Marshal.PtrToStructure(ExportWidget_GetClientCoord(Native), typeof(IntCoord)); }
		}

		#endregion
		#region Property InheritedEnabled

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern bool ExportWidget_GetInheritedEnabled(IntPtr _native);

		public bool InheritedEnabled
		{
			get { return ExportWidget_GetInheritedEnabled(Native); }
		}

		#endregion
		#region Property Enabled

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern bool ExportWidget_GetEnabled(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetEnabled(IntPtr _widget, [MarshalAs(UnmanagedType.U1)] bool _value);

		public bool Enabled
		{
			get { return ExportWidget_GetEnabled(Native); }
			set { ExportWidget_SetEnabled(Native, value); }
		}

		#endregion
		#region Property ChildCount

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportWidget_GetChildCount(IntPtr _native);

		public uint ChildCount
		{
			get { return ExportWidget_GetChildCount(Native); }
		}

		#endregion
		#region Property ParentSize

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportWidget_GetParentSize(IntPtr _native);

		public IntSize ParentSize
		{
			get { return (IntSize)Marshal.PtrToStructure(ExportWidget_GetParentSize(Native), typeof(IntSize)); }
		}

		#endregion
		#region Property Parent

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportWidget_GetParent(IntPtr _native);

		public Widget Parent
		{
			get { return (Widget)BaseWidget.GetByNative(ExportWidget_GetParent(Native)); }
		}

		#endregion
		#region Property IsRootWidget

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern bool ExportWidget_IsRootWidget(IntPtr _native);

		public bool IsRootWidget
		{
			get { return ExportWidget_IsRootWidget(Native); }
		}

		#endregion
		#region Property InheritsAlpha

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern bool ExportWidget_GetInheritsAlpha(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetInheritsAlpha(IntPtr _widget, [MarshalAs(UnmanagedType.U1)] bool _value);

		public bool InheritsAlpha
		{
			get { return ExportWidget_GetInheritsAlpha(Native); }
			set { ExportWidget_SetInheritsAlpha(Native, value); }
		}

		#endregion
		#region Property Alpha

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern float ExportWidget_GetAlpha(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetAlpha(IntPtr _widget, float _value);

		public float Alpha
		{
			get { return ExportWidget_GetAlpha(Native); }
			set { ExportWidget_SetAlpha(Native, value); }
		}

		#endregion
		#region Property Align

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern Align ExportWidget_GetAlign(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetAlign(IntPtr _widget, [MarshalAs(UnmanagedType.I4)] Align _value);

		public Align Align
		{
			get { return ExportWidget_GetAlign(Native); }
			set { ExportWidget_SetAlign(Native, value); }
		}

		#endregion
		#region Property InheritedVisible

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern bool ExportWidget_GetInheritedVisible(IntPtr _native);

		public bool InheritedVisible
		{
			get { return ExportWidget_GetInheritedVisible(Native); }
		}

		#endregion
		#region Property Visible

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern bool ExportWidget_GetVisible(IntPtr _widget);
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetVisible(IntPtr _widget, [MarshalAs(UnmanagedType.U1)] bool _value);

		public bool Visible
		{
			get { return ExportWidget_GetVisible(Native); }
			set { ExportWidget_SetVisible(Native, value); }
		}

		#endregion
		#region Property Name

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportWidget_GetName(IntPtr _native);

		public string Name
		{
			get { return Marshal.PtrToStringAnsi(ExportWidget_GetName(Native)); }
		}

		#endregion
		
    }
}
