/*!
	@file
	@author		Albert Semenov
	@date		09/2008
	@module
*/
#include "MyGUI_Gui.h"
#include "MyGUI_LanguageManager.h"
#include "MyGUI_XmlDocument.h"

namespace MyGUI
{

	const std::string XML_TYPE("Language");

	INSTANCE_IMPLEMENT(LanguageManager);

	void LanguageManager::initialise()
	{
		MYGUI_ASSERT(false == mIsInitialise, INSTANCE_TYPE_NAME << " initialised twice");
		MYGUI_LOG(Info, "* Initialise: " << INSTANCE_TYPE_NAME);

		Gui::getInstance().registerLoadXmlDelegate(XML_TYPE) = newDelegate(this, &LanguageManager::_load);

		mCurrentLanguage = mMapFile.end();


		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully initialized");
		mIsInitialise = true;
	}

	void LanguageManager::shutdown()
	{
		if (false == mIsInitialise) return;
		MYGUI_LOG(Info, "* Shutdown: " << INSTANCE_TYPE_NAME);

		Gui::getInstance().unregisterLoadXmlDelegate(XML_TYPE);

		MYGUI_LOG(Info, INSTANCE_TYPE_NAME << " successfully shutdown");
		mIsInitialise = false;
	}

	bool LanguageManager::load(const std::string & _file, const std::string & _group)
	{
		return Gui::getInstance()._loadImplement(_file, _group, true, XML_TYPE, INSTANCE_TYPE_NAME);
	}

	void LanguageManager::_load(xml::xmlNodePtr _node, const std::string & _file)
	{
		/*std::string layer, def, text;

		// ����� ����� � ��������, �������� ����
		xml::xmlNodeIterator pointer = _node->getNodeIterator();
		while (pointer.nextNode(XML_TYPE)) {

			// ������ ��������
			pointer->findAttribute("layer", layer);
			pointer->findAttribute("default", def);

			// ���������
			text = pointer->findAttribute("texture");

			IntSize textureSize = SkinManager::getTextureSize(text);

			// ����� ����� � ��������, �������� ����
			xml::xmlNodeIterator info = pointer->getNodeIterator();
			while (info.nextNode("Info")) {

				// �������� ����������
				FloatRect offset(0, 0, 1, 1);

				// ������ ��������
				std::string texture(info->findAttribute("texture"));
				std::string name(info->findAttribute("name"));
				std::string size(info->findAttribute("size"));
				IntPoint point = IntPoint::parse(info->findAttribute("point"));

				std::string offset_str(info->findAttribute("offset"));
				if (false == offset_str.empty()) {
					if (texture.empty()) offset = SkinManager::convertTextureCoord(FloatRect::parse(offset_str), textureSize);
					else offset = SkinManager::convertTextureCoord(FloatRect::parse(offset_str), SkinManager::getTextureSize(texture));
				}

				// ��������� ������
				if (mMapPointers.find(name) != mMapPointers.end()) {
					MYGUI_LOG(Warning, "pointer '" << name << "' exist, erase old data");
				}
				mMapPointers[name] = PointerInfo(offset, point, IntSize::parse(size), texture);

			};
		};

		// ���� ���� �����, �� �������������, ���� ��� �������, �� �������
		if (false == layer.empty()) {
			if (null == mMousePointer) {
				mMousePointer = _createWidget("Widget", "StaticImage", IntCoord(), ALIGN_DEFAULT, "", "");
			}
			LayerManager::getInstance().attachToLayerKeeper(layer, mMousePointer);
		}

		// ���� ���� ��������� ������ �� ������
		if (false == def.empty()) mDefaultPointer = def;
		if (false == text.empty()) mTexture = text;

		// ���� ���������� ���, �� ������� ������ �� ������
		if (mDefaultPointer.empty() && !mMapPointers.empty()) mDefaultPointer = mMapPointers.begin()->first;

		// ������ ��������� ���������
		setPointer(mDefaultPointer, null);*/
	}

	void LanguageManager::setCurrentLanguage(const std::string & _name)
	{
		mCurrentLanguage = mMapFile.find(_name);
		if (mCurrentLanguage == mMapFile.end()) {
			MYGUI_LOG(Error, "Language '" << _name << "' is not found");
			return;
		}

		loadLanguage(mCurrentLanguage->second);
	}

	void LanguageManager::loadLanguage(const std::string & _file)
	{
		mMapLanguage.clear();
	}

} // namespace MyGUI	
