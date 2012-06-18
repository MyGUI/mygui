using System;
using MyGUI.Managed;

namespace TestApp.Sharp
{
    public class Test_ProgressBar
    {
        public static void Test()
        {
            ProgressBar progress = Gui.Instance.CreateWidget<ProgressBar>("ProgressBar", new IntCoord(620, 220, 200, 16), Align.Default, "Main");

            FlowDirection direction = progress.FlowDirection;
            progress.FlowDirection = FlowDirection.RightToLeft;

            progress.ProgressRange = 100;
            progress.ProgressRange = progress.ProgressRange + 100;

            progress.ProgressPosition = 50;
            progress.ProgressPosition = progress.ProgressPosition + 1;

            progress.ProgressAutoTrack = !progress.ProgressAutoTrack;
        }
    }
}
