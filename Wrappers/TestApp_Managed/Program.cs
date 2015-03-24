using System;
using System.Collections.Generic;
using System.Text;
using MyGUI.Managed;

namespace TestApp.Managed
{
    class Program
    {
		[STAThread]
		public static void Main(string[] args)
		{
			//PoorPlatformTest();
			DefaultPlatformTest();
		}

		private static void DefaultPlatformTest()
		{
			Export.Initialise();

			Test.TestWidgets();

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

			Test.TestWidgets();

			Platform.DestroyGui();
			Platform.DestroyPlatform();
		}
	}
}
