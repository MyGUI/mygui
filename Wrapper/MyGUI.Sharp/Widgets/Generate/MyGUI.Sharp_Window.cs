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

    public  class Window : Widget
    {

        #region Window

        public Window()
            : base()
        {
        }

        internal Window(BaseWidget _parent, IntPtr _widget)
            : base(_parent, _widget)
        {
        }

        internal Window(IntPtr _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
            : base(_parent, _style, _skin, _coord, _align, _layer, _name)
        {
        }

        protected override string GetWidgetType() { return "Window"; }

        internal static BaseWidget RequestWrapWindow(BaseWidget _parent, IntPtr _widget)
        {
            return new Window(_parent, _widget);
        }

        internal static BaseWidget RequestCreateWindow(IntPtr _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
        {
            return new Window(_parent, _style, _skin, _coord, _align, _layer, _name);
        }
        
		#endregion
	
		
		//InsertPoint

   		#region Event WindowChangeCoord

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWindowEvent_AdviseWindowChangeCoord( IntPtr _native, bool _advise );

		public delegate void HandleWindowChangeCoord(
			 Window _sender );
			
		private HandleWindowChangeCoord mEventWindowChangeCoord;
		public event HandleWindowChangeCoord EventWindowChangeCoord
		{
			add
			{
				if (mEventWindowChangeCoord == null) ExportWindowEvent_AdviseWindowChangeCoord( mNative, true );
				mEventWindowChangeCoord += value;
			}
			remove
			{
				mEventWindowChangeCoord -= value;
				if (mEventWindowChangeCoord == null) ExportWindowEvent_AdviseWindowChangeCoord( mNative, false );
			}
		}


		private struct ExportEventWindowChangeCoord
		{
			[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ExportWindowEvent_DelegateWindowChangeCoord( ExportHandle _delegate );
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.Interface)]  Window _sender );
				
			private static ExportHandle mDelegate;
			public ExportEventWindowChangeCoord( ExportHandle _delegate )
			{
				mDelegate = _delegate;
				ExportWindowEvent_DelegateWindowChangeCoord( mDelegate );
			}
		}
		static ExportEventWindowChangeCoord mExportWindowChangeCoord =
			new ExportEventWindowChangeCoord(new ExportEventWindowChangeCoord.ExportHandle( OnExportWindowChangeCoord ));

		private static void OnExportWindowChangeCoord(
			 Window _sender )
		{
			if (_sender.mEventWindowChangeCoord != null)
				_sender.mEventWindowChangeCoord(
					 _sender );
		}

		#endregion



   		#region Event WindowButtonPressed

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWindowEvent_AdviseWindowButtonPressed( IntPtr _native, bool _advise );

		public delegate void HandleWindowButtonPressed(
			 Window _sender ,
			 string _name );
			
		private HandleWindowButtonPressed mEventWindowButtonPressed;
		public event HandleWindowButtonPressed EventWindowButtonPressed
		{
			add
			{
				if (mEventWindowButtonPressed == null) ExportWindowEvent_AdviseWindowButtonPressed( mNative, true );
				mEventWindowButtonPressed += value;
			}
			remove
			{
				mEventWindowButtonPressed -= value;
				if (mEventWindowButtonPressed == null) ExportWindowEvent_AdviseWindowButtonPressed( mNative, false );
			}
		}


		private struct ExportEventWindowButtonPressed
		{
			[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ExportWindowEvent_DelegateWindowButtonPressed( ExportHandle _delegate );
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.Interface)]  Window _sender ,
				[MarshalAs(UnmanagedType.LPStr)]  string _name );
				
			private static ExportHandle mDelegate;
			public ExportEventWindowButtonPressed( ExportHandle _delegate )
			{
				mDelegate = _delegate;
				ExportWindowEvent_DelegateWindowButtonPressed( mDelegate );
			}
		}
		static ExportEventWindowButtonPressed mExportWindowButtonPressed =
			new ExportEventWindowButtonPressed(new ExportEventWindowButtonPressed.ExportHandle( OnExportWindowButtonPressed ));

		private static void OnExportWindowButtonPressed(
			 Window _sender ,
			 string _name )
		{
			if (_sender.mEventWindowButtonPressed != null)
				_sender.mEventWindowButtonPressed(
					 _sender ,
					 _name );
		}

		#endregion



   		#region Property Snap

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportWindow_GetSnap( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWindow_SetSnap( IntPtr _widget, [MarshalAs(UnmanagedType.U1)]  bool _value );

		public bool Snap
		{
			get { return  ExportWindow_GetSnap( mNative )  ; }
			set { ExportWindow_SetSnap( mNative,  value ); }
		}

		#endregion



   


   


   


   


   


   


   		#region Method SetMaxSize

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWindow_SetMaxSize_width_height( IntPtr _native ,
			  int _width ,
			  int _height );

		public void SetMaxSize(
			int _width ,
			int _height )
		{
			ExportWindow_SetMaxSize_width_height( mNative , 
				 _width ,
				 _height );
		}

		#endregion



   		#region Property MaxSize

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportWindow_GetMaxSize( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWindow_SetMaxSize( IntPtr _widget,  ref IntSize _value );

		public IntSize MaxSize
		{
			get { return  (IntSize)Marshal.PtrToStructure(  ExportWindow_GetMaxSize( mNative )  , typeof(IntSize) )  ; }
			set { ExportWindow_SetMaxSize( mNative, ref value ); }
		}

		#endregion



   		#region Method SetMinSize

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWindow_SetMinSize_width_height( IntPtr _native ,
			  int _width ,
			  int _height );

		public void SetMinSize(
			int _width ,
			int _height )
		{
			ExportWindow_SetMinSize_width_height( mNative , 
				 _width ,
				 _height );
		}

		#endregion



   		#region Property MinSize

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportWindow_GetMinSize( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWindow_SetMinSize( IntPtr _widget,  ref IntSize _value );

		public IntSize MinSize
		{
			get { return  (IntSize)Marshal.PtrToStructure(  ExportWindow_GetMinSize( mNative )  , typeof(IntSize) )  ; }
			set { ExportWindow_SetMinSize( mNative, ref value ); }
		}

		#endregion



   


   		#region Property AutoAlpha

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportWindow_GetAutoAlpha( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWindow_SetAutoAlpha( IntPtr _widget, [MarshalAs(UnmanagedType.U1)]  bool _value );

		public bool AutoAlpha
		{
			get { return  ExportWindow_GetAutoAlpha( mNative )  ; }
			set { ExportWindow_SetAutoAlpha( mNative,  value ); }
		}

		#endregion



   


   		#region Method SetVisibleSmooth

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportWindow_SetVisibleSmooth_visible( IntPtr _native ,
			[MarshalAs(UnmanagedType.U1)]  bool _visible );

		public void SetVisibleSmooth(
			bool _visible )
		{
			ExportWindow_SetVisibleSmooth_visible( mNative , 
				 _visible );
		}

		#endregion



   


   


   

		
    }

}
