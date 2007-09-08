#pragma once

#include <vector>
#include <string>

namespace widget
{

	struct util
	{
		static const std::vector<std::string> & split( const std::string& _value, const std::string & _delims = "\t\n ")
		{
			static std::vector<std::string> ret;
			ret.clear();

			size_t start=0, pos;
			do {
				pos = _value.find_first_of(_delims, start);
				if (pos == start) {
					// Do nothing
					start = pos + 1;
				} else if (pos == std::string::npos) {
					// Copy the rest of the string
					ret.push_back( _value.substr(start) );
					break;
				} else {
					// Copy up to delimiter
					ret.push_back( _value.substr(start, pos - start) );
					start = pos + 1;
				}
				// parse up to next real data
				start = _value.find_first_not_of(_delims, start);

			} while (pos != std::string::npos);

			return ret;
		}
	};

	template< class T >
	T parseValue( const std::string& _value )
	{
		std::istringstream str(_value);
		T ret = 0;
		str >> ret;
        return ret;
	}

	inline int parseInt(const std::string& _value) {return parseValue<int>(_value);}
	inline float parseFloat(const std::string& _value) {return parseValue<float>(_value);}


	template< class T >
	std::string toString (T val)
	{
        std::ostringstream stream;
        stream << val;
        return stream.str();
	}

	inline std::string toString(float _value) {return toString<float>(_value);}
	inline std::string toString(double _value) {return toString<double>(_value);}
	inline std::string toString(char _value) {return toString<char>(_value);}
	inline std::string toString(unsigned char _value) {return toString<unsigned char>(_value);}
	inline std::string toString(wchar_t _value) {return toString<wchar_t>(_value);}
	inline std::string toString(short _value) {return toString<short>(_value);}
	inline std::string toString(unsigned short _value) {return toString<unsigned short>(_value);}
	inline std::string toString(int _value) {return toString<int>(_value);}
	inline std::string toString(unsigned int _value) {return toString<unsigned int>(_value);}
	inline std::string toString (const std::string & val)	{return val;}

	template< class T1,  class T2 >
	std::string toString (T1 p1, T2 p2)
	{
		return toString(p1) + toString(p2);
	}

	template< class T1,  class T2,  class T3 >
	std::string toString (T1 p1, T2 p2, T3 p3)
	{
		return toString(p1) + toString(p2) + toString(p3);
	}

	template< class T1,  class T2,  class T3, class T4 >
	std::string toString (T1 p1, T2 p2, T3 p3, T4 p4)
	{
		return toString(p1) + toString(p2) + toString(p3) + toString(p4);
	}

	template< class T1,  class T2,  class T3, class T4, class T5 >
	std::string toString (T1 p1, T2 p2, T3 p3, T4 p4, T5 p5)
	{
		return toString(p1) + toString(p2) + toString(p3) + toString(p4) + toString(p5);
	}

	template< class T1,  class T2,  class T3, class T4, class T5, class T6 >
	std::string toString (T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6)
	{
		return toString(p1) + toString(p2) + toString(p3) + toString(p4) + toString(p5) + toString(p6);
	}

	template< class T1,  class T2,  class T3, class T4, class T5, class T6, class T7 >
	std::string toString (T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7)
	{
		return toString(p1) + toString(p2) + toString(p3) + toString(p4) + toString(p5) + toString(p6) + toString(p7);
	}

}