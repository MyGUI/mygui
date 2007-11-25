/*!
	@file
	@author		Albert Semenov
	@date		11/2007
	@module
*/
#ifndef __MYGUI_WIDGET_SKIN_INFO_H__
#define __MYGUI_WIDGET_SKIN_INFO_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_BasisWidgetBinding.h"

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

		void setInfo(const IntSize & _size, const std::string &_material)
		{
			mSize = _size;
			mMaterial = _material;
		}

		void addInfo(const BasisWidgetBinding & _bind)
		{
			checkState(_bind.mStates);
			mBasis.push_back(BasisWidgetInfo(_bind.mType, _bind.mOffset, _bind.mAlign));
			checkBasis();
			fillState(_bind.mStates, mBasis.size()-1);
		}

		inline void addParam(const std::string &_key, const std::string &_value)
		{
			mParams[_key] = _value;
		}

	private:
		void checkState(const MapBasisWidgetStateInfo & _states)
		{
			for (MapBasisWidgetStateInfo::const_iterator iter = _states.begin(); iter != _states.end(); iter ++) {
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
			for (MapWidgetStateInfo::iterator iter = mStates.begin(); iter!=mStates.end(); iter++) {
				while (iter->second.offsets.size() < mBasis.size()) {
					iter->second.offsets.push_back(FloatRect(0, 0, 1, 1));
				};
			}
		}

		inline void fillState(const MapBasisWidgetStateInfo & _states, size_t _index)
		{
			for (MapBasisWidgetStateInfo::const_iterator iter = _states.begin(); iter != _states.end(); iter ++) {
				// выставляем смещение для текущего саб скина
				mStates[iter->first].offsets[_index] = iter->second.offset;
				// если нужно то выставляем цвета
				if (iter->second.color != Ogre::ColourValue::ZERO) mStates[iter->first].color = iter->second.color;
				// если нужно то выставляем и альфу
				if (iter->second.alpha != -1) mStates[iter->first].alpha = iter->second.alpha;
			}
		}

	public:
		inline const IntSize & getSize() const {return mSize;}
		inline const std::string & getMaterial() const {return mMaterial;};
		inline const VectorBasisWidgetInfo & getBasisInfo() const {return mBasis;};
		inline const MapWidgetStateInfo & getStateInfo() const {return mStates;};
		inline const MapString & getParams() const {return mParams;};

	private:
		IntSize mSize;
		std::string mMaterial;
		VectorBasisWidgetInfo mBasis;
		MapWidgetStateInfo mStates;
		// дополнительные параметры скина
		MapString mParams;

	};

} // namespace MyGUI

#endif // __MYGUI_WIDGET_SKIN_INFO_H__