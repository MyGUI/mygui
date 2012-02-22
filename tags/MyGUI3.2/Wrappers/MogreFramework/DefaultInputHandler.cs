using System;
using System.Collections.Generic;
using System.Text;
using System.Windows.Forms;
using Mogre;
using System.Drawing;

namespace MogreFramework
{
    public class DefaultInputHandler
    {
        #region Fields
        const int INTERVAL = 17;

        bool mLastFocus = false;
        bool mRotating = false;
        Point mLastLocation;
        System.Windows.Forms.Timer mTimer = new System.Windows.Forms.Timer();

        protected float mTrans = 10;
        protected float mRot = -0.2f;
        protected Vector3 mTranslate = Vector3.ZERO;
        protected OgreWindow mWindow = null;
        #endregion

        #region Constructor
        public DefaultInputHandler(OgreWindow win)
        {
            mWindow = win;

            win.KeyDown += new KeyEventHandler(HandleKeyDown);
            win.KeyUp += new KeyEventHandler(HandleKeyUp);
            win.MouseDown += new MouseEventHandler(HandleMouseDown);
            win.MouseUp += new MouseEventHandler(HandleMouseUp);
            win.Disposed += new EventHandler(win_Disposed);
            win.LostFocus += new EventHandler(win_LostFocus);
            win.GotFocus += new EventHandler(win_GotFocus);

            mTimer.Interval = INTERVAL;
            mTimer.Enabled = true;
            mTimer.Tick += new EventHandler(Timer_Tick);
        }
        #endregion

        #region Event Handlers
        void win_Disposed(object sender, EventArgs e)
        {
            mTimer.Enabled = false;
        }

        void win_GotFocus(object sender, EventArgs e)
        {
            mTimer.Enabled = true;
        }

        void win_LostFocus(object sender, EventArgs e)
        {
            mTimer.Enabled = false;
            mTranslate = Vector3.ZERO;
        }

        void Timer_Tick(object sender, EventArgs e)
        {
            if (mLastFocus)
            {
                // Perform the movement
                Point delta = Cursor.Position;
                delta.X -= mLastLocation.X;
                delta.Y -= mLastLocation.Y;
                HandleMouseMove(delta);
            }
            
            mLastLocation = Cursor.Position;
            mLastFocus = mWindow.Focused;

            if (mLastFocus)
                mWindow.Camera.Position += mWindow.Camera.Orientation * mTranslate;
        }

        private void HandleMouseMove(Point delta)
        {
            if (mRotating)
            {
                mWindow.Camera.Yaw(new Degree(delta.X * mRot));
                mWindow.Camera.Pitch(new Degree(delta.Y * mRot));
            }
        }

        protected virtual void HandleKeyUp(object sender, KeyEventArgs e)
        {
            switch (e.KeyCode)
            {
                case Keys.Up:
                case Keys.W:
                case Keys.Down:
                case Keys.S:
                    mTranslate.z = 0;
                    break;

                case Keys.Left:
                case Keys.A:
                case Keys.Right:
                case Keys.D:
                    mTranslate.x = 0;
                    break;

                case Keys.PageUp:
                case Keys.Q:
                case Keys.PageDown:
                case Keys.E:
                    mTranslate.y = 0;
                    break;
            }
        }

        protected virtual void HandleKeyDown(object sender, KeyEventArgs e)
        {
            float amount = mTrans;
            switch (e.KeyCode)
            {
                case Keys.Up:
                case Keys.W:
                    mTranslate.z = -amount;
                    break;

                case Keys.Down:
                case Keys.S:
                    mTranslate.z = amount;
                    break;

                case Keys.Left:
                case Keys.A:
                    mTranslate.x = -amount;
                    break;

                case Keys.Right:
                case Keys.D:
                    mTranslate.x = amount;
                    break;

                case Keys.PageUp:
                case Keys.Q:
                    mTranslate.y = amount;
                    break;

                case Keys.PageDown:
                case Keys.E:
                    mTranslate.y = -amount;
                    break;
            }
        }

        protected virtual void HandleMouseUp(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
                Cursor.Show();
                mRotating = false;
            }
        }

        protected virtual void HandleMouseDown(object sender, MouseEventArgs e)
        {
            if (e.Button == MouseButtons.Right)
            {
                Cursor.Hide();
                mRotating = true;
            }
        }
        #endregion
    }
}
