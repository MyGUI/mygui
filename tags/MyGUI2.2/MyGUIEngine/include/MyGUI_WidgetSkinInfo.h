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
#include "MyGUI_MaskPickInfo.h"

namespace MyGUI
{

	// информация, об одном сабвиджете
	struct SubWidgetInfo
	{
		SubWidgetInfo(const std::string & _type, const IntCoord& _coord, Align _align, const MapString & _properties) :
			coord(_coord),
			align(_align),
			type(_type),
			properties(_properties)
		{
		}

		IntCoord coord;
		Align align;
		std::string type;
		MapString properties;
	};

	typedef std::vector<SubWidgetInfo> VectorSubWidgetInfo;

	class WidgetSkinInfo;
	typedef WidgetSkinInfo * WidgetSkinInfoPtr;
	typedef std::map<std::string, WidgetSkinInfoPtr> MapWidgetSkinInfoPtr;

	// вспомогательный класс для инициализации одного скина
	class MYGUI_EXPORT WidgetSkinInfo
	{

	public:
		WidgetSkinInfo()
		{
		}

		void setInfo(const IntSize & _size, const std::string &_texture)
		{
			mSize = _size;
			mTexture = _texture;
		}

		void addInfo(const SubWidgetBinding & _bind)
		{
			checkState(_bind.mStates);
			mBasis.push_back(SubWidgetInfo(_bind.mType, _bind.mOffset, _bind.mAlign, _bind.mProperties));
			checkBasis();
			fillState(_bind.mStates, mBasis.size()-1);
		}

		void addProperty(const std::string &_key, const std::string &_value)
		{
			mProperties[_key] = _value;
		}

		void addChild(const ChildSkinInfo& _child)
		{
			mChilds.push_back(_child);
		}

		bool loadMask(const std::string& _file)
		{
			return mMaskPeek.load(_file);
		}

		void clear()
		{
			for (MapWidgetStateInfo::iterator iter = mStates.begin(); iter!=mStates.end(); ++iter) {
				for (VectorStateInfo::iterator iter2=iter->second.begin(); iter2!=iter->second.end(); ++iter2) {
					delete *iter2;
				}
			}
		}

	private:
		void checkState(const MapStateInfo & _states)
		{
			for (MapStateInfo::const_iterator iter = _states.begin(); iter != _states.end(); ++iter) {
				checkState(iter->first);
			}
		}

		void checkState(const std::string & _name)
		{
			// ищем такой же ключ
			MapWidgetStateInfo::const_iterator iter = mStates.find(_name);
			if (iter == mStates.end()) {
				// добавляем новый стейт
				mStates[_name] = VectorStateInfo();
			}
		}

		void checkBasis()
		{
			// и увеличиваем размер смещений по колличеству сабвиджетов
			for (MapWidgetStateInfo::iterator iter = mStates.begin(); iter!=mStates.end(); ++iter) {
				iter->second.resize(mBasis.size());
			}
		}

		void fillState(const MapStateInfo & _states, size_t _index)
		{
			for (MapStateInfo::const_iterator iter = _states.begin(); iter != _states.end(); ++iter) {
				mStates[iter->first][_index] = iter->second;
			}
		}

	public:
		const IntSize & getSize() const { return mSize; }
		const std::string & getTextureName() const { return mTexture; }
		const VectorSubWidgetInfo & getBasisInfo() const { return mBasis; }
		const MapWidgetStateInfo & getStateInfo() const { return mStates; }
		const MapString & getProperties() const { return mProperties; }
		const VectorChildSkinInfo & getChild() const { return mChilds; }
		MaskPickInfo const * getMask() const { return &mMaskPeek; }

	private:
		IntSize mSize;
		std::string mTexture;
		VectorSubWidgetInfo mBasis;
		MapWidgetStateInfo mStates;
		// дополнительные параметры скина
		MapString mProperties;
		// дети скина
		VectorChildSkinInfo mChilds;
		// маска для этого скина для пикинга
		MaskPickInfo mMaskPeek;

	};

} // namespace MyGUI

#endif // __MYGUI_WIDGET_SKIN_INFO_H__
