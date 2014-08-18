using System;
using System.Collections.Generic;
using System.Text;
using System.Diagnostics;

namespace DoxygenWrapper.Wrappers
{
	public class ConsoleUtility
	{
		public static void WriteLine(string _value)
		{
			Console.WriteLine(_value);
		}

		public static void WriteLine(object _value)
		{
			Console.WriteLine(_value.ToString());
		}

		public static void WriteLine(string _value, params object[] _params)
		{
			Console.WriteLine(string.Format(_value, _params));
		}

		public static void WriteErrorLine(string _value)
		{
			Console.ForegroundColor = ConsoleColor.Yellow;
			Console.WriteLine(_value);
			Console.WriteLine();
			Console.ResetColor();
		}

		public static void WriteErrorLine(object _value)
		{
			Console.ForegroundColor = ConsoleColor.Yellow;
			Console.WriteLine(_value.ToString());
			Console.WriteLine();
			Console.ResetColor();
		}

		public static void WriteErrorLine(string _value, params object[] _params)
		{
			Console.ForegroundColor = ConsoleColor.Yellow;
			Console.WriteLine(string.Format(_value, _params));
			Console.WriteLine();
			Console.ResetColor();
		}
	}
}
