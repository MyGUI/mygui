/*!
	@file
	@author		Albert Semenov
	@date		01/2009
	@module
*/
#ifndef __UTILITY_H__
#define __UTILITY_H__

#include "Compound.h"
#include "Member.h"

namespace wrapper
{
	namespace utility
	{

		//--------------------------------------------------------------------------------------//
		// фабрика, создает тип по айди класса и своему айди
		//--------------------------------------------------------------------------------------//
		Member* getByRef(const std::string& _compound, const std::string& _member);

		//--------------------------------------------------------------------------------------//
		// возвращает полное им€ у типов, которые пр€чутьс€ за тайпдифом
		//--------------------------------------------------------------------------------------//
		std::string getTypedef(const std::string& _type, Compound * _root);

		//--------------------------------------------------------------------------------------//
		// возвращает информацию по типу и по имени
		//--------------------------------------------------------------------------------------//
		Compound* getCompound(const std::string& _kind, const std::string& _name, Compound * _root);

	} // namespace utility
} // namespace wrapper

#endif // __UTILITY_H__
