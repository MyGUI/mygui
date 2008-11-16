/*!
    @file
    @author     Albert Semenov
    @date       08/2008
    @module
*/
#include "DemoKeeper.h"

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <assert.h>
#include <assert.h>

#include <gcroot.h>



namespace demo
{

	struct Param
	{
		Param(int _value)
		{
			value = _value;
		}
		int value;
	};

	/*value struct Param2
	{
		Param2(Param _value)
		{
			value = _value.value; 
		}
		static operator Param (Param2 _value)
		{
			return Param(_value.value);
		}

		int value;
	};*/


	delegate void HandleDelegate(Param _value);

	template <typename T>
	class Base
	{
	public:
		typedef void (*Func)(T);

		void Initialise(System::MulticastDelegate ^ _delegate)
		{
			m_delegate = _delegate;
		}

		void Invoke(T p1)
		{
			System::MulticastDelegate^ invoke = (System::MulticastDelegate^)m_delegate;
			System::Type^ type = invoke->GetType();
			System::Reflection::MethodInfo^ mInfo = type->GetMethod("Invoke", gcnew cli::array<System::Type^> {T::typeid });
			Func foo = (Func)System::Runtime::InteropServices::Marshal::GetFunctionPointerForDelegate(invoke).ToPointer();
			foo(p1);
			//mInfo->Invoke(invoke, gcnew cli::array<System::Object^, 1> { (System::Object^)Param(p1) });
		}

	private:
		gcroot<System::MulticastDelegate^> m_delegate;
	};


	/*template <typename T>
	class Test
	{
	public:
		System::MulticastDelegate ^ m_delegate;
	};*/

	value struct Export
	{

		static void foo(Param _value)
		{
			int test = 0;
		}

	};

	void test()
	{
	}

    void DemoKeeper::createScene()
    {
		MyGUI::helper::addResourceLocation("D:/MyGUI_Source/trunk/Media/TestApp", "FileSystem", "General", false, false);
		MyGUI::helper::addResourceLocation("D:/MyGUI_Source/trunk/Media/TestApp/2.zip", "Zip", "General", false, false);

		Base<Param> base;
		base.Initialise(gcnew HandleDelegate(Export::foo));

		base.Invoke(Param(101));
		
		MyGUI::delegates::CDelegate0 eventTest;

		eventTest = MyGUI::newDelegate(test);
		eventTest();
		
	}
 
    void DemoKeeper::destroyScene()
    {
    }

	 
} // namespace demo
