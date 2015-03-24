using System;
using MyGUI.Managed;

namespace TestApp.Managed
{
    public class Test_EditBox
    {
        public static void Test()
        {
            EditBox box = Gui.Instance.CreateWidget<EditBox>("EditBox", new IntCoord(520, 20, 300, 30), Align.Default, "Main");

			box.EventEditSelectAccept += new EditBox.HandleEditSelectAccept(box_EventEditSelectAccept);
			box.EventEditTextChange += new EditBox.HandleEditTextChange(box_EventEditTextChange);

			box.OnlyText = "";
            box.IsVisibleHScroll = !box.IsVisibleHScroll;
            box.IsVisibleVScroll = !box.IsVisibleVScroll;
            box.TabPrinting = !box.TabPrinting;
            box.EditWordWrap = !box.EditWordWrap;
            box.EditWordWrap = !box.EditWordWrap;
            box.SetPasswordChar("*");
            uint pass = box.PasswordChar;
            box.PasswordChar = pass + 1;
            box.EditStatic = !box.EditStatic;
            box.EditStatic = !box.EditStatic;
            box.EditMultiLine = !box.EditMultiLine;
            box.EditPassword = !box.EditPassword;
            box.EditPassword = !box.EditPassword;
            box.EditReadOnly = !box.EditReadOnly;
            box.EditReadOnly = !box.EditReadOnly;
            box.AddText("this is text");
            box.EraseText(5, 3);
            box.InsertText("is not ", 5);
            box.MaxTextLength = box.MaxTextLength + 1;
            box.OverflowToTheLeft = !box.OverflowToTheLeft;
            uint length = box.TextLength;
            box.OnlyText = "#FF0000" + box.OnlyText;
            box.TextCursor = box.TextCursor - 1;
            box.SetTextSelection(0, 12);
            box.SetTextSelectionColour(new Colour(0, 1, 0));
            bool isselect = box.IsTextSelection;
            box.DeleteTextSelection();
            string text = box.GetTextInterval(2, 2);
            box.SetTextSelection(0, 2);
            length = box.TextSelectionLength;
            box.SetTextIntervalColour(3, 2, new Colour(0, 0, 1));
        }

        static void box_EventEditTextChange(EditBox _sender)
        {
            Gui.Instance.Log("TestApp", LogLevel.Info, "EventEditTextChange");
        }

        static void box_EventEditSelectAccept(EditBox _sender)
        {
            Gui.Instance.Log("TestApp", LogLevel.Info, "EventEditSelectAccept");
        }
    }
}
