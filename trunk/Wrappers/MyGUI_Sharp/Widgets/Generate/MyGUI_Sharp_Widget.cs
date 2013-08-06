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

    public partial class Widget : BaseWidget
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
		private static extern void ExportWidgetEvent_AdviseChangeCoord( IntPtr _native, bool _advise );

		public delegate void HandleChangeCoord(
			 Widget _sender );
			
		private HandleChangeCoord mEventChangeCoord;
		public event HandleChangeCoord EventChangeCoord
		{
			add
			{
				if (ExportEventChangeCoord.mDelegate == null)
				{
					ExportEventChangeCoord.mDelegate = new ExportEventChangeCoord.ExportHandle( OnExportChangeCoord );
					ExportEventChangeCoord.ExportWidgetEvent_DelegateChangeCoord( ExportEventChangeCoord.mDelegate );
				}

				if (mEventChangeCoord == null)
					ExportWidgetEvent_AdviseChangeCoord( Native, true );
				mEventChangeCoord += value;
			}
			remove
			{
				mEventChangeCoord -= value;
				if (mEventChangeCoord == null)
					ExportWidgetEvent_AdviseChangeCoord( Native, false );
			}
		}

		private struct ExportEventChangeCoord
		{
			[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportWidgetEvent_DelegateChangeCoord( ExportHandle _delegate );
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  Widget _sender );
				
			public static ExportHandle mDelegate;
		}

		private static void OnExportChangeCoord(
			 Widget _sender )
		{
			if (_sender.mEventChangeCoord != null)
				_sender.mEventChangeCoord(
					 _sender );
		}

		#endregion
		#region Event ChangeProperty

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidgetEvent_AdviseChangeProperty( IntPtr _native, bool _advise );

		public delegate void HandleChangeProperty(
			 Widget _sender ,
			 string _key ,
			 string _value );
			
		private HandleChangeProperty mEventChangeProperty;
		public event HandleChangeProperty EventChangeProperty
		{
			add
			{
				if (ExportEventChangeProperty.mDelegate == null)
				{
					ExportEventChangeProperty.mDelegate = new ExportEventChangeProperty.ExportHandle( OnExportChangeProperty );
					ExportEventChangeProperty.ExportWidgetEvent_DelegateChangeProperty( ExportEventChangeProperty.mDelegate );
				}

				if (mEventChangeProperty == null)
					ExportWidgetEvent_AdviseChangeProperty( Native, true );
				mEventChangeProperty += value;
			}
			remove
			{
				mEventChangeProperty -= value;
				if (mEventChangeProperty == null)
					ExportWidgetEvent_AdviseChangeProperty( Native, false );
			}
		}

		private struct ExportEventChangeProperty
		{
			[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
			public static extern void ExportWidgetEvent_DelegateChangeProperty( ExportHandle _delegate );
			[UnmanagedFunctionPointer(CallingConvention.StdCall)]
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  Widget _sender ,
				[MarshalAs(UnmanagedType.LPStr)]  string _key ,
				[MarshalAs(UnmanagedType.LPStr)]  string _value );
				
			public static ExportHandle mDelegate;
		}

		private static void OnExportChangeProperty(
			 Widget _sender ,
			 string _key ,
			 string _value )
		{
			if (_sender.mEventChangeProperty != null)
				_sender.mEventChangeProperty(
					 _sender ,
					 _key ,
					 _value );
		}

		#endregion
		#region Method SetProperty

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetProperty_key_value( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPStr)]  string _key ,
			[MarshalAs(UnmanagedType.LPStr)]  string _value );

		public void SetProperty(
			string _key ,
			string _value )
		{
			ExportWidget_SetProperty_key_value( Native , 
				 _key ,
				 _value );
		}

		#endregion
		#region Method SetWidgetStyle

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetWidgetStyle_style_layer( IntPtr _native ,
			[MarshalAs(UnmanagedType.I4)]  WidgetStyle _style ,
			[MarshalAs(UnmanagedType.LPStr)]  string _layer );

		public void SetWidgetStyle(
			WidgetStyle _style ,
			string _layer )
		{
			ExportWidget_SetWidgetStyle_style_layer( Native , 
				 _style ,
				 _layer );
		}

		#endregion
		#region Method ChangeWidgetSkin

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_ChangeWidgetSkin_skinName( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPStr)]  string _skinName );

		public void ChangeWidgetSkin(
			string _skinName )
		{
			ExportWidget_ChangeWidgetSkin_skinName( Native , 
				 _skinName );
		}

		#endregion
		#region Method AttachToWidget

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_AttachToWidget_parent_style_layer( IntPtr _native ,
			[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  BaseWidget _parent ,
			[MarshalAs(UnmanagedType.I4)]  WidgetStyle _style ,
			[MarshalAs(UnmanagedType.LPStr)]  string _layer );

		public void AttachToWidget(
			Widget _parent ,
			WidgetStyle _style ,
			string _layer )
		{
			ExportWidget_AttachToWidget_parent_style_layer( Native , 
				 _parent ,
				 _style ,
				 _layer );
		}

		#endregion
		#region Method DetachFromWidget

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_DetachFromWidget_layer( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPStr)]  string _layer );

		public void DetachFromWidget(
			string _layer )
		{
			ExportWidget_DetachFromWidget_layer( Native , 
				 _layer );
		}

		#endregion
		#region Method SetEnabledSilent

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetEnabledSilent_value( IntPtr _native ,
			[MarshalAs(UnmanagedType.U1)]  bool _value );

		public void SetEnabledSilent(
			bool _value )
		{
			ExportWidget_SetEnabledSilent_value( Native , 
				 _value );
		}

		#endregion
		#region Method FindWidget

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]
		private static extern Widget ExportWidget_FindWidget_name( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPStr)]  string _name );

		public Widget FindWidget(
			string _name )
		{
			return  ExportWidget_FindWidget_name( Native , 
				 _name )  ;
		}

		#endregion
		#region Method GetChildAt

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]
		private static extern Widget ExportWidget_GetChildAt_index( IntPtr _native ,
			  uint _index );

		public Widget GetChildAt(
			uint _index )
		{
			return  ExportWidget_GetChildAt_index( Native , 
				 _index )  ;
		}

		#endregion
		#region Method SetColour

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetColour_value( IntPtr _native ,
			[In] ref Colour _value );

		public void SetColour(
			Colour _value )
		{
			ExportWidget_SetColour_value( Native , 
				ref _value );
		}

		#endregion
		#region Method SetRealCoord

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetRealCoord_left_top_width_height( IntPtr _native ,
			  float _left ,
			  float _top ,
			  float _width ,
			  float _height );

		public void SetRealCoord(
			float _left ,
			float _top ,
			float _width ,
			float _height )
		{
			ExportWidget_SetRealCoord_left_top_width_height( Native , 
				 _left ,
				 _top ,
				 _width ,
				 _height );
		}

		#endregion
		#region Method SetRealSize

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetRealSize_width_height( IntPtr _native ,
			  float _width ,
			  float _height );

		public void SetRealSize(
			float _width ,
			float _height )
		{
			ExportWidget_SetRealSize_width_height( Native , 
				 _width ,
				 _height );
		}

		#endregion
		#region Method SetRealPosition

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetRealPosition_left_top( IntPtr _native ,
			  float _left ,
			  float _top );

		public void SetRealPosition(
			float _left ,
			float _top )
		{
			ExportWidget_SetRealPosition_left_top( Native , 
				 _left ,
				 _top );
		}

		#endregion
		#region Method SetRealCoord

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetRealCoord_value( IntPtr _native ,
			[In] ref FloatCoord _value );

		public void SetRealCoord(
			FloatCoord _value )
		{
			ExportWidget_SetRealCoord_value( Native , 
				ref _value );
		}

		#endregion
		#region Method SetRealSize

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetRealSize_value( IntPtr _native ,
			[In] ref FloatSize _value );

		public void SetRealSize(
			FloatSize _value )
		{
			ExportWidget_SetRealSize_value( Native , 
				ref _value );
		}

		#endregion
		#region Method SetRealPosition

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetRealPosition_value( IntPtr _native ,
			[In] ref FloatPoint _value );

		public void SetRealPosition(
			FloatPoint _value )
		{
			ExportWidget_SetRealPosition_value( Native , 
				ref _value );
		}

		#endregion
		#region Method SetCoord

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetCoord_left_top_width_height( IntPtr _native ,
			  int _left ,
			  int _top ,
			  int _width ,
			  int _height );

		public void SetCoord(
			int _left ,
			int _top ,
			int _width ,
			int _height )
		{
			ExportWidget_SetCoord_left_top_width_height( Native , 
				 _left ,
				 _top ,
				 _width ,
				 _height );
		}

		#endregion
		#region Method SetSize

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetSize_width_height( IntPtr _native ,
			  int _width ,
			  int _height );

		public void SetSize(
			int _width ,
			int _height )
		{
			ExportWidget_SetSize_width_height( Native , 
				 _width ,
				 _height );
		}

		#endregion
		#region Method SetPosition

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetPosition_left_top( IntPtr _native ,
			  int _left ,
			  int _top );

		public void SetPosition(
			int _left ,
			int _top )
		{
			ExportWidget_SetPosition_left_top( Native , 
				 _left ,
				 _top );
		}

		#endregion
		#region Property WidgetStyle

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I4)]
		private static extern WidgetStyle ExportWidget_GetWidgetStyle( IntPtr _native );

		public WidgetStyle WidgetStyle
		{
			get { return  ExportWidget_GetWidgetStyle( Native )  ; }
		}

		#endregion
		#region Property ClientWidget

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]
		private static extern Widget ExportWidget_GetClientWidget( IntPtr _native );

		public Widget ClientWidget
		{
			get { return  ExportWidget_GetClientWidget( Native )  ; }
		}

		#endregion
		#region Property ClientCoord

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportWidget_GetClientCoord( IntPtr _native );

		public IntCoord ClientCoord
		{
			get { return  (IntCoord)Marshal.PtrToStructure(  ExportWidget_GetClientCoord( Native )  , typeof(IntCoord) )  ; }
		}

		#endregion
		#region Property InheritedEnabled

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportWidget_GetInheritedEnabled( IntPtr _native );

		public bool InheritedEnabled
		{
			get { return  ExportWidget_GetInheritedEnabled( Native )  ; }
		}

		#endregion
		#region Property Enabled

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportWidget_GetEnabled( IntPtr _widget );
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetEnabled( IntPtr _widget, [MarshalAs(UnmanagedType.U1)]  bool _value );

		public bool Enabled
		{
			get { return  ExportWidget_GetEnabled( Native )  ; }
			set { ExportWidget_SetEnabled( Native,  value ); }
		}

		#endregion
		#region Property ChildCount

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportWidget_GetChildCount( IntPtr _native );

		public uint ChildCount
		{
			get { return  ExportWidget_GetChildCount( Native )  ; }
		}

		#endregion
		#region Property ParentSize

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportWidget_GetParentSize( IntPtr _native );

		public IntSize ParentSize
		{
			get { return  (IntSize)Marshal.PtrToStructure(  ExportWidget_GetParentSize( Native )  , typeof(IntSize) )  ; }
		}

		#endregion
		#region Property Parent

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]
		private static extern Widget ExportWidget_GetParent( IntPtr _native );

		public Widget Parent
		{
			get { return  ExportWidget_GetParent( Native )  ; }
		}

		#endregion
		#region Property IsRootWidget

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportWidget_IsRootWidget( IntPtr _native );

		public bool IsRootWidget
		{
			get { return  ExportWidget_IsRootWidget( Native )  ; }
		}

		#endregion
		#region Property InheritsAlpha

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportWidget_GetInheritsAlpha( IntPtr _widget );
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetInheritsAlpha( IntPtr _widget, [MarshalAs(UnmanagedType.U1)]  bool _value );

		public bool InheritsAlpha
		{
			get { return  ExportWidget_GetInheritsAlpha( Native )  ; }
			set { ExportWidget_SetInheritsAlpha( Native,  value ); }
		}

		#endregion
		#region Property Alpha

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern float ExportWidget_GetAlpha( IntPtr _widget );
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetAlpha( IntPtr _widget,   float _value );

		public float Alpha
		{
			get { return  ExportWidget_GetAlpha( Native )  ; }
			set { ExportWidget_SetAlpha( Native,  value ); }
		}

		#endregion
		#region Property Align

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I4)]
		private static extern Align ExportWidget_GetAlign( IntPtr _widget );
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetAlign( IntPtr _widget, [MarshalAs(UnmanagedType.I4)]  Align _value );

		public Align Align
		{
			get { return  ExportWidget_GetAlign( Native )  ; }
			set { ExportWidget_SetAlign( Native,  value ); }
		}

		#endregion
		#region Property InheritedVisible

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportWidget_GetInheritedVisible( IntPtr _native );

		public bool InheritedVisible
		{
			get { return  ExportWidget_GetInheritedVisible( Native )  ; }
		}

		#endregion
		#region Property Visible

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportWidget_GetVisible( IntPtr _widget );
		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetVisible( IntPtr _widget, [MarshalAs(UnmanagedType.U1)]  bool _value );

		public bool Visible
		{
			get { return  ExportWidget_GetVisible( Native )  ; }
			set { ExportWidget_SetVisible( Native,  value ); }
		}

		#endregion
		#region Property Name

		[DllImport(DllName.m_dllName, CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportWidget_GetName( IntPtr _native );

		public string Name
		{
			get { return  Marshal.PtrToStringAnsi(  ExportWidget_GetName( Native )  )  ; }
		}

		#endregion
		
    }

}
