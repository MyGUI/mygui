/*!
    @file
    @author     Albert Semenov
    @date       08/2008
    @module
*/
#include "DemoKeeper.h"

	template<typename T>
	class Enumerator
	{
	private:
		Enumerator() { }

	public:
		explicit Enumerator(const T & _container) :
			m_first(true),
			m_current(_container.begin()),
			m_end(_container.end())
		{
		}

		Enumerator(typename T::const_iterator _first, typename T::const_iterator _end) :
			m_first(true),
			m_current(_first),
			m_end(_end)
		{
		}

		bool next()
		{
			if (m_current == m_end) return false;
			else if (m_first) {
				m_first = false;
				return true;
			}
			++ m_current;
			if (m_current == m_end) return false;
			return true;
		}

		typename T::const_reference operator->() const { assert(m_current != m_end); return (*m_current); }
		typename T::const_reference current() { assert(m_current != m_end); return (*m_current); }

	private:
		typename T::const_iterator m_current;
		typename T::const_iterator m_end;
		bool m_first;
	};

class Class
{
public:
	int test() { return 11; }
	int const_test() const { return 12; }
};

namespace demo
{

    void DemoKeeper::createScene()
    {
		MyGUI::helper::addResourceLocation("D:/MyGUI_Source/trunk/Media/TestApp", "FileSystem", "General", false, false);

		typedef Class * ClassPtr;
		typedef std::vector<Class> VectorClass;
		typedef std::vector<ClassPtr> VectorClassPtr;

		typedef Enumerator<VectorClass> EnumeratorClass;
		typedef Enumerator<VectorClassPtr> EnumeratorClassPtr;

		VectorClass vector1;
		VectorClassPtr vector2;

		Class value;
		vector1.push_back(value);
		vector2.push_back(&value);

		EnumeratorClass enum1(vector1);
		EnumeratorClassPtr enum2(vector2);

		while (enum1.next()) {
			//int test = enum1.current().test();
			int const_test = enum1.current().const_test();
		};

		while (enum2.next()) {
			int test = enum2.current()->test();
			int const_test = enum2.current()->const_test();
		};


		typedef std::vector<std::string> VectorString;
		typedef Enumerator<VectorString> EnumeratorVectorString;
		VectorString vec;
		vec.push_back("value");
		//EnumeratorVectorString enum_vec(vec.begin(), vec.end());
		EnumeratorVectorString enum_vec(vec);
		while (enum_vec.next()) {
			std::string value = enum_vec.current();
		};

	}
 
    void DemoKeeper::destroyScene()
    {
    }

	 
} // namespace demo
