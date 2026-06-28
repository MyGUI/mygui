/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_COMMON_STATE_INFO_H_
#define MYGUI_COMMON_STATE_INFO_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_IStateInfo.h"
#include "MyGUI_CoordConverter.h"
#include "MyGUI_LanguageManager.h"
#include "MyGUI_TextureUtility.h"

namespace MyGUI
{

	class MYGUI_EXPORT SubSkinStateInfo : public IStateInfo
	{
		MYGUI_RTTI_DERIVED(SubSkinStateInfo)

	public:
		const FloatRect& getRect() const
		{
			return mRect;
		}

	private:
		void deserialization(xml::ElementPtr _node, Version _version) override;

	private:
		FloatRect mRect;
	};

	class MYGUI_EXPORT TileRectStateInfo : public IStateInfo
	{
		MYGUI_RTTI_DERIVED(TileRectStateInfo)

	public:
		const FloatRect& getRect() const
		{
			return mRect;
		}

		const IntSize& getTileSize() const
		{
			return mTileSize;
		}

		bool getTileH() const
		{
			return mTileH;
		}

		bool getTileV() const
		{
			return mTileV;
		}

	private:
		void deserialization(xml::ElementPtr _node, Version _version) override;

	private:
		FloatRect mRect;
		IntSize mTileSize;
		bool mTileH{true};
		bool mTileV{true};
	};

	class MYGUI_EXPORT RotatingSkinStateInfo : public IStateInfo
	{
		MYGUI_RTTI_DERIVED(RotatingSkinStateInfo)

	public:
		float getAngle() const
		{
			return mAngle;
		}

		const IntPoint& getCenter() const
		{
			return mCenter;
		}

		const FloatRect& getRect() const
		{
			return mRect;
		}

	private:
		void deserialization(xml::ElementPtr _node, Version _version) override;

	private:
		FloatRect mRect;
		IntPoint mCenter;
		float mAngle{0}; // Angle in radians
	};


	class MYGUI_EXPORT EditTextStateInfo : public IStateInfo
	{
		MYGUI_RTTI_DERIVED(EditTextStateInfo)

	public:
		const Colour& getColour() const
		{
			return mColour;
		}

		bool getShift() const
		{
			return mShift;
		}

	private:
		void deserialization(xml::ElementPtr _node, Version _version) override;

	private:
		Colour mColour{Colour::White};
		bool mShift{false};
	};

} // namespace MyGUI

#endif // MYGUI_COMMON_STATE_INFO_H_
