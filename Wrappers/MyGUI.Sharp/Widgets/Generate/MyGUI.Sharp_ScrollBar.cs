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

    public  class ScrollBar : Widget
    {

        #region ScrollBar

        protected override string GetWidgetType() { return "ScrollBar"; }

        internal static BaseWidget RequestWrapScrollBar(BaseWidget _parent, IntPtr _widget)
        {
			ScrollBar widget = new ScrollBar();
			widget.WrapWidget(_parent, _widget);
            return widget;
        }

        internal static BaseWidget RequestCreateScrollBar(BaseWidget _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
        {
			ScrollBar widget = new ScrollBar();
			widget.CreateWidget(_parent, _style, _skin, _coord, _align, _layer, _name);
            return widget;
        }
        
		#endregion
	
		
		//InsertPoint

   		#region Event ScrollChangePosition

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportScrollBarEvent_AdviseScrollChangePosition( IntPtr _native, bool _advise );

		public delegate void HandleScrollChangePosition(
			 ScrollBar _sender ,
			 uint _position );
			
		private HandleScrollChangePosition mEventScrollChangePosition;
		public event HandleScrollChangePosition EventScrollChangePosition
		{
			add
			{
				if (mEventScrollChangePosition == null) ExportScrollBarEvent_AdviseScrollChangePosition( mNative, true );
				mEventScrollChangePosition += value;
			}
			remove
			{
				mEventScrollChangePosition -= value;
				if (mEventScrollChangePosition == null) ExportScrollBarEvent_AdviseScrollChangePosition( mNative, false );
			}
		}


		private struct ExportEventScrollChangePosition
		{
			[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ExportScrollBarEvent_DelegateScrollChangePosition( ExportHandle _delegate );
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.CustomMarshaler, MarshalTypeRef = typeof(InterfaceMarshaler))]  ScrollBar _sender ,
				  uint _position );
				
			private static ExportHandle mDelegate;
			public ExportEventScrollChangePosition( ExportHandle _delegate )
			{
				mDelegate = _delegate;
				ExportScrollBarEvent_DelegateScrollChangePosition( mDelegate );
			}
		}
		static ExportEventScrollChangePosition mExportScrollChangePosition =
			new ExportEventScrollChangePosition(new ExportEventScrollChangePosition.ExportHandle( OnExportScrollChangePosition ));

		private static void OnExportScrollChangePosition(
			 ScrollBar _sender ,
			 uint _position )
		{
			if (_sender.mEventScrollChangePosition != null)
				_sender.mEventScrollChangePosition(
					 _sender ,
					 _position );
		}

		#endregion



   


   


   


   


   


   


   		#region Property MoveToClick

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportScrollBar_GetMoveToClick( IntPtr _widget );
		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportScrollBar_SetMoveToClick( IntPtr _widget, [MarshalAs(UnmanagedType.U1)]  bool _value );

		public bool MoveToClick
		{
			get { return  ExportScrollBar_GetMoveToClick( mNative )  ; }
			set { ExportScrollBar_SetMoveToClick( mNative,  value ); }
		}

		#endregion



   		#region Property MinTrackSize

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportScrollBar_GetMinTrackSize( IntPtr _widget );
		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportScrollBar_SetMinTrackSize( IntPtr _widget,   int _value );

		public int MinTrackSize
		{
			get { return  ExportScrollBar_GetMinTrackSize( mNative )  ; }
			set { ExportScrollBar_SetMinTrackSize( mNative,  value ); }
		}

		#endregion



   		#region Property TrackSize

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportScrollBar_GetTrackSize( IntPtr _widget );
		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportScrollBar_SetTrackSize( IntPtr _widget,   int _value );

		public int TrackSize
		{
			get { return  ExportScrollBar_GetTrackSize( mNative )  ; }
			set { ExportScrollBar_SetTrackSize( mNative,  value ); }
		}

		#endregion



   		#region Method GetLineSize

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportScrollBar_GetLineSize( IntPtr _native );

		public int GetLineSize( )
		{
			return  ExportScrollBar_GetLineSize( mNative )  ;
		}

		#endregion



   		#region Property ScrollViewPage

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportScrollBar_GetScrollViewPage( IntPtr _widget );
		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportScrollBar_SetScrollViewPage( IntPtr _widget,   uint _value );

		public uint ScrollViewPage
		{
			get { return  ExportScrollBar_GetScrollViewPage( mNative )  ; }
			set { ExportScrollBar_SetScrollViewPage( mNative,  value ); }
		}

		#endregion



   		#region Property ScrollPage

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportScrollBar_GetScrollPage( IntPtr _widget );
		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportScrollBar_SetScrollPage( IntPtr _widget,   uint _value );

		public uint ScrollPage
		{
			get { return  ExportScrollBar_GetScrollPage( mNative )  ; }
			set { ExportScrollBar_SetScrollPage( mNative,  value ); }
		}

		#endregion



   		#region Property ScrollPosition

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportScrollBar_GetScrollPosition( IntPtr _widget );
		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportScrollBar_SetScrollPosition( IntPtr _widget,   uint _value );

		public uint ScrollPosition
		{
			get { return  ExportScrollBar_GetScrollPosition( mNative )  ; }
			set { ExportScrollBar_SetScrollPosition( mNative,  value ); }
		}

		#endregion



   		#region Property ScrollRange

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportScrollBar_GetScrollRange( IntPtr _widget );
		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportScrollBar_SetScrollRange( IntPtr _widget,   uint _value );

		public uint ScrollRange
		{
			get { return  ExportScrollBar_GetScrollRange( mNative )  ; }
			set { ExportScrollBar_SetScrollRange( mNative,  value ); }
		}

		#endregion



   		#region Property VerticalAlignment

		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportScrollBar_GetVerticalAlignment( IntPtr _widget );
		[DllImport("MyGUI_Export", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportScrollBar_SetVerticalAlignment( IntPtr _widget, [MarshalAs(UnmanagedType.U1)]  bool _value );

		public bool VerticalAlignment
		{
			get { return  ExportScrollBar_GetVerticalAlignment( mNative )  ; }
			set { ExportScrollBar_SetVerticalAlignment( mNative,  value ); }
		}

		#endregion



   


   


   

		
    }

}
