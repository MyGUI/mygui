using System;
using System.Collections.Generic;
using System.Xml;
using DoxygenWrapper.Wrappers;
using System.IO;
using System.Diagnostics;

namespace DoxygenWrapper
{
	class Program
	{
		private static void Main(string[] _args)
		{
			DirectoryInfo dataFolder = new DirectoryInfo(@"..\..\..\Data");
			if (!dataFolder.Exists)
			{
				ConsoleUtility.WriteErrorLine("Folder {0} is not exist", dataFolder.FullName);
				return;
			}

			while (true)
			{
				PrintInfo(dataFolder);

				ConsoleKeyInfo key = Console.ReadKey();
				Console.WriteLine();

				if (key.Key == ConsoleKey.Escape)
				{
					break;
				}
				else if (key.Key == ConsoleKey.D1)
				{
					// будет невалидным
					mCompoundManager = null;

					ProcessStartInfo info = new ProcessStartInfo();
					info.FileName = "doxygen.exe";
					info.WorkingDirectory = @"..\..\..\doxygen";

					Process doxygen = Process.Start(info);
					doxygen.WaitForExit();
				}
				else if (key.Key == ConsoleKey.D2)
				{
					CheckCompoundManager();

					string fileName = "CompoundTree.xml";
					mCompoundManager.DumpCompoundTree(fileName);

					ConsoleUtility.WriteLine("Dump saved to {0}", fileName);
				}
				else
				{
					int offset = (int)key.Key - (int)ConsoleKey.D3;
					string fileName = GetFileByOffset(offset, dataFolder);
					if (fileName != "")
					{
						CheckCompoundManager();

						ConsoleUtility.WriteLine("Generate for {0}", fileName);

						WrapperManager wrapperManager = new WrapperManager();
						string file = Path.Combine(dataFolder.FullName, fileName);
						wrapperManager.Initialise(file);

						ConsoleUtility.WriteLine("Generate complite");
					}
					else
					{
						ConsoleUtility.WriteErrorLine("Error file found");
					}
				}

				Console.WriteLine();
			}
		}

		private static void CheckCompoundManager()
		{
			if (mCompoundManager == null)
			{
				ConsoleUtility.WriteLine("Initialise compound manager");
				mCompoundManager = new CompoundManager();
				mCompoundManager.SetIgnoredNameList(new string[] { "MyGUI::Allocator", "MyGUI::Allocator::rebind" });
				mCompoundManager.Initialise(@"..\..\..\doxygen\xml");
			}
		}

		private static string GetFileByOffset(int _offset, DirectoryInfo _dataFolder)
		{
			int index = 0;
			foreach (var file in _dataFolder.GetFiles("*.xml", SearchOption.TopDirectoryOnly))
			{
				if (index == _offset)
					return file.Name;
				index++;
			}
			return "";
		}

		private static void PrintInfo(DirectoryInfo _dataFolder)
		{
			Console.WriteLine();
			Console.WriteLine("Press key :");
			Console.WriteLine("Escape - Exit");
			Console.WriteLine("1 - Generate doxygen");
			Console.WriteLine("2 - Doxygen compound dump");

			int index = 3;
			foreach (var file in _dataFolder.GetFiles("*.xml", SearchOption.TopDirectoryOnly))
			{
				ConsoleUtility.WriteLine("{0} - {1}", index, file.Name);
				index++;
			}

			Console.WriteLine();
		}

		private static CompoundManager mCompoundManager = null;
	}
}
