using System;
using System.Drawing;
using System.Globalization;

namespace Common
{
    /// <summary>
    /// Конвертор различных типов в строку и из строки
    /// </summary>
    public static class StringConverter
    {
        /// <summary>
        /// Размер в строку
        /// </summary>
        /// <param name="_size">конвертируемая структура</param>
        /// <returns>строковое представление</returns>
        public static string SizeToString(Size _size)
        {
            return _size.Width + " " + _size.Height;
        }

        /// <summary>
        /// Позицию в строку
        /// </summary>
        /// <param name="_location">конвертируемая структура</param>
        /// <returns>строковое представление</returns>
        public static string LocationToString(Point _location)
        {
            return _location.X + " " + _location.Y;
        }

        /// <summary>
        /// Строку в цвет
        /// </summary>
        /// <param name="_str">строковое представление</param>
        /// <returns>полученное значение цвта</returns>
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

        /// <summary>
        /// Цвет в строку
        /// </summary>
        /// <param name="_val">значение цвета</param>
        /// <returns>строковое представление</returns>
        public static string ColorToString(Color _val)
        {
            string str = _val.ToArgb().ToString("X");
            if (str.Length > 6) str = str.Substring(str.Length - 6);
            return str;
        }
    }
}
