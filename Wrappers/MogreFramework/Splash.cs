using System;
using System.Collections.Generic;
using System.ComponentModel;
using System.Data;
using System.Drawing;
using System.Text;
using System.Windows.Forms;

namespace MogreFramework
{
    internal partial class Splash : Form
    {
        #region Constructor
        /// <summary>
        /// Constructor.
        /// </summary>
        public Splash()
        {
            InitializeComponent();
        }
        #endregion

        #region Public Methods
        /// <summary>
        /// Increments the progress bar and sets the string to be displayed.
        /// </summary>
        /// <param name="text"></param>
        public void Increment(string text)
        {
            Progress.Increment(1);
            LoadingText.Text = text;

            Update();
            Application.DoEvents();
        }

        /// <summary>
        /// Does the same as Form.Show, except that it pumps the event queue once after
        /// showing the dialog.
        /// </summary>
        public new void Show()
        {
            base.Show();
            Application.DoEvents();
        }
        #endregion
    }
}
