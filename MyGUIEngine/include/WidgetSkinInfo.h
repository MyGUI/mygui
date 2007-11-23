#ifndef __WIDGET_SKIN_INFO_H__
#define __WIDGET_SKIN_INFO_H__

#include "Prerequest.h"
#include "BasisWidgetBinding.h"

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
			m_size = _size;
			m_material = _material;
		}

		void addInfo(const BasisWidgetBinding & _bind)
		{
			checkState(_bind.m_states);
			m_basis.push_back(BasisWidgetInfo(_bind.m_type, _bind.m_offset, _bind.m_aligin));
			checkBasis();
			fillState(_bind.m_states, m_basis.size()-1);
		}

		inline void addParam(const std::string &_key, const std::string &_value)
		{
			m_params[_key] = _value;
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
			MapWidgetStateInfo::const_iterator iter = m_states.find(_name);
			if (iter == m_states.end()) {
				// добавляем новый стейт
				m_states[_name] = WidgetStateInfo();
			}
		}

		inline void checkBasis()
		{
			// и увеличиваем размер смещений по колличеству сабвиджетов
			for (MapWidgetStateInfo::iterator iter = m_states.begin(); iter!=m_states.end(); iter++) {
				while (iter->second.offsets.size() < m_basis.size()) {
					iter->second.offsets.push_back(FloatRect(0, 0, 1, 1));
				};
			}
		}

		inline void fillState(const MapBasisWidgetStateInfo & _states, size_t _index)
		{
			for (MapBasisWidgetStateInfo::const_iterator iter = _states.begin(); iter != _states.end(); iter ++) {
				// выставляем смещение для текущего саб скина
				m_states[iter->first].offsets[_index] = iter->second.offset;
				// если нужно то выставляем цвета
				if (iter->second.color != Ogre::ColourValue::ZERO) m_states[iter->first].color = iter->second.color;
				// если нужно то выставляем и альфу
				if (iter->second.alpha != -1) m_states[iter->first].alpha = iter->second.alpha;
			}
		}

	public:
		inline const IntSize & getSize() const {return m_size;}
		inline const std::string & getMaterial() const {return m_material;};
		inline const VectorBasisWidgetInfo & getBasisInfo() const {return m_basis;};
		inline const MapWidgetStateInfo & getStateInfo() const {return m_states;};
		inline const MapString & getParams() const {return m_params;};

	private:
		IntSize m_size;
		std::string m_material;
		VectorBasisWidgetInfo m_basis;
		MapWidgetStateInfo m_states;
		// дополнительные параметры скина
		MapString m_params;

	};

} // namespace MyGUI

#endif // __WIDGET_SKIN_INFO_H__