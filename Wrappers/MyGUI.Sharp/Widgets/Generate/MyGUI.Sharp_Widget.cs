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
			widget.CreateWidget(_parent, _style, _skin, _coord, _align, _layer, _name);
            return widget;
        }
        
		#endregion
	
		
		//InsertPoint

   		#region Event ChangeProperty

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
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
				if (mEventChangeProperty == null) ExportWidgetEvent_AdviseChangeProperty( mNative, true );
				mEventChangeProperty += value;
			}
			remove
			{
				mEventChangeProperty -= value;
				if (mEventChangeProperty == null) ExportWidgetEvent_AdviseChangeProperty( mNative, false );
			}
		}


		private struct ExportEventChangeProperty
		{
			[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ExportWidgetEvent_DelegateChangeProperty( ExportHandle _delegate );
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  Widget _sender ,
				[MarshalAs(UnmanagedType.LPStr)]  string _key ,
				[MarshalAs(UnmanagedType.LPStr)]  string _value );
				
			private static ExportHandle mDelegate;
			public ExportEventChangeProperty( ExportHandle _delegate )
			{
				mDelegate = _delegate;
				ExportWidgetEvent_DelegateChangeProperty( mDelegate );
			}
		}
		static ExportEventChangeProperty mExportChangeProperty =
			new ExportEventChangeProperty(new ExportEventChangeProperty.ExportHandle( OnExportChangeProperty ));

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

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetProperty_key_value( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPStr)]  string _key ,
			[MarshalAs(UnmanagedType.LPStr)]  string _value );

		public void SetProperty(
			string _key ,
			string _value )
		{
			ExportWidget_SetProperty_key_value( mNative , 
				 _key ,
				 _value );
		}

		#endregion



   		#region Method GetWidgetStyle

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I4)]
		private static extern WidgetStyle ExportWidget_GetWidgetStyle( IntPtr _native );

		public WidgetStyle GetWidgetStyle( )
		{
			return  ExportWidget_GetWidgetStyle( mNative )  ;
		}

		#endregion



   		#region Method SetWidgetStyle

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
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

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_ChangeWidgetSkin_skinName( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPStr)]  string _skinName );

		public void ChangeWidgetSkin(
			string _skinName )
		{
			ExportWidget_ChangeWidgetSkin_skinName( mNative , 
				 _skinName );
		}

		#endregion



   		#region Method AttachToWidget

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_AttachToWidget_parent_style_layer( IntPtr _native ,
			[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  BaseWidget _parent ,
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

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_DetachFromWidget_layer( IntPtr _native ,
			[MarshalAs(UnmanagedType.LPStr)]  string _layer );

		public void DetachFromWidget(
			string _layer )
		{
			ExportWidget_DetachFromWidget_layer( mNative , 
				 _layer );
		}

		#endregion



   		#region Method GetClientWidget

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]
		private static extern Widget ExportWidget_GetClientWidget( IntPtr _native );

		public Widget GetClientWidget( )
		{
			return  ExportWidget_GetClientWidget( mNative )  ;
		}

		#endregion



   		#region Method GetClientCoord

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportWidget_GetClientCoord( IntPtr _native );

		public IntCoord GetClientCoord( )
		{
			return  (IntCoord)Marshal.PtrToStructure(  ExportWidget_GetClientCoord( mNative )  , typeof(IntCoord) )  ;
		}

		#endregion



   		#region Method GetInheritedEnabled

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportWidget_GetInheritedEnabled( IntPtr _native );

		public bool GetInheritedEnabled( )
		{
			return  ExportWidget_GetInheritedEnabled( mNative )  ;
		}

		#endregion



   		#region Method SetEnabledSilent

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
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

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportWidget_GetEnabled( IntPtr _widget );
		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetEnabled( IntPtr _widget, [MarshalAs(UnmanagedType.U1)]  bool _value );

		public bool Enabled
		{
			get { return  ExportWidget_GetEnabled( mNative )  ; }
			set { ExportWidget_SetEnabled( mNative,  value ); }
		}

		#endregion



   


   		#region Method FindWidget

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]
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

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]
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

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportWidget_GetChildCount( IntPtr _native );

		public uint GetChildCount( )
		{
			return  ExportWidget_GetChildCount( mNative )  ;
		}

		#endregion



   


   		#region Method GetParentSize

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportWidget_GetParentSize( IntPtr _native );

		public IntSize GetParentSize( )
		{
			return  (IntSize)Marshal.PtrToStructure(  ExportWidget_GetParentSize( mNative )  , typeof(IntSize) )  ;
		}

		#endregion



   		#region Method GetParent

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]
		private static extern Widget ExportWidget_GetParent( IntPtr _native );

		public Widget GetParent( )
		{
			return  ExportWidget_GetParent( mNative )  ;
		}

		#endregion



   		#region Method IsRootWidget

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportWidget_IsRootWidget( IntPtr _native );

		public bool IsRootWidget( )
		{
			return  ExportWidget_IsRootWidget( mNative )  ;
		}

		#endregion



   		#region Method SetColour

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetColour_value( IntPtr _native ,
			[In] ref Colour _value );

		public void SetColour(
			Colour _value )
		{
			ExportWidget_SetColour_value( mNative , 
				ref _value );
		}

		#endregion



   		#region Property InheritsAlpha

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportWidget_GetInheritsAlpha( IntPtr _widget );
		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetInheritsAlpha( IntPtr _widget, [MarshalAs(UnmanagedType.U1)]  bool _value );

		public bool InheritsAlpha
		{
			get { return  ExportWidget_GetInheritsAlpha( mNative )  ; }
			set { ExportWidget_SetInheritsAlpha( mNative,  value ); }
		}

		#endregion



   		#region Property Alpha

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern float ExportWidget_GetAlpha( IntPtr _widget );
		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetAlpha( IntPtr _widget,   float _value );

		public float Alpha
		{
			get { return  ExportWidget_GetAlpha( mNative )  ; }
			set { ExportWidget_SetAlpha( mNative,  value ); }
		}

		#endregion



   		#region Property Align

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.I4)]
		private static extern Align ExportWidget_GetAlign( IntPtr _widget );
		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetAlign( IntPtr _widget, [MarshalAs(UnmanagedType.I4)]  Align _value );

		public Align Align
		{
			get { return  ExportWidget_GetAlign( mNative )  ; }
			set { ExportWidget_SetAlign( mNative,  value ); }
		}

		#endregion



   		#region Method GetInheritedVisible

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportWidget_GetInheritedVisible( IntPtr _native );

		public bool GetInheritedVisible( )
		{
			return  ExportWidget_GetInheritedVisible( mNative )  ;
		}

		#endregion



   		#region Property Visible

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportWidget_GetVisible( IntPtr _widget );
		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetVisible( IntPtr _widget, [MarshalAs(UnmanagedType.U1)]  bool _value );

		public bool Visible
		{
			get { return  ExportWidget_GetVisible( mNative )  ; }
			set { ExportWidget_SetVisible( mNative,  value ); }
		}

		#endregion



   		#region Method GetName

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportWidget_GetName( IntPtr _native );

		public string GetName( )
		{
			return  Marshal.PtrToStringAnsi(  ExportWidget_GetName( mNative )  )  ;
		}

		#endregion



   		#region Method SetRealCoord

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
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

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
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

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
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

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
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

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
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

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
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

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
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

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
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

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
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

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
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

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
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

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWidget_SetPosition_value( IntPtr _native ,
			[In] ref IntPoint _value );

		public void SetPosition(
			IntPoint _value )
		{
			ExportWidget_SetPosition_value( mNative , 
				ref _value );
		}

		#endregion



   


   


   


   


   


   


   


   


   


   


   


   


   

		
    }

}
