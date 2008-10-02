
using System;
using Common;

namespace WidgetSkinEditor
{
    class Converter
    {
        public static Align StringToAlign(string _val)
        {
            try
            {
                string[] aligns = _val.Split(' ');
                Align a = Align.DEFAULT;
                for (int i = 0; i < aligns.Length; i++)
                {
                    string s = aligns[i].Replace("ALIGN_", string.Empty);
                    a |= (Align)Enum.Parse(typeof(Align), s);
                }
                return a;
            }
            catch (Exception _e)
            {
                throw new Exception("ошибка при определении выравнивания :'" + _val + "'", _e);
            }
        }

        public static string AlignToString(Align _val)
        {
            if(_val == Align.DEFAULT) return string.Empty;
            string al = string.Empty;
            string[] sligns = _val.ToString().Split(new string[]{", "} , StringSplitOptions.RemoveEmptyEntries);
            foreach (string s in sligns)
                if(s != Align.DEFAULT.ToString()) al += "ALIGN_" + s + " ";
            
            return al.Trim();
        }
    }
}
