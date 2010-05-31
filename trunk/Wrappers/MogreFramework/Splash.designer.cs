namespace MogreFramework
{
    partial class Splash
    {
        /// <summary>
        /// Required designer variable.
        /// </summary>
        private System.ComponentModel.IContainer components = null;

        /// <summary>
        /// Clean up any resources being used.
        /// </summary>
        /// <param name="disposing">true if managed resources should be disposed; otherwise, false.</param>
        protected override void Dispose(bool disposing)
        {
            if (disposing && (components != null))
            {
                components.Dispose();
            }
            base.Dispose(disposing);
        }

        #region Windows Form Designer generated code

        /// <summary>
        /// Required method for Designer support - do not modify
        /// the contents of this method with the code editor.
        /// </summary>
        private void InitializeComponent()
        {
            this.LoadingText = new System.Windows.Forms.Label();
            this.Progress = new System.Windows.Forms.ProgressBar();
            this.SuspendLayout();
            // 
            // LoadingText
            // 
            this.LoadingText.AutoEllipsis = true;
            this.LoadingText.BackColor = System.Drawing.Color.Transparent;
            this.LoadingText.ForeColor = System.Drawing.Color.White;
            this.LoadingText.Location = new System.Drawing.Point(9, 159);
            this.LoadingText.Name = "LoadingText";
            this.LoadingText.Size = new System.Drawing.Size(376, 13);
            this.LoadingText.TabIndex = 0;
            this.LoadingText.Text = "Loading...";
            this.LoadingText.UseWaitCursor = true;
            // 
            // Progress
            // 
            this.Progress.Location = new System.Drawing.Point(12, 175);
            this.Progress.Maximum = 8;
            this.Progress.Name = "Progress";
            this.Progress.Size = new System.Drawing.Size(373, 14);
            this.Progress.Step = 1;
            this.Progress.TabIndex = 1;
            this.Progress.UseWaitCursor = true;
            // 
            // Splash
            // 
            this.AutoScaleDimensions = new System.Drawing.SizeF(6F, 13F);
            this.AutoScaleMode = System.Windows.Forms.AutoScaleMode.Font;
            this.BackgroundImage = global::MogreFramework.Properties.Resources.Splash;
            this.BackgroundImageLayout = System.Windows.Forms.ImageLayout.Stretch;
            this.ClientSize = new System.Drawing.Size(400, 200);
            this.ControlBox = false;
            this.Controls.Add(this.Progress);
            this.Controls.Add(this.LoadingText);
            this.FormBorderStyle = System.Windows.Forms.FormBorderStyle.None;
            this.Name = "Splash";
            this.ShowIcon = false;
            this.StartPosition = System.Windows.Forms.FormStartPosition.CenterScreen;
            this.Text = "Loading...";
            this.UseWaitCursor = true;
            this.ResumeLayout(false);

        }

        #endregion

        private System.Windows.Forms.Label LoadingText;
        private System.Windows.Forms.ProgressBar Progress;
    }
}
