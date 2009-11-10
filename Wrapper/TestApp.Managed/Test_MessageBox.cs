using System;
using MyGUI.Managed;


namespace TestApp.Managed
{
    public class Test_MessageBox
    {
        public static void Test()
        {
            MessageBox box = Gui.Instance.CreateWidget<MessageBox>("Message", new IntCoord(520, 20, 100, 100), Align.Default, "Popup");
            box.EventMessageBoxResult += new MessageBox.HandleMessageBoxResult(box_EventMessageBoxResult);

            box.Caption = "Caption";
            box.SetMessageStyle(MessageBoxStyle.Ignore | MessageBoxStyle.IconWarning);
            box.SetMessageButton(MessageBoxStyle.Retry);
            box.SetMessageIcon(MessageBoxStyle.IconError);
            string layer = box.GetDefaultLayer();
            box.AddButtonName("button name1");
            box.SetMessageText("message");
            //box.EndMessage(MessageBoxStyle.Try);
        }

        static void box_EventMessageBoxResult(MessageBox _sender, MessageBoxStyle _result)
        {
            ExampleApplication.DebugOut("EventMessageBoxResult  _result=" + _result.ToString());
        }
    }
}
