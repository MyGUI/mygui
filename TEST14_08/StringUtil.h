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

}