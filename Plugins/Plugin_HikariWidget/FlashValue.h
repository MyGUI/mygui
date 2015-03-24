/*
	This file is part of Hikari, a library that allows developers
	to use Flash in their Ogre3D applications.

	Copyright (C) 2008 Adam J. Simmons

	This library is free software; you can redistribute it and/or
	modify it under the terms of the GNU Lesser General Public
	License as published by the Free Software Foundation; either
	version 2.1 of the License, or (at your option) any later version.

	This library is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
	Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public
	License along with this library; if not, write to the Free Software
	Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef FlashValue_H_
#define FlashValue_H_

#include <MyGUI.h>

namespace Hikari
{

	/**
	* Defines the Flash variable types, used by FlashValue.
	*/
	enum FlashType
	{
		FT_NULL,
		FT_BOOLEAN,
		FT_NUMBER,
		FT_STRING
	};

	/**
	* FlashValue represents a Flash ActionScript variable. The currently supported types are: null, boolean, number, and string.
	*/
	class FlashValue
	{
	public:
		/**
		* Creates a null FlashValue.
		*/
		FlashValue();

		/**
		* Creates a FlashValue as a boolean type.
		*
		* @param	booleanValue	The value to initialize this FlashValue with.
		*/
		FlashValue(bool booleanValue);

		/**
		* Creates a FlashValue as a number type.
		*
		* @param	numericValue	The value to initialize this FlashValue with.
		*/
		FlashValue(int numericValue);

		/**
		* Creates a FlashValue as a number type.
		*
		* @param	numericValue	The value to initialize this FlashValue with.
		*/
		FlashValue(float numericValue);

		/**
		* Creates a FlashValue as a string type.
		*
		* @param	stringValue	The value to initialize this FlashValue with.
		*/
		FlashValue(const char* stringValue);

		/**
		* Creates a FlashValue as a string type.
		*
		* @param	stringValue	The value to initialize this FlashValue with.
		*/
		FlashValue(const wchar_t* stringValue);

		/**
		* Creates a FlashValue as a string type.
		*
		* @param	stringValue	The value to initialize this FlashValue with.
		*/
		FlashValue(const std::string& stringValue);

		/**
		* Creates a FlashValue as a string type.
		*
		* @param	stringValue	The value to initialize this FlashValue with.
		*/
		FlashValue(const std::wstring& stringValue);

		/**
		* Creates a FlashValue as a string type.
		*
		* @param	stringValue	The value to initialize this FlashValue with.
		*/
		FlashValue(const MyGUI::UString& stringValue);

		/**
		* Retrieves the FlashType of this FlashValue.
		*/
		short getType() const;

		/**
		* Returns whether or not this FlashValue is of a null type.
		*/
		bool isNull() const;

		/**
		* Sets this FlashValue as a null type.
		*/
		void setNull();

		/**
		* Retrieves the value as a boolean.
		*
		* @note	If the actual value type is FT_BOOLEAN, this directly retrieves the actual value. Otherwise
		*		this function will make an interpretation of the value as that of a boolean. Number values
		*		will be cast to boolean and string values will be parsed lexically ("true" and "false" are valid).
		*		A value type of null will always return false.
		*/
		bool getBool() const;

		/**
		* Retrieves the value as a number.
		*
		* @note	If the actual value type is FT_NUMBER, this directly retrieves the actual value. Otherwise
		*		this function will make an interpretation of the value as that of a number type. Boolean
		*		values will be cast to a number and string values will be parsed lexically. A value type of
		*		null will always return '0'.
		*/
		float getNumber() const;

		/**
		* If this FlashValue is a number type, retrieves the number value interpreted as a color.
		*
		* @note	Color values in ActionScript are generally encoded as a number, hence this function's utility.
		*/
		MyGUI::Colour getNumberAsColor() const;

		/**
		* Retrieves the value as a string.
		*
		* @note	If the actual value type is FT_STRING, this directly retrieves the actual value. Otherwise
		*		this function will make an interpretation of the value as that of a string type. Boolean
		*		values will either be "true" or "false", number values will be output in standard form,
		*		and null value types will always return an empty string.
		*/
		MyGUI::UString getString() const;

	private:
		MyGUI::UString strValue;
		float numValue;
		bool boolValue;
		short valueType;
	};

	/**
	* Arguments is defined as a vector of FlashValues. Used to communicate with ActionScript functions.
	*/
	typedef std::vector<FlashValue> Arguments;

	/**
	* This helper class can be used to quickly declare variable-length FlashValue arguments inline.
	*
	* Syntax is: \code Args(x)(x)(x)(x)... \endcode
	*/
	class Args : public Arguments
	{
	public:
		Args();

		Args(Args& v);

		explicit Args(const FlashValue& firstArg);

		Args& operator()(const FlashValue& newArg);
	};

}

#endif
