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
	// ��������������� ����� ��� ������������� ������ �����
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
			// ���� ����� �� ����
			MapWidgetStateInfo::const_iterator iter = mStates.find(_name);
			if (iter == mStates.end()) {
				// ��������� ����� �����
				mStates[_name] = WidgetStateInfo();
			}
		}

		inline void checkBasis()
		{
			// � ����������� ������ �������� �� ����������� �����������
			for (MapWidgetStateInfo::iterator iter = mStates.begin(); iter!=mStates.end(); ++iter) {
				while (iter->second.offsets.size() < mBasis.size()) {
					iter->second.offsets.push_back(FloatRect(0, 0, 1, 1));
				};
			}
		}

		inline void fillState(const MapSubWidgetStateInfo & _states, size_t _index)
		{
			for (MapSubWidgetStateInfo::const_iterator iter = _states.begin(); iter != _states.end(); ++iter) {
				// ���������� �������� ��� �������� ��� �����
				mStates[iter->first].offsets[_index] = iter->second.offset;
				// ���� ����� �� ���������� �����
				if (iter->second.colour != Ogre::ColourValue::ZERO) mStates[iter->first].colour = iter->second.colour;
				// ���� ����� �� ���������� � �����
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
		// �������������� ��������� �����
		MapString mParams;
		// ���� �����
		VectorChildSkinInfo mChilds;
		// ����� ��� ����� ����� ��� �������
		MaskPeekInfo mMaskPeek;

	};

} // namespace MyGUI

#endif // __MYGUI_WIDGET_SKIN_INFO_H__
