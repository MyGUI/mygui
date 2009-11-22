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

        public Widget()
            : base()
        {
        }

        internal Widget(BaseWidget _parent, IntPtr _widget)
            : base(_parent, _widget)
        {
        }

        internal Widget(IntPtr _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
            : base(_parent, _style, _skin, _coord, _align, _layer, _name)
        {
        }

        protected override string GetWidgetType() { return "Widget"; }

        internal static BaseWidget RequestWrapWidget(BaseWidget _parent, IntPtr _widget)
        {
            return new Widget(_parent, _widget);
        }

        internal static BaseWidget RequestCreateWidget(IntPtr _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
        {
            return new Widget(_parent, _style, _skin, _coord, _align, _layer, _name);
        }
        
		#endregion
	
		
		//InsertPoint

   		#region Method SetCaptionWithNewLine

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetCaptionWithNewLine_value( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPStr)]  string _value );

		public void SetCaptionWithNewLine(
			string _value )
		{
			ExportWidget_SetCaptionWithNewLine_value( mNative , 
				 _value );
		}

		#endregion



   


   		#region Method GetWidgetStyle

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I4)]
		private static extern WidgetStyle ExportWidget_GetWidgetStyle( IntPtr _native );

		public WidgetStyle GetWidgetStyle( )
		{
			return  ExportWidget_GetWidgetStyle( mNative )  ;
		}

		#endregion



   		#region Method SetWidgetStyle

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetWidgetStyle_style_layer( IntPtr _native ,
			[MarshalAs(UnmanagedType.I4)]  WidgetStyle _style ,
			[MarshalAs(UnmanagedType.LPStr)]  string _layer );

		public void SetWidgetStyle(
			WidgetStyle _style ,
			string _layer )
		{
			ExportWidget_SetWidgetStyle_style_layer( mNative , 
				 _style ,
				 _layer );
		}

		#endregion



   		#region Method ChangeWidgetSkin

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_ChangeWidgetSkin_skinname( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPStr)]  string _skinname );

		public void ChangeWidgetSkin(
			string _skinname )
		{
			ExportWidget_ChangeWidgetSkin_skinname( mNative , 
				 _skinname );
		}

		#endregion



   		#region Method AttachToWidget

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_AttachToWidget_parent_style_layer( IntPtr _native ,
			[MarshalAs(UnmanagedType.Interface)]  BaseWidget _parent ,
			[MarshalAs(UnmanagedType.I4)]  WidgetStyle _style ,
			[MarshalAs(UnmanagedType.LPStr)]  string _layer );

		public void AttachToWidget(
			Widget _parent ,
			WidgetStyle _style ,
			string _layer )
		{
			ExportWidget_AttachToWidget_parent_style_layer( mNative , 
				 _parent ,
				 _style ,
				 _layer );
		}

		#endregion



   		#region Method DetachFromWidget

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_DetachFromWidget_layer( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPStr)]  string _layer );

		public void DetachFromWidget(
			string _layer )
		{
			ExportWidget_DetachFromWidget_layer( mNative , 
				 _layer );
		}

		#endregion



   		#region Property EnableToolTip

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportWidget_GetEnableToolTip( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetEnableToolTip( IntPtr _widget, [MarshalAs(UnmanagedType.U1)]  bool _value );

		public bool EnableToolTip
		{
			get { return  ExportWidget_GetEnableToolTip( mNative )  ; }
			set { ExportWidget_SetEnableToolTip( mNative,  value ); }
		}

		#endregion



   		#region Property NeedToolTip

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportWidget_GetNeedToolTip( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetNeedToolTip( IntPtr _widget, [MarshalAs(UnmanagedType.U1)]  bool _value );

		public bool NeedToolTip
		{
			get { return  ExportWidget_GetNeedToolTip( mNative )  ; }
			set { ExportWidget_SetNeedToolTip( mNative,  value ); }
		}

		#endregion



   


   


   		#region Method GetClientWidget

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.Interface)]
		private static extern Widget ExportWidget_GetClientWidget( IntPtr _native );

		public Widget GetClientWidget( )
		{
			return  ExportWidget_GetClientWidget( mNative )  ;
		}

		#endregion



   		#region Method GetClientCoord

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportWidget_GetClientCoord( IntPtr _native );

		public IntCoord GetClientCoord( )
		{
			return  (IntCoord)Marshal.PtrToStructure(  ExportWidget_GetClientCoord( mNative )  , typeof(IntCoord) )  ;
		}

		#endregion



   		#region Method GetLayerName

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportWidget_GetLayerName( IntPtr _native );

		public string GetLayerName( )
		{
			return  Marshal.PtrToStringAnsi(  ExportWidget_GetLayerName( mNative )  )  ;
		}

		#endregion



   		#region Property Pointer

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportWidget_GetPointer( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetPointer( IntPtr _widget, [MarshalAs(UnmanagedType.LPStr)]  string _value );

		public string Pointer
		{
			get { return  Marshal.PtrToStringAnsi(  ExportWidget_GetPointer( mNative )  )  ; }
			set { ExportWidget_SetPointer( mNative,  value ); }
		}

		#endregion



   		#region Method SetEnabledSilent

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetEnabledSilent_value( IntPtr _native ,
			[MarshalAs(UnmanagedType.U1)]  bool _value );

		public void SetEnabledSilent(
			bool _value )
		{
			ExportWidget_SetEnabledSilent_value( mNative , 
				 _value );
		}

		#endregion



   		#region Property Enabled

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportWidget_IsEnabled( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetEnabled( IntPtr _widget, [MarshalAs(UnmanagedType.U1)]  bool _value );

		public bool Enabled
		{
			get { return  ExportWidget_IsEnabled( mNative )  ; }
			set { ExportWidget_SetEnabled( mNative,  value ); }
		}

		#endregion



   		#region Method SetMaskPick

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetMaskPick_filename( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPStr)]  string _filename );

		public void SetMaskPick(
			string _filename )
		{
			ExportWidget_SetMaskPick_filename( mNative , 
				 _filename );
		}

		#endregion



   		#region Property InheritsPick

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportWidget_IsInheritsPick( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetInheritsPick( IntPtr _widget, [MarshalAs(UnmanagedType.U1)]  bool _value );

		public bool InheritsPick
		{
			get { return  ExportWidget_IsInheritsPick( mNative )  ; }
			set { ExportWidget_SetInheritsPick( mNative,  value ); }
		}

		#endregion



   		#region Property NeedMouseFocus

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportWidget_IsNeedMouseFocus( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetNeedMouseFocus( IntPtr _widget, [MarshalAs(UnmanagedType.U1)]  bool _value );

		public bool NeedMouseFocus
		{
			get { return  ExportWidget_IsNeedMouseFocus( mNative )  ; }
			set { ExportWidget_SetNeedMouseFocus( mNative,  value ); }
		}

		#endregion



   		#region Property NeedKeyFocus

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportWidget_IsNeedKeyFocus( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetNeedKeyFocus( IntPtr _widget, [MarshalAs(UnmanagedType.U1)]  bool _value );

		public bool NeedKeyFocus
		{
			get { return  ExportWidget_IsNeedKeyFocus( mNative )  ; }
			set { ExportWidget_SetNeedKeyFocus( mNative,  value ); }
		}

		#endregion



   		#region Method FindWidget

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.Interface)]
		private static extern Widget ExportWidget_FindWidget_name( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPStr)]  string _name );

		public Widget FindWidget(
			string _name )
		{
			return  ExportWidget_FindWidget_name( mNative , 
				 _name )  ;
		}

		#endregion



   		#region Method GetChildAt

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.Interface)]
		private static extern Widget ExportWidget_GetChildAt_index( IntPtr _native ,
			  uint _index );

		public Widget GetChildAt(
			uint _index )
		{
			return  ExportWidget_GetChildAt_index( mNative , 
				 _index )  ;
		}

		#endregion



   		#region Method GetChildCount

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportWidget_GetChildCount( IntPtr _native );

		public uint GetChildCount( )
		{
			return  ExportWidget_GetChildCount( mNative )  ;
		}

		#endregion



   


   		#region Method GetParent

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.Interface)]
		private static extern Widget ExportWidget_GetParent( IntPtr _native );

		public Widget GetParent( )
		{
			return  ExportWidget_GetParent( mNative )  ;
		}

		#endregion



   		#region Method IsRootWidget

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportWidget_IsRootWidget( IntPtr _native );

		public bool IsRootWidget( )
		{
			return  ExportWidget_IsRootWidget( mNative )  ;
		}

		#endregion



   		#region Method SetColour

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetColour_value( IntPtr _native ,
			[In] ref Colour _value );

		public void SetColour(
			Colour _value )
		{
			ExportWidget_SetColour_value( mNative , 
				ref _value );
		}

		#endregion



   		#region Method SetState

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportWidget_SetState_value( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPStr)]  string _value );

		public bool SetState(
			string _value )
		{
			return  ExportWidget_SetState_value( mNative , 
				 _value )  ;
		}

		#endregion



   		#region Property InheritsAlpha

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportWidget_IsInheritsAlpha( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetInheritsAlpha( IntPtr _widget, [MarshalAs(UnmanagedType.U1)]  bool _value );

		public bool InheritsAlpha
		{
			get { return  ExportWidget_IsInheritsAlpha( mNative )  ; }
			set { ExportWidget_SetInheritsAlpha( mNative,  value ); }
		}

		#endregion



   		#region Property Alpha

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern float ExportWidget_GetAlpha( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetAlpha( IntPtr _widget,   float _value );

		public float Alpha
		{
			get { return  ExportWidget_GetAlpha( mNative )  ; }
			set { ExportWidget_SetAlpha( mNative,  value ); }
		}

		#endregion



   		#region Property Caption

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportWidget_GetCaption( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetCaption( IntPtr _widget, [MarshalAs(UnmanagedType.LPWStr)]  string _value );

		public string Caption
		{
			get { return  Marshal.PtrToStringUni(  ExportWidget_GetCaption( mNative )  )  ; }
			set { ExportWidget_SetCaption( mNative,  value ); }
		}

		#endregion



   		#region Method SetAlign

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetAlign_value( IntPtr _native ,
			[MarshalAs(UnmanagedType.I4)]  Align _value );

		public void SetAlign(
			Align _value )
		{
			ExportWidget_SetAlign_value( mNative , 
				 _value );
		}

		#endregion



   		#region Method SetVisible

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetVisible_value( IntPtr _native ,
			[MarshalAs(UnmanagedType.U1)]  bool _value );

		public void SetVisible(
			bool _value )
		{
			ExportWidget_SetVisible_value( mNative , 
				 _value );
		}

		#endregion



   		#region Method SetRealCoord

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
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
			ExportWidget_SetRealCoord_left_top_width_height( mNative , 
				 _left ,
				 _top ,
				 _width ,
				 _height );
		}

		#endregion



   		#region Method SetRealSize

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetRealSize_width_height( IntPtr _native ,
			  float _width ,
			  float _height );

		public void SetRealSize(
			float _width ,
			float _height )
		{
			ExportWidget_SetRealSize_width_height( mNative , 
				 _width ,
				 _height );
		}

		#endregion



   		#region Method SetRealPosition

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetRealPosition_left_top( IntPtr _native ,
			  float _left ,
			  float _top );

		public void SetRealPosition(
			float _left ,
			float _top )
		{
			ExportWidget_SetRealPosition_left_top( mNative , 
				 _left ,
				 _top );
		}

		#endregion



   		#region Method SetRealCoord

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetRealCoord_value( IntPtr _native ,
			[In] ref FloatCoord _value );

		public void SetRealCoord(
			FloatCoord _value )
		{
			ExportWidget_SetRealCoord_value( mNative , 
				ref _value );
		}

		#endregion



   		#region Method SetRealSize

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetRealSize_value( IntPtr _native ,
			[In] ref FloatSize _value );

		public void SetRealSize(
			FloatSize _value )
		{
			ExportWidget_SetRealSize_value( mNative , 
				ref _value );
		}

		#endregion



   		#region Method SetRealPosition

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetRealPosition_value( IntPtr _native ,
			[In] ref FloatPoint _value );

		public void SetRealPosition(
			FloatPoint _value )
		{
			ExportWidget_SetRealPosition_value( mNative , 
				ref _value );
		}

		#endregion



   		#region Method SetCoord

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
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
			ExportWidget_SetCoord_left_top_width_height( mNative , 
				 _left ,
				 _top ,
				 _width ,
				 _height );
		}

		#endregion



   		#region Method SetSize

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetSize_width_height( IntPtr _native ,
			  int _width ,
			  int _height );

		public void SetSize(
			int _width ,
			int _height )
		{
			ExportWidget_SetSize_width_height( mNative , 
				 _width ,
				 _height );
		}

		#endregion



   		#region Method SetPosition

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetPosition_left_top( IntPtr _native ,
			  int _left ,
			  int _top );

		public void SetPosition(
			int _left ,
			int _top )
		{
			ExportWidget_SetPosition_left_top( mNative , 
				 _left ,
				 _top );
		}

		#endregion



   		#region Method SetCoord

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetCoord_value( IntPtr _native ,
			[In] ref IntCoord _value );

		public void SetCoord(
			IntCoord _value )
		{
			ExportWidget_SetCoord_value( mNative , 
				ref _value );
		}

		#endregion



   		#region Method SetSize

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetSize_value( IntPtr _native ,
			 ref IntSize _value );

		public void SetSize(
			IntSize _value )
		{
			ExportWidget_SetSize_value( mNative , 
				ref _value );
		}

		#endregion



   		#region Method SetPosition

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetPosition_value( IntPtr _native ,
			[In] ref IntPoint _value );

		public void SetPosition(
			IntPoint _value )
		{
			ExportWidget_SetPosition_value( mNative , 
				ref _value );
		}

		#endregion



   		#region Method GetName

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportWidget_GetName( IntPtr _native );

		public string GetName( )
		{
			return  Marshal.PtrToStringAnsi(  ExportWidget_GetName( mNative )  )  ;
		}

		#endregion



   


   


   


   


   


   


   


   


   


   


   


   


   


   


   

		
    }

}
