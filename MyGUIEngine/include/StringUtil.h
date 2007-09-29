#ifndef _STRINGUTIL_H_
#define _STRINGUTIL_H_

#include <vector>
#include <string>

namespace util
{

	// конвертирование в строку
	template< class T >
	inline std::string toString (T p)
	{
        std::ostringstream stream;
        stream << p;
        return stream.str();
	}

	template< class T1,  class T2 >
	inline std::string toString (T1 p1, T2 p2)
	{
        std::ostringstream stream;
        stream << p1 << p2;
        return stream.str();
	}

	template< class T1,  class T2,  class T3 >
	inline std::string toString (T1 p1, T2 p2, T3 p3)
	{
        std::ostringstream stream;
        stream << p1 << p2 << p3;
        return stream.str();
	}

	template< class T1,  class T2,  class T3, class T4 >
	inline std::string toString (T1 p1, T2 p2, T3 p3, T4 p4)
	{
        std::ostringstream stream;
        stream << p1 << p2 << p3 << p4;
        return stream.str();
	}

	template< class T1,  class T2,  class T3, class T4, class T5 >
	inline std::string toString (T1 p1, T2 p2, T3 p3, T4 p4, T5 p5)
	{
        std::ostringstream stream;
        stream << p1 << p2 << p3 << p4 << p5;
        return stream.str();
	}

	template< class T1,  class T2,  class T3, class T4, class T5, class T6 >
	inline std::string toString (T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6)
	{
        std::ostringstream stream;
        stream << p1 << p2 << p3 << p4 << p5 << p6;
        return stream.str();
	}

	template< class T1,  class T2,  class T3, class T4, class T5, class T6, class T7 >
	inline std::string toString (T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7)
	{
        std::ostringstream stream;
        stream << p1 << p2 << p3 << p4 << p5 << p6 << p7;
        return stream.str();
	}

	template< class T1,  class T2,  class T3, class T4, class T5, class T6, class T7, class T8 >
	inline std::string toString (T1 p1, T2 p2, T3 p3, T4 p4, T5 p5, T6 p6, T7 p7, T8 p8)
	{
        std::ostringstream stream;
        stream << p1 << p2 << p3 << p4 << p5 << p6 << p7 << p8;
        return stream.str();
	}


	// утилиты для парсинга
	template< class T >
	inline T parseValue( const std::string& _value )
	{
		std::istringstream str(_value);
		T ret = 0;
		str >> ret;
        return ret;
	}

	inline char parseChar(const std::string& _value) {return parseValue<char>(_value);}
	inline unsigned char parseUChar(const std::string& _value) {return parseValue<unsigned char>(_value);}
	inline short parseShort(const std::string& _value) {return parseValue<short>(_value);}
	inline unsigned short parseUShort(const std::string& _value) {return parseValue<unsigned short>(_value);}
	inline int parseInt(const std::string& _value) {return parseValue<int>(_value);}
	inline unsigned int parseUInt(const std::string& _value) {return parseValue<unsigned int>(_value);}
	inline size_t parseSizeT(const std::string& _value) {return parseValue<size_t>(_value);}
	inline float parseFloat(const std::string& _value) {return parseValue<float>(_value);}
	inline double parseDouble(const std::string& _value) {return parseValue<double>(_value);}
	inline bool parseBool(const std::string& _value) {return ( (_value == "true") || (_value == "1") );}

	namespace templates
	{
		// делаем через шаблон, дабы реализацию заключить в *.h
		template< class T >
		const std::vector<std::string> & template_split( const std::string& _value, const std::string & _delims = "\t\n ")
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

		// для парсинга сложных типов, состоящих из простых
		template< class T1, class T2 >
		T1 template_parse_2(const std::string & _value)
		{
			const std::vector<std::string> & vec = template_split<void>(_value);
			if (vec.size() == 2) return T1( parseValue<T2>(vec[0]), parseValue<T2>(vec[1]) );
			return T1();
		}

		template< class T1, class T2 >
		T1 template_parse_4(const std::string & _value)
		{
			const std::vector<std::string> & vec = template_split<void>(_value);
			if (vec.size() == 4) return T1( parseValue<T2>(vec[0]), parseValue<T2>(vec[1]), parseValue<T2>(vec[2]), parseValue<T2>(vec[3]) );
			return T1();
		}

	} // namespace templates

	// метод разделения строк
	inline const std::vector<std::string> & split(const std::string& _value, const std::string & _delims = "\t\n ") {return templates::template_split<void>(_value, _delims);}

} // namespace util


#endif
