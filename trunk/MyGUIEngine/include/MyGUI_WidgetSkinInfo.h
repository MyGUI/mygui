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

	// ����������, �� ����� ����������
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

	// ��������������� ����� ��� ������������� ������ �����
	class _MyGUIExport WidgetSkinInfo
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

		inline void addProperty(const std::string &_key, const std::string &_value)
		{
			mProperties[_key] = _value;
		}

		inline void addChild(const ChildSkinInfo& _child)
		{
			mChilds.push_back(_child);
		}

		inline bool loadMask(const std::string& _file)
		{
			return mMaskPeek.load(_file);
		}

		inline void clear()
		{
			for (MapWidgetStateInfo::iterator iter = mStates.begin(); iter!=mStates.end(); ++iter) {
				iter->second.clear();
			}
		}

	private:
		void checkState(const MapStateInfo & _states)
		{
			for (MapStateInfo::const_iterator iter = _states.begin(); iter != _states.end(); ++iter) {
				checkState(iter->first);
			}
		}

		inline void checkState(const std::string & _name)
		{
			// ���� ����� �� ����
			MapWidgetStateInfo::const_iterator iter = mStates.find(_name);
			if (iter == mStates.end()) {
				// ��������� ����� �����
				mStates[_name] = VectorStateInfo();
			}
		}

		inline void checkBasis()
		{
			// � ����������� ������ �������� �� ����������� �����������
			for (MapWidgetStateInfo::iterator iter = mStates.begin(); iter!=mStates.end(); ++iter) {
				iter->second.resize(mBasis.size());
			}
		}

		inline void fillState(const MapStateInfo & _states, size_t _index)
		{
			for (MapStateInfo::const_iterator iter = _states.begin(); iter != _states.end(); ++iter) {
				mStates[iter->first][_index] = iter->second;
			}
		}

	public:
		inline const IntSize & getSize() const { return mSize; }
		inline const std::string & getTextureName() const { return mTexture; }
		inline const VectorSubWidgetInfo & getBasisInfo() const { return mBasis; }
		inline const MapWidgetStateInfo & getStateInfo() const { return mStates; }
		inline const MapString & getProperties() const { return mProperties; }
		inline const VectorChildSkinInfo & getChild() const { return mChilds; }
		inline const MaskPeekInfo & getMask() const { return mMaskPeek; }

	private:
		IntSize mSize;
		std::string mTexture;
		VectorSubWidgetInfo mBasis;
		MapWidgetStateInfo mStates;
		// �������������� ��������� �����
		MapString mProperties;
		// ���� �����
		VectorChildSkinInfo mChilds;
		// ����� ��� ����� ����� ��� �������
		MaskPeekInfo mMaskPeek;

	};

} // namespace MyGUI

#endif // __MYGUI_WIDGET_SKIN_INFO_H__
