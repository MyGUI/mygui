#pragma once

#include <Ogre.h>
#include "MyGUI_Source\\MyGUI.h"

using namespace Ogre;

class StretchControl : public MyGUI::EventCallback
{
private:
	StretchControl::StretchControl() {};

public:
	StretchControl::StretchControl(const String & strMaterial, const String & strMaterialBorderNormal, const String & strMaterialBorderActived)
	{
	};

private:
	String m_strMaterialName;
	uint16 m_uTextureSizeX, m_uTextureSizeY;

	String m_strMaterialBorderNormal;
	String m_strMaterialBorderActived;

};