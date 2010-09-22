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

#include "FlashValue.h"

using namespace Hikari;

template<class NumberType>
inline NumberType toNumber(MyGUI::UString numberString)
{
	if (numberString.substr(0, 4).compare("true") == 0) return 1;
	else if (numberString.substr(0, 4).compare("false") == 0) return 0;

	std::istringstream converter(numberString);

	if (typeid(NumberType) == typeid(bool))
	{
		int result;
		return (converter >> result).fail() ? false : !!result;
	}

	NumberType result;
	return (converter >> result).fail() ? 0 : result;
}

template<class NumberType>
inline std::string numberToString(const NumberType& number)
{
	std::ostringstream converter;

	if (typeid(NumberType) == typeid(bool))
	{
		return number ? "true" : "false";
	}

	return (converter << number).fail() ? "" : converter.str();
}

FlashValue::FlashValue() : numValue(0), boolValue(0), valueType(FT_NULL)
{
}

FlashValue::FlashValue(bool booleanValue) : numValue(0), boolValue(booleanValue), valueType(FT_BOOLEAN)
{
}

FlashValue::FlashValue(int numericValue) : numValue(float(numericValue)), boolValue(0), valueType(FT_NUMBER)
{
}

FlashValue::FlashValue(float numericValue) : numValue(numericValue), boolValue(0), valueType(FT_NUMBER)
{
}

FlashValue::FlashValue(const char* stringValue) : strValue(stringValue), numValue(0), boolValue(0), valueType(FT_STRING)
{
}

FlashValue::FlashValue(const wchar_t* stringValue) : strValue(stringValue), numValue(0), boolValue(0), valueType(FT_STRING)
{
}

FlashValue::FlashValue(const std::string& stringValue) : strValue(stringValue), numValue(0), boolValue(0), valueType(FT_STRING)
{
}

FlashValue::FlashValue(const std::wstring& stringValue) : strValue(stringValue), numValue(0), boolValue(0), valueType(FT_STRING)
{
}

FlashValue::FlashValue(const MyGUI::UString& stringValue) : strValue(stringValue), numValue(0), boolValue(0), valueType(FT_STRING)
{
}

short FlashValue::getType() const
{
	return valueType;
}

bool FlashValue::isNull() const
{
	return valueType == FT_NULL;
}

void FlashValue::setNull()
{
	strValue.clear();
	numValue = 0;
	boolValue = false;
	valueType = FT_NULL;
}

bool FlashValue::getBool() const
{
	if (valueType == FT_BOOLEAN)
		return boolValue;
	else if (valueType == FT_NUMBER)
		return !!((int)numValue);
	else if (valueType == FT_STRING)
		return toNumber<bool>(strValue);

	return false;
}

float FlashValue::getNumber() const
{
	if (valueType == FT_NUMBER)
		return numValue;
	else if (valueType == FT_BOOLEAN)
		return (float)boolValue;
	else if (valueType == FT_STRING)
		return toNumber<float>(strValue);

	return 0;
}

MyGUI::Colour FlashValue::getNumberAsColor() const
{
	if (valueType != FT_NUMBER)
		return MyGUI::Colour::Zero;

	MyGUI::Colour result;
	result.blue = ((int)numValue % 256) / 255.0f;
	result.green = (((int)numValue / 256) % 256) / 255.0f;
	result.red = (((int)numValue / 256 / 256) % 256) / 255.0f;

	return result;
}

MyGUI::UString FlashValue::getString() const
{
	if (valueType == FT_STRING)
		return strValue;
	else if (valueType == FT_BOOLEAN)
		return numberToString<bool>(boolValue);
	else if (valueType == FT_NUMBER)
		return numberToString<float>(numValue);

	return "";
}

Args::Args()
{
}

Args::Args(Args& v)
{
	this->swap(v);
}

Args::Args(const FlashValue& firstArg) : Arguments(1, firstArg)
{
}

Args& Args::operator()(const FlashValue& newArg)
{
	this->push_back(newArg);
	return *this;
}
