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

    public  class RenderBox : Widget
    {

        #region RenderBox

        public RenderBox()
            : base()
        {
        }

        internal RenderBox(BaseWidget _parent, IntPtr _widget)
            : base(_parent, _widget)
        {
        }

        internal RenderBox(IntPtr _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
            : base(_parent, _style, _skin, _coord, _align, _layer, _name)
        {
        }

        protected override string GetWidgetType() { return "RenderBox"; }

        internal static BaseWidget RequestWrapRenderBox(BaseWidget _parent, IntPtr _widget)
        {
            return new RenderBox(_parent, _widget);
        }

        internal static BaseWidget RequestCreateRenderBox(IntPtr _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
        {
            return new RenderBox(_parent, _style, _skin, _coord, _align, _layer, _name);
        }
        
		#endregion
	
		
		//InsertPoint

   		#region Event UpdateViewport

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportRenderBoxEvent_AdviseUpdateViewport( IntPtr _native, bool _advise );

		public delegate void HandleUpdateViewport(
			 RenderBox _sender );
			
		private HandleUpdateViewport mEventUpdateViewport;
		public event HandleUpdateViewport EventUpdateViewport
		{
			add
			{
				if (mEventUpdateViewport == null) ExportRenderBoxEvent_AdviseUpdateViewport( mNative, true );
				mEventUpdateViewport += value;
			}
			remove
			{
				mEventUpdateViewport -= value;
				if (mEventUpdateViewport == null) ExportRenderBoxEvent_AdviseUpdateViewport( mNative, false );
			}
		}


		private struct ExportEventUpdateViewport
		{
			[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
			private static extern void ExportRenderBoxEvent_DelegateUpdateViewport( ExportHandle _delegate );
			public delegate void ExportHandle(
				[MarshalAs(UnmanagedType.Interface)]  RenderBox _sender );
				
			private static ExportHandle mDelegate;
			public ExportEventUpdateViewport( ExportHandle _delegate )
			{
				mDelegate = _delegate;
				ExportRenderBoxEvent_DelegateUpdateViewport( mDelegate );
			}
		}
		static ExportEventUpdateViewport mExportUpdateViewport =
			new ExportEventUpdateViewport(new ExportEventUpdateViewport.ExportHandle( OnExportUpdateViewport ));

		private static void OnExportUpdateViewport(
			 RenderBox _sender )
		{
			if (_sender.mEventUpdateViewport != null)
				_sender.mEventUpdateViewport(
					 _sender );
		}

		#endregion



   		#region Property BackgroundColour

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        
		private static extern IntPtr ExportRenderBox_GetBackgroundColour( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportRenderBox_SetBackgroundColour( IntPtr _widget, [In] ref Colour _value );

		public Colour BackgroundColour
		{
			get { return  (Colour)Marshal.PtrToStructure(  ExportRenderBox_GetBackgroundColour( mNative )  , typeof(Colour) )  ; }
			set { ExportRenderBox_SetBackgroundColour( mNative, ref value ); }
		}

		#endregion



   


   


   


   


   


   


   


   


   


   

		
    }

}
