using System;
using System.Windows.Forms;
using System.Runtime.InteropServices;
using System.Text;
using System.Diagnostics;
using MyGUI.Managed;

namespace MyGUI.OgrePlatform
{
    public class InputHelper
    {

        #region Fields
        private static KeyCode[] mKeys = new KeyCode[256];
        #endregion

        #region Static constructor
        static InputHelper()
        {
            mKeys[(int)Keys.Back] = KeyCode.Backspace;
            mKeys[(int)Keys.Tab] = KeyCode.Tab;
            mKeys[(int)Keys.Return] = KeyCode.Return;
            mKeys[(int)Keys.ShiftKey] = KeyCode.LeftShift;
            mKeys[(int)Keys.ControlKey] = KeyCode.LeftControl;
            mKeys[(int)Keys.Menu] = KeyCode.LeftAlt;
            mKeys[(int)Keys.Capital] = KeyCode.Capital;
            mKeys[(int)Keys.Escape] = KeyCode.Escape;
            mKeys[(int)Keys.Space] = KeyCode.Space;
            mKeys[(int)Keys.PageUp] = KeyCode.PageUp;
            mKeys[(int)Keys.PageDown] = KeyCode.PageDown;
            mKeys[(int)Keys.End] = KeyCode.End;
            mKeys[(int)Keys.Home] = KeyCode.Home;
            mKeys[(int)Keys.Left] = KeyCode.ArrowLeft;
            mKeys[(int)Keys.Up] = KeyCode.ArrowUp;
            mKeys[(int)Keys.Right] = KeyCode.ArrowRight;
            mKeys[(int)Keys.Down] = KeyCode.ArrowDown;
            mKeys[(int)Keys.PrintScreen] = KeyCode.SysRq;
            mKeys[(int)Keys.Insert] = KeyCode.Insert;
            mKeys[(int)Keys.Delete] = KeyCode.Delete;

            mKeys[(int)Keys.D0] = KeyCode.Zero;
            mKeys[(int)Keys.D1] = KeyCode.One;
            mKeys[(int)Keys.D2] = KeyCode.Two;
            mKeys[(int)Keys.D3] = KeyCode.Three;
            mKeys[(int)Keys.D4] = KeyCode.Four;
            mKeys[(int)Keys.D5] = KeyCode.Five;
            mKeys[(int)Keys.D6] = KeyCode.Six;
            mKeys[(int)Keys.D7] = KeyCode.Seven;
            mKeys[(int)Keys.D8] = KeyCode.Eight;
            mKeys[(int)Keys.D9] = KeyCode.Nine;

            mKeys[(int)Keys.A] = KeyCode.A;
            mKeys[(int)Keys.B] = KeyCode.B;
            mKeys[(int)Keys.C] = KeyCode.C;
            mKeys[(int)Keys.D] = KeyCode.D;
            mKeys[(int)Keys.E] = KeyCode.E;
            mKeys[(int)Keys.F] = KeyCode.F;
            mKeys[(int)Keys.G] = KeyCode.G;
            mKeys[(int)Keys.H] = KeyCode.H;
            mKeys[(int)Keys.I] = KeyCode.I;
            mKeys[(int)Keys.J] = KeyCode.J;
            mKeys[(int)Keys.K] = KeyCode.K;
            mKeys[(int)Keys.L] = KeyCode.L;
            mKeys[(int)Keys.M] = KeyCode.M;
            mKeys[(int)Keys.N] = KeyCode.N;
            mKeys[(int)Keys.O] = KeyCode.O;
            mKeys[(int)Keys.P] = KeyCode.P;
            mKeys[(int)Keys.Q] = KeyCode.Q;
            mKeys[(int)Keys.R] = KeyCode.R;
            mKeys[(int)Keys.S] = KeyCode.S;
            mKeys[(int)Keys.T] = KeyCode.T;
            mKeys[(int)Keys.U] = KeyCode.U;
            mKeys[(int)Keys.V] = KeyCode.V;
            mKeys[(int)Keys.W] = KeyCode.W;
            mKeys[(int)Keys.X] = KeyCode.X;
            mKeys[(int)Keys.Y] = KeyCode.Y;
            mKeys[(int)Keys.Z] = KeyCode.Z;

            mKeys[(int)Keys.LWin] = KeyCode.LeftWindows;
            mKeys[(int)Keys.RWin] = KeyCode.RightWindows;
            mKeys[(int)Keys.Apps] = KeyCode.AppMenu;
            mKeys[(int)Keys.Sleep] = KeyCode.Sleep;

            mKeys[(int)Keys.NumPad0] = KeyCode.Numpad0;
            mKeys[(int)Keys.NumPad1] = KeyCode.Numpad1;
            mKeys[(int)Keys.NumPad2] = KeyCode.Numpad2;
            mKeys[(int)Keys.NumPad3] = KeyCode.Numpad3;
            mKeys[(int)Keys.NumPad4] = KeyCode.Numpad4;
            mKeys[(int)Keys.NumPad5] = KeyCode.Numpad5;
            mKeys[(int)Keys.NumPad6] = KeyCode.Numpad6;
            mKeys[(int)Keys.NumPad7] = KeyCode.Numpad7;
            mKeys[(int)Keys.NumPad8] = KeyCode.Numpad8;
            mKeys[(int)Keys.NumPad9] = KeyCode.Numpad9;

            mKeys[(int)Keys.Multiply] = KeyCode.Multiply;
            mKeys[(int)Keys.Add] = KeyCode.Add;
            mKeys[(int)Keys.Subtract] = KeyCode.Subtract;
            mKeys[(int)Keys.Decimal] = KeyCode.Decimal;
            mKeys[(int)Keys.Divide] = KeyCode.Divide;

            mKeys[(int)Keys.F1] = KeyCode.F1;
            mKeys[(int)Keys.F2] = KeyCode.F2;
            mKeys[(int)Keys.F3] = KeyCode.F3;
            mKeys[(int)Keys.F4] = KeyCode.F4;
            mKeys[(int)Keys.F5] = KeyCode.F5;
            mKeys[(int)Keys.F6] = KeyCode.F6;
            mKeys[(int)Keys.F7] = KeyCode.F7;
            mKeys[(int)Keys.F8] = KeyCode.F8;
            mKeys[(int)Keys.F9] = KeyCode.F9;
            mKeys[(int)Keys.F10] = KeyCode.F10;
            mKeys[(int)Keys.F11] = KeyCode.F11;
            mKeys[(int)Keys.F12] = KeyCode.F12;
            mKeys[(int)Keys.F13] = KeyCode.F13;
            mKeys[(int)Keys.F14] = KeyCode.F14;
            mKeys[(int)Keys.F15] = KeyCode.F15;

            mKeys[(int)Keys.NumLock] = KeyCode.NumLock;
            mKeys[(int)Keys.LShiftKey] = KeyCode.LeftShift;
            mKeys[(int)Keys.RShiftKey] = KeyCode.RightShift;
            mKeys[(int)Keys.LControlKey] = KeyCode.LeftControl;
            mKeys[(int)Keys.RControlKey] = KeyCode.RightControl;
            mKeys[(int)Keys.LMenu] = KeyCode.LeftAlt;
            mKeys[(int)Keys.RMenu] = KeyCode.RightAlt;

            mKeys[(int)Keys.Oemtilde] = KeyCode.Grave;
            mKeys[(int)Keys.OemMinus] = KeyCode.Minus;
            mKeys[(int)Keys.Oemplus] = KeyCode.Equals;
            mKeys[(int)Keys.OemPipe] = KeyCode.Backslash;
            mKeys[(int)Keys.OemOpenBrackets] = KeyCode.LeftBracket;
            mKeys[(int)Keys.OemCloseBrackets] = KeyCode.RightBracket;
            mKeys[(int)Keys.OemSemicolon] = KeyCode.Semicolon;
            mKeys[(int)Keys.OemQuotes] = KeyCode.Apostrophe;
            mKeys[(int)Keys.Oemcomma] = KeyCode.Comma;
            mKeys[(int)Keys.OemPeriod] = KeyCode.Period;
            mKeys[(int)Keys.OemQuestion] = KeyCode.Slash;
            mKeys[(int)Keys.Scroll] = KeyCode.ScrollLock;
            mKeys[(int)Keys.Pause] = KeyCode.Pause;
        }
        #endregion

        #region Export
        [DllImport("user32.dll", CharSet = CharSet.Unicode, ExactSpelling = true)]
        private static extern int ToUnicodeEx(
            uint wVirtKey,
            uint wScanCode,
            Keys[] lpKeyState,
            StringBuilder pwszBuff,
            int cchBuff,
            uint wFlags,
            IntPtr dwhkl);

        [DllImport("user32.dll", ExactSpelling = true)]
        internal static extern IntPtr GetKeyboardLayout(uint threadId);

        [DllImport("user32.dll", ExactSpelling = true)]
        internal static extern bool GetKeyboardState(Keys[] keyStates);

        [DllImport("user32.dll", ExactSpelling = true)]
        internal static extern uint GetWindowThreadProcessId(IntPtr hwindow, out uint processId);
        #endregion

        #region Methods
        private static string CodeToString(int scanCode)
        {
            uint procId;
            uint thread = GetWindowThreadProcessId(Process.GetCurrentProcess().MainWindowHandle, out procId);
            IntPtr hkl = GetKeyboardLayout(thread);

            if (hkl == IntPtr.Zero)
            {
                Console.WriteLine("Sorry, that keyboard does not seem to be valid.");
                return string.Empty;
            }

            Keys[] keyStates = new Keys[256];
            if (!GetKeyboardState(keyStates))
                return string.Empty;

            StringBuilder sb = new StringBuilder(10);
            int rc = ToUnicodeEx((uint)scanCode, (uint)scanCode, keyStates, sb, sb.Capacity, 0, hkl);
            return sb.ToString();
        }

        public static uint VirtualKeyToChar(Keys _virtualKey)
        {
            string result = CodeToString((int)_virtualKey);
            return (uint)(result.Length > 0 ? result[0] : 0);
        }

        public static KeyCode VirtualKeyToScanCode(Keys _virtualKey)
        {
            int value = (int)_virtualKey;
            if (value < mKeys.Length)
            {
                KeyCode result = mKeys[value];
                return result;
            }
            return KeyCode.None;
        }

        public static MouseButton MouseFlagToMouseButton(MouseButtons _button)
        {
            if (_button == MouseButtons.Left)
                return MouseButton.Left;
            else if (_button == MouseButtons.Right)
                return MouseButton.Right;
            else if (_button == MouseButtons.Middle)
                return MouseButton.Middle;

            return MouseButton.None;
        }
        #endregion

    }
}
