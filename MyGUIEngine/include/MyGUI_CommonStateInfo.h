/*!
	@file
	@author		Albert Semenov
	@date		06/2009
	@module
*/
/*
	This file is part of MyGUI.
	
	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.
	
	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.
	
	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __MYGUI_COMMON_STATE_INFO_H__
#define __MYGUI_COMMON_STATE_INFO_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_IStateInfo.h"
#include "MyGUI_SkinManager.h"
#include "MyGUI_LanguageManager.h"
#include "MyGUI_GenericFactory.h"

namespace MyGUI
{

	class MYGUI_EXPORT SubSkinStateInfo :
		public IStateInfo,
		public GenericFactory<SubSkinStateInfo>
	{
		MYGUI_RTTI_DERIVED( SubSkinStateInfo );

	public:
		virtual ~SubSkinStateInfo() { }

		const FloatRect& getRect() { return mRect; }

	private:
		virtual void deserialization(xml::ElementPtr _node, Version _version)
		{
			std::string texture = _node->getParent()->getParent()->findAttribute("texture");

			// поддержка замены тегов в скинах
			if (_version >= Version(1, 1))
			{
				texture = LanguageManager::getInstance().replaceTags(texture);
			}

			const IntSize & size = SkinManager::getInstance().getTextureSize(texture);
			const FloatRect & source = FloatRect::parse(_node->findAttribute("offset"));
			mRect = SkinManager::getInstance().convertTextureCoord(source, size);
		}

	private:
		FloatRect mRect;
	};

	class MYGUI_EXPORT EditTextStateInfo :
		public IStateInfo,
		public GenericFactory<EditTextStateInfo>
	{
		MYGUI_RTTI_DERIVED( EditTextStateInfo );

	public:
		EditTextStateInfo() : mColour(Colour::White), mShift(false) { }
		virtual ~EditTextStateInfo() { }

		const Colour& getColour() { return mColour; }
		bool getShift() { return mShift; }

	private:
		virtual void deserialization(xml::ElementPtr _node, Version _version)
		{
			mShift = utility::parseBool(_node->findAttribute("shift"));

			std::string colour = _node->findAttribute("colour");
			if (_version >= Version(1, 1))
			{
				colour = LanguageManager::getInstance().replaceTags(colour);
			}

			mColour = Colour::parse(colour);
		}

	private:
		Colour mColour;
		bool mShift;
	};

} // namespace MyGUI

#endif // __MYGUI_COMMON_STATE_INFO_H__
