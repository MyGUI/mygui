#pragma once

#include <vector>
#include <Ogre.h>
#include "WidgetDefines.h"
#include "xmlDocument.h"


namespace widget
{

	class SkinManager
	{
	private:
		SkinManager()
		{
			initialise();
		}

		~SkinManager()
		{
			for (SkinInfo::iterator iter=m_skins.begin(); iter!=m_skins.end(); iter++) {
				delete iter->second;
			}
			m_skins.clear();
		}

	public:
		static SkinManager & getInstance() // ����� �������� �����
		{
			static SkinManager instance;
			return instance;
		}

		char parseAlign(const std::string & _value)
		{
			char flag = 0;
			const std::vector<std::string> & vec = util::split(_value);
			for (size_t pos=0; pos<vec.size(); pos++) {
				MapFlags::iterator iter = m_mapAlign.find(vec[pos]);
				if (iter != m_mapAlign.end()) flag |= iter->second;
			}
			return flag;
		}

		WidgetSkinInfo * getSkin(const Ogre::String & _name)
		{
			SkinInfo::iterator iter = m_skins.find(_name);
			if (iter == m_skins.end()) {
				// ����� ���������� ��������� ����
				assert(0 && "skin is not find");
			}
			return iter->second;
		}

		//	��� ������� �������� �����
		WidgetSkinInfo * create(const Ogre::String & _name)
		{
			WidgetSkinInfo * skin = new WidgetSkinInfo();
			m_skins[_name] = skin;
			return skin;
		}

		void loadSkin(const std::string & _file)
		{
			xml::xmlDocument doc;
			if (!doc.load(_file)) OGRE_EXCEPT(0, doc.getLastError(), "");

			xml::xmlNodePtr xml_root = doc.getRoot();
			if (xml_root == 0) return;
			if (xml_root->getName() != "MyGUI_SkinInfo") return;

			// ��������������� ����� ��� �������� ���������
			BasisWidgetBinding bind;

			// ����� ����� � ��������, �������� ���� �� �������
			xml::VectorNode & skins = xml_root->getChilds();
			for (size_t i_skin=0; i_skin<skins.size(); i_skin++) {
				xml::xmlNodePtr skinInfo = skins[i_skin];
				if (skinInfo->getName() != "Skin") continue;

				// ������ �������� �����
				const xml::VectorAttributes & attrib = skinInfo->getAttributes();
				Ogre::String skinName, skinMaterial;//, fontName;
				intSize size;
				for (size_t i_attrib=0; i_attrib<attrib.size(); i_attrib++) {
					// ������� ���� ������� - ��������
					const xml::PairAttributes & pairAttributes = attrib[i_attrib];
					if (pairAttributes.first == "Name") skinName = pairAttributes.second;
					else if (pairAttributes.first == "Material") skinMaterial = pairAttributes.second;
					else if (pairAttributes.first == "Size") size = intSize::parse(pairAttributes.second);
				}

				// ������� ����
				WidgetSkinInfo * widget_info = create(skinName);
				widget_info->setInfo(size, skinMaterial);

				// ����� ����� � ��������, ���� � ��� �������
				xml::VectorNode & basisSkins = skinInfo->getChilds();
				for (size_t i_basis=0; i_basis<basisSkins.size(); i_basis++) {
					xml::xmlNodePtr basisSkinInfo = basisSkins[i_basis];

					if (basisSkinInfo->getName() == "Property") {
						// ��������� ��������
						const xml::VectorAttributes & attrib = basisSkinInfo->getAttributes();
						std::string key, value;
						for (size_t i_attrib=0; i_attrib<attrib.size(); i_attrib++) {
							// ������� ���� ������� - ��������
							const xml::PairAttributes & pairAttributes = attrib[i_attrib];
							if (pairAttributes.first == "Key") key = pairAttributes.second;
							else if (pairAttributes.first == "Value") value = pairAttributes.second;
						}
						// ��������� ��������
						widget_info->addParam(key, value);
						// ��� ������
						continue;

					} else if (basisSkinInfo->getName() != "BasisSkin") continue;

					// ������ �������� ��� �����
					const xml::VectorAttributes & attrib = basisSkinInfo->getAttributes();
					Ogre::String basisSkinType;
					intRect offset;
					char align = ALIGN_NONE;
					for (size_t i_attrib=0; i_attrib<attrib.size(); i_attrib++) {
						// ������� ���� ������� - ��������
						const xml::PairAttributes & pairAttributes = attrib[i_attrib];
						if (pairAttributes.first == "Type") basisSkinType = pairAttributes.second;
						else if (pairAttributes.first == "Offset") offset = intRect::parse(pairAttributes.second);
						else if (pairAttributes.first == "Align") align = parseAlign(pairAttributes.second);
					}

					bind.create(offset, align, basisSkinType);

					// ����� ����� � ��������, ���� �� ��������
					xml::VectorNode & basisState = basisSkinInfo->getChilds();
					for (size_t i_state=0; i_state<basisState.size(); i_state++) {
						xml::xmlNodePtr basisStateInfo = basisState[i_state];
						if (basisStateInfo->getName() != "State") continue;

						// ������ �������� ������
						const xml::VectorAttributes & attrib = basisStateInfo->getAttributes();
						Ogre::String basisStateName;
						floatRect offset;
						for (size_t i_attrib=0; i_attrib<attrib.size(); i_attrib++) {
							// ������� ���� ������� - ��������
							const xml::PairAttributes & pairAttributes = attrib[i_attrib];
							if (pairAttributes.first == "Name") basisStateName = pairAttributes.second;
							else if (pairAttributes.first == "Offset") offset = floatRect::parse(pairAttributes.second);
						}
						// ��������� ���� � ������
						bind.add(basisStateName, offset);

					} // for (size_t i_state=0; i_state<basisState.size(); i_state++) {

					// ������ �� ������ ��������� � ����
					widget_info->addInfo(bind);

				} // for (size_t i_basis=0; i_basis<basisSkins.size(); i_basis++) {
			} // for (size_t i_skin=0; i_skin<skins.size(); i_skin++) {
		}

	private:
		void initialise()
		{
			// �������� ����� ������� ������������
			REGISTER_VALUE(m_mapAlign, ALIGN_NONE);
			REGISTER_VALUE(m_mapAlign, ALIGN_HCENTER);
			REGISTER_VALUE(m_mapAlign, ALIGN_VCENTER);
			REGISTER_VALUE(m_mapAlign, ALIGN_CENTER);
			REGISTER_VALUE(m_mapAlign, ALIGN_CENTER_PARENT);
			REGISTER_VALUE(m_mapAlign, ALIGN_LEFT);
			REGISTER_VALUE(m_mapAlign, ALIGN_RIGHT);
			REGISTER_VALUE(m_mapAlign, ALIGN_HSTRETCH);
			REGISTER_VALUE(m_mapAlign, ALIGN_TOP);
			REGISTER_VALUE(m_mapAlign, ALIGN_BOTTOM);
			REGISTER_VALUE(m_mapAlign, ALIGN_VSTRETCH);
			REGISTER_VALUE(m_mapAlign, ALIGN_STRETCH);
		}

	private:
		SkinInfo m_skins;
		MapFlags m_mapAlign;

	}; // class SkinManager

} // namespace widget