using System;
using System.Collections.Generic;
using System.Text;
using Mogre;
using System.Windows.Forms;
using MyGUI.OgrePlatform;
using MogreFramework;
using MyGUI.Managed;

namespace MogreApp
{
    class Program
    {
        [STAThread]
        static void Main(string[] args)
        {
            try
            {
                MyOgreWindow win = new MyOgreWindow();
                new SceneCreator(win);
                win.Go();
            }
            catch (System.Runtime.InteropServices.SEHException)
            {
                if (OgreException.IsThrown)
                    System.Windows.Forms.MessageBox.Show(OgreException.LastException.FullDescription, "An Ogre exception has occurred!");
                else
                    throw;
            }
        }
    }
    class MyOgreWindow : OgreWindow
    {
        int mWheel;
        public static bool mQuit;

        public static bool Quit
        {
            get { return mQuit; }
            set { mQuit = value; }
        }

        protected override void CreateInputHandler()
        {
            this.Root.FrameStarted += new FrameListener.FrameStartedHandler(Root_FrameStarted);
            this.KeyDown += new KeyEventHandler(KeyDownHandler);
            this.KeyUp += new KeyEventHandler(KeyUpHandler);
            this.MouseDown += new MouseEventHandler(MouseDownHandler);
            this.MouseUp += new MouseEventHandler(MouseUpHandler);
            this.MouseMove += new MouseEventHandler(MouseMoveHandler);
            this.MouseWheel += new MouseEventHandler(MyOgreWindow_MouseWheel);
        }

        bool Root_FrameStarted(FrameEvent evt)
        {
            return !MyOgreWindow.Quit;
        }

        void KeyDownHandler(object sender, KeyEventArgs e)
        {
            if (!mQuit && InputManager.Instance.InjectKeyPress(
                InputHelper.VirtualKeyToScanCode(e.KeyCode),
                InputHelper.VirtualKeyToChar(e.KeyCode)))
                return;
        }
        void KeyUpHandler(object sender, KeyEventArgs e)
        {
            if (!mQuit && InputManager.Instance.InjectKeyRelease(
                InputHelper.VirtualKeyToScanCode(e.KeyCode)))
                return;
        }
        void MouseDownHandler(object sender, MouseEventArgs e)
        {
            if (!mQuit && InputManager.Instance.InjectMousePress(
                e.X,
                e.Y,
                InputHelper.MouseFlagToMouseButton(e.Button)))
                return;
        }
        void MouseUpHandler(object sender, MouseEventArgs e)
        {
            if (!mQuit && InputManager.Instance.InjectMouseRelease(
                e.X,
                e.Y,
                InputHelper.MouseFlagToMouseButton(e.Button)))
                return;
        }
        void MouseMoveHandler(object sender, MouseEventArgs e)
        {
            if (!mQuit && InputManager.Instance.InjectMouseMove(
                e.X,
                e.Y,
                mWheel))
                return;
        }
        void MyOgreWindow_MouseWheel(object sender, MouseEventArgs e)
        {
            mWheel += e.Delta;
            if (!mQuit && InputManager.Instance.InjectMouseMove(
                e.X,
                e.Y,
                mWheel))
                return;
        }
    }

    class SceneCreator
    {
        OgreWindow mWindow;

        public SceneCreator(OgreWindow win)
        {
            win.SceneCreating += new OgreWindow.SceneEventHandler(SceneCreating);
            win.Disposed += new EventHandler(win_Disposed);
        }

        void win_Disposed(object sender, EventArgs e)
        {
            Export.DestroyGUI();
        }

        void SceneCreating(OgreWindow win)
        {
            mWindow = win;

            Export.CreateGUI();
            Export.SetRenderWindow(win.RenderWindow);
            Export.SetSceneManager(win.SceneManager);
            Export.SetActiveViewport(0);

            PointerManager.Instance.Visible = false;

            MyGUI.Managed.Button button = Gui.Instance.CreateWidget<MyGUI.Managed.Button>("Button", new IntCoord(10, 10, 100, 24), Align.Default, "Main");
            button.Caption = "Quit";
            button.EventMouseButtonClick += button_EventMouseButtonClick;
        }

        void button_EventMouseButtonClick(Widget _sender)
        {
            MyOgreWindow.Quit = true;
        }
    }
}
