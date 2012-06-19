using System;
using System.Collections.Generic;
using System.Text;
using MyGUI.Sharp;
using MyGUI.Sharp.Demo;

namespace TestApp.Sharp
{
    class Program
    {
		[STAThread]
        public static void Main(string[] args)
        {
            Gui.Instance.ExportTest();

			//PoorPlatformTest();
			DefaultPlatformTest();
        }

		private static void DefaultPlatformTest()
		{
			Export.Initialise();

			TestWidgets();

			float time = 0;
			Export.AddFrameDelegate(delegate(float _time)
			{
				time += _time;
				if (time < 1)
					return;
				time = 0;

				Test_Gui.Update();
			});
			Export.Run();
		}

		private static void PoorPlatformTest()
		{
			Platform.CreatePlatform("MyGUI.log");
			Platform.CreateGui("MyGUICore.xml");

			TestWidgets();

			Platform.DestroyGui();
			Platform.DestroyPlatform();
		}

		private static void TestWidgets()
		{
			Test_Button.Test();
			Test_ComboBox.Test();
			Test_DDContainer.Test();
			Test_EditBox.Test();
			Test_ItemBox.Test();
			Test_ListBox.Test();
			Test_MenuBar.Test();
			Test_MenuControl.Test();
			Test_MultiListBox.Test();
			Test_PopupMenu.Test();
			Test_ProgressBar.Test();
			Test_ScrollView.Test();
			Test_ImageBox.Test();
			Test_TextBox.Test();
			Test_TabControl.Test();
			Test_ScrollBar.Test();
			Test_Widget.Test();
			Test_Window.Test();

			Test_Gui.Test();
		}
    }
}
