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
	// ��������������� ����� ��� ������������� ������ �����
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
			for (MapWidgetStateInfo::iterator iter = mStates.begin(); iter!=mStates.end(); iter++) {
				while (iter->second.offsets.size() < mBasis.size()) {
					iter->second.offsets.push_back(FloatRect(0, 0, 1, 1));
				};
			}
		}

		inline void fillState(const MapBasisWidgetStateInfo & _states, size_t _index)
		{
			for (MapBasisWidgetStateInfo::const_iterator iter = _states.begin(); iter != _states.end(); iter ++) {
				// ���������� �������� ��� �������� ��� �����
				mStates[iter->first].offsets[_index] = iter->second.offset;
				// ���� ����� �� ���������� �����
				if (iter->second.color != Ogre::ColourValue::ZERO) mStates[iter->first].color = iter->second.color;
				// ���� ����� �� ���������� � �����
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
		// �������������� ��������� �����
		MapString mParams;

	};

} // namespace MyGUI

#endif // __MYGUI_WIDGET_SKIN_INFO_H__