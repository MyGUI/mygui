using System;
using System.ComponentModel;
using System.Drawing;
using System.Globalization;
using System.Windows.Forms;
using System.Xml;
using Core;

namespace ViewLoger
{
    public partial class LogWindow : ListView
    {
        public LogWindow()
        {
            InitializeComponent();
        }

        public LogWindow(IContainer container)
        {
            container.Add(this);

            InitializeComponent();
        }

        void LogWindow_Resize(object sender, EventArgs e)
        {
            int w = ClientSize.Width / Columns.Count;
            foreach (ColumnHeader header in Columns)
                header.Width = w;
        }

        public void AddNote(int _level, string _from, string _msg)
        {
            string level = _level.ToString();
            Color fcolor = Color.FromKnownColor(KnownColor.WindowText);
            Color bgcolor = Color.FromKnownColor(KnownColor.Window);

            XmlElement settings = CoreFacade.SettingsProvider.Root.SelectSingleNode("Msg[@level='" + _level + "']") as XmlElement;
            if(settings != null)
            {
                level = settings["name"].InnerText;
                fcolor = StringToColor(settings["foreColor"].InnerText);
                bgcolor = StringToColor(settings["bgColor"].InnerText);
            }

            ListViewItem li = Items.Add(level);
            li.SubItems.Add(_from);
            li.SubItems.Add(_msg);
            li.BackColor = bgcolor;
            li.ForeColor = fcolor;
        }

        public static Color StringToColor(string _str)
        {
            Color clr = Color.Empty;
            string scolor = _str.TrimStart('#');
            if (!_str.Trim().Contains(" "))
                clr = Color.FromArgb(Int32.Parse(scolor, NumberStyles.HexNumber));
            else
            {
                string[] clrs = scolor.Split(' ');
                if (clrs.Length != 3) throw new Exception("Неправельный формат цвета '" + scolor + "'");
                clr = Color.FromArgb(255 * (int)Convert.ToSingle(clrs[0], NumberFormatInfo.InvariantInfo),
                                    255 * (int)Convert.ToSingle(clrs[1], NumberFormatInfo.InvariantInfo),
                                    255 * (int)Convert.ToSingle(clrs[2], NumberFormatInfo.InvariantInfo));
            }
            return clr;
        }
    }
}
