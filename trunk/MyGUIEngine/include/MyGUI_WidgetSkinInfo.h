/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_WIDGET_SKIN_INFO_H__
#define __MYGUI_WIDGET_SKIN_INFO_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_SubWidgetBinding.h"
#include "MyGUI_ChildSkinInfo.h"
#include "MyGUI_MaskPeekInfo.h"

namespace MyGUI
{
	// вспомогательный класс для инициализации одного скина
	class _MyGUIExport WidgetSkinInfo
	{

	public:
		WidgetSkinInfo()
		{
			checkState("normal");
		}

		void setInfo(const IntSize & _size, const std::string &_texture)
		{
			mSize = _size;
			mTexture = _texture;
		}

		void addInfo(const SubWidgetBinding & _bind)
		{
			checkState(_bind.mStates);
			mBasis.push_back(SubWidgetInfo(_bind.mType, _bind.mOffset, _bind.mAlign));
			checkBasis();
			fillState(_bind.mStates, mBasis.size()-1);
		}

		inline void addParam(const std::string &_key, const std::string &_value)
		{
			mParams[_key] = _value;
		}

		inline void addChild(const ChildSkinInfo& _child)
		{
			mChilds.push_back(_child);
		}

		inline bool loadMask(const std::string& _file)
		{
			return mMaskPeek.load(_file);
		}

	private:
		void checkState(const MapSubWidgetStateInfo & _states)
		{
			for (MapSubWidgetStateInfo::const_iterator iter = _states.begin(); iter != _states.end(); ++iter) {
				checkState(iter->first);
			}
		}

		inline void checkState(const std::string & _name)
		{
			// ищем такой же ключ
			MapWidgetStateInfo::const_iterator iter = mStates.find(_name);
			if (iter == mStates.end()) {
				// добавляем новый стейт
				mStates[_name] = WidgetStateInfo();
			}
		}

		inline void checkBasis()
		{
			// и увеличиваем размер смещений по колличеству сабвиджетов
			for (MapWidgetStateInfo::iterator iter = mStates.begin(); iter!=mStates.end(); ++iter) {
				while (iter->second.offsets.size() < mBasis.size()) {
					iter->second.offsets.push_back(FloatRect(0, 0, 1, 1));
				};
			}
		}

		inline void fillState(const MapSubWidgetStateInfo & _states, size_t _index)
		{
			for (MapSubWidgetStateInfo::const_iterator iter = _states.begin(); iter != _states.end(); ++iter) {
				// выставляем смещение для текущего саб скина
				mStates[iter->first].offsets[_index] = iter->second.offset;
				// если нужно то выставляем цвета
				if (iter->second.colour != Ogre::ColourValue::ZERO) mStates[iter->first].colour = iter->second.colour;
				// если нужно то выставляем и альфу
				if (iter->second.alpha != -1) mStates[iter->first].alpha = iter->second.alpha;
			}
		}

	public:
		inline const IntSize & getSize() const {return mSize;}
		inline const std::string & getTextureName() const {return mTexture;}
		inline const VectorSubWidgetInfo & getBasisInfo() const {return mBasis;}
		inline const MapWidgetStateInfo & getStateInfo() const {return mStates;}
		inline const MapString & getParams() const {return mParams;}
		inline const VectorChildSkinInfo& getChild() const {return mChilds;}
		inline const MaskPeekInfo& getMask() const {return mMaskPeek;}

	private:
		IntSize mSize;
		std::string mTexture;
		VectorSubWidgetInfo mBasis;
		MapWidgetStateInfo mStates;
		// дополнительные параметры скина
		MapString mParams;
		// дети скина
		VectorChildSkinInfo mChilds;
		// маска для этого скина для пикинга
		MaskPeekInfo mMaskPeek;

	};

} // namespace MyGUI

#endif // __MYGUI_WIDGET_SKIN_INFO_H__
