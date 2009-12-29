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

    public  class VScrollBar : Widget
    {

        #region VScrollBar

        public VScrollBar()
            : base()
        {
        }

        internal VScrollBar(BaseWidget _parent, IntPtr _widget)
            : base(_parent, _widget)
        {
        }

        internal VScrollBar(IntPtr _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
            : base(_parent, _style, _skin, _coord, _align, _layer, _name)
        {
        }

        protected override string GetWidgetType() { return "VScroll"; }

        internal static BaseWidget RequestWrapVScrollBar(BaseWidget _parent, IntPtr _widget)
        {
            return new VScrollBar(_parent, _widget);
        }

        internal static BaseWidget RequestCreateVScrollBar(IntPtr _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
        {
            return new VScrollBar(_parent, _style, _skin, _coord, _align, _layer, _name);
        }
        
		#endregion
	
		
		//InsertPoint

   		#region Event ScrollChangePosition

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportVScrollBarEvent_AdviseScrollChangePosition( IntPtr _native, bool _advise );

		public delegate void HandleScrollChangePosition(
			 VScrollBar _sender ,
			 uint _position );
			
		private HandleScrollChangePosition mEventScrollChangePosition;
		public event HandleScrollChangePosition EventScrollChangePosition
		{
			add
			{
				if (mEventScrollChangePosition == null) ExportVScrollBarEvent_AdviseScrollChangePosition( mNative, true );
				mEventScrollChangePosition += value;
			}
			remove
			{
				mEventScrollChangePosition -= value;
				if (mEventScrollChangePosition == null) ExportVScrollBarEvent_AdviseScrollChangePosition( mNative, false );
			}
		}


		private struct ExportEventScrollChangePosition
		{
			[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ExportVScrollBarEvent_DelegateScrollChangePosition( ExportHandle _delegate );
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.Interface)]  VScrollBar _sender ,
				  uint _position );
				
			private static ExportHandle mDelegate;
			public ExportEventScrollChangePosition( ExportHandle _delegate )
			{
				mDelegate = _delegate;
				ExportVScrollBarEvent_DelegateScrollChangePosition( mDelegate );
			}
		}
		static ExportEventScrollChangePosition mExportScrollChangePosition =
			new ExportEventScrollChangePosition(new ExportEventScrollChangePosition.ExportHandle( OnExportScrollChangePosition ));

		private static void OnExportScrollChangePosition(
			 VScrollBar _sender ,
			 uint _position )
		{
			if (_sender.mEventScrollChangePosition != null)
				_sender.mEventScrollChangePosition(
					 _sender ,
					 _position );
		}

		#endregion



   


   


   


   


   


   


   


   		#region Property MoveToClick

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportVScrollBar_GetMoveToClick( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportVScrollBar_SetMoveToClick( IntPtr _widget, [MarshalAs(UnmanagedType.U1)]  bool _value );

		public bool MoveToClick
		{
			get { return  ExportVScrollBar_GetMoveToClick( mNative )  ; }
			set { ExportVScrollBar_SetMoveToClick( mNative,  value ); }
		}

		#endregion



   		#region Property MinTrackSize

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportVScrollBar_GetMinTrackSize( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportVScrollBar_SetMinTrackSize( IntPtr _widget,   int _value );

		public int MinTrackSize
		{
			get { return  ExportVScrollBar_GetMinTrackSize( mNative )  ; }
			set { ExportVScrollBar_SetMinTrackSize( mNative,  value ); }
		}

		#endregion



   		#region Property TrackSize

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportVScrollBar_GetTrackSize( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportVScrollBar_SetTrackSize( IntPtr _widget,   int _value );

		public int TrackSize
		{
			get { return  ExportVScrollBar_GetTrackSize( mNative )  ; }
			set { ExportVScrollBar_SetTrackSize( mNative,  value ); }
		}

		#endregion



   		#region Method GetLineSize

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern int ExportVScrollBar_GetLineSize( IntPtr _native );

		public int GetLineSize( )
		{
			return  ExportVScrollBar_GetLineSize( mNative )  ;
		}

		#endregion



   		#region Property ScrollViewPage

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportVScrollBar_GetScrollViewPage( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportVScrollBar_SetScrollViewPage( IntPtr _widget,   uint _value );

		public uint ScrollViewPage
		{
			get { return  ExportVScrollBar_GetScrollViewPage( mNative )  ; }
			set { ExportVScrollBar_SetScrollViewPage( mNative,  value ); }
		}

		#endregion



   		#region Property ScrollPage

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportVScrollBar_GetScrollPage( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportVScrollBar_SetScrollPage( IntPtr _widget,   uint _value );

		public uint ScrollPage
		{
			get { return  ExportVScrollBar_GetScrollPage( mNative )  ; }
			set { ExportVScrollBar_SetScrollPage( mNative,  value ); }
		}

		#endregion



   		#region Property ScrollPosition

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportVScrollBar_GetScrollPosition( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportVScrollBar_SetScrollPosition( IntPtr _widget,   uint _value );

		public uint ScrollPosition
		{
			get { return  ExportVScrollBar_GetScrollPosition( mNative )  ; }
			set { ExportVScrollBar_SetScrollPosition( mNative,  value ); }
		}

		#endregion



   		#region Property ScrollRange

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern uint ExportVScrollBar_GetScrollRange( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportVScrollBar_SetScrollRange( IntPtr _widget,   uint _value );

		public uint ScrollRange
		{
			get { return  ExportVScrollBar_GetScrollRange( mNative )  ; }
			set { ExportVScrollBar_SetScrollRange( mNative,  value ); }
		}

		#endregion



   


   


   

		
    }

}
