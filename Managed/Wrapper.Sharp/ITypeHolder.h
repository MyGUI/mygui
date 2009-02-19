/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __I_TYPE_HOLDER_H__
#define __I_TYPE_HOLDER_H__

#include <string>

namespace wrapper
{

	class ITypeHolder
	{
	public:
		virtual std::string getTypeDescription(const std::string& _type) = 0;
		virtual std::string getMemberName(const std::string& _name) = 0;
		virtual std::string getMemberData(const std::string& _name) = 0;
		virtual std::string getTypeInfo(const std::string& _type) = 0;
	};

} // namespace wrapper

#endif // __I_TYPE_HOLDER_H__
