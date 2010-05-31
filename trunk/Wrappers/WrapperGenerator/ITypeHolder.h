/*!
	@file
	@author		Albert Semenov
	@date		01/2009
*/
#ifndef __I_TYPE_HOLDER_H__
#define __I_TYPE_HOLDER_H__

#include <string>
#include <vector>
#include <map>

namespace wrapper
{

	struct Param { std::string type, name, def; };
	typedef std::vector<Param> VectorParam;

	class ITypeHolder
	{
	public:
		ITypeHolder() { }
		virtual ~ITypeHolder() { }

		typedef std::pair<std::string, std::string> PairString;
		typedef std::vector<PairString> VectorPairString;
		typedef std::vector<std::string> VectorString;
		typedef std::map<std::string, VectorPairString> MapVectorPairString;

		virtual std::string getTypeDescription(const std::string& _type) = 0;
		virtual std::string getMemberData(const std::string& _name) = 0;
		virtual VectorPairString getTypeInfo(const std::string& _type) = 0;
		virtual std::string getTemplatePrefix(const std::string& _template, const std::string& _rettype, const VectorParam& _params) = 0;
	};

} // namespace wrapper

#endif // __I_TYPE_HOLDER_H__

