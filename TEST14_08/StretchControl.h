#pragma once

#include "MyGUI_Source\\MyGUI.h"

class StretchControl
{
private:
	StretchControl::StretchControl() {};
public:
	StretchControl::StretchControl(const String & strMaterial, const String & m_strMaterialBorderNormal, const String & strMaterialBorderActived) {};

private:
	String m_strMaterialName;
	uint16 m_uTextureSizeX, m_uTextureSizeY;

	String m_strMaterialBorderNormal;
	String m_strMaterialBorderActived;

};