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

    public  class DDContainer : Widget
    {

        #region DDContainer

        public DDContainer()
            : base()
        {
        }

        internal DDContainer(BaseWidget _parent, IntPtr _widget)
            : base(_parent, _widget)
        {
        }

        internal DDContainer(IntPtr _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
            : base(_parent, _style, _skin, _coord, _align, _layer, _name)
        {
        }

        protected override string GetWidgetType() { return "DDContainer"; }

        internal static BaseWidget RequestWrapDDContainer(BaseWidget _parent, IntPtr _widget)
        {
            return new DDContainer(_parent, _widget);
        }

        internal static BaseWidget RequestCreateDDContainer(IntPtr _parent, WidgetStyle _style, string _skin, IntCoord _coord, Align _align, string _layer, string _name)
        {
            return new DDContainer(_parent, _style, _skin, _coord, _align, _layer, _name);
        }
        
		#endregion
	
		
		//InsertPoint















   


   		#region Property NeedDragDrop

		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
        [return: MarshalAs(UnmanagedType.U1)]
		private static extern bool ExportDDContainer_GetNeedDragDrop( IntPtr _widget );
		[DllImport("MyGUI.Export.dll", CallingConvention = CallingConvention.Cdecl)]
		private static extern void ExportDDContainer_SetNeedDragDrop( IntPtr _widget, [MarshalAs(UnmanagedType.U1)]  bool _value );

		public bool NeedDragDrop
		{
			get { return  ExportDDContainer_GetNeedDragDrop( mNative )  ; }
			set { ExportDDContainer_SetNeedDragDrop( mNative,  value ); }
		}

		#endregion



   


   


   

		
    }

}
