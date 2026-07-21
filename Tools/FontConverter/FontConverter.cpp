/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

// Headless CLI that loads a ResourceTrueTypeFont XML, generates its glyph atlas
// in memory, and writes a ResourceManualFont XML + PNG pair equivalent to what
// the FontEditor's export flow produces. Intended for re-baking assets from source
// XMLs without launching the GUI editor.

#include "Precompiled.h"
#include "BaseManager.h"

#include <MyGUI.h>
#include <MyGUI_FontData.h>
#include <MyGUI_ResourceTrueTypeFont.h>

#include <pugixml.hpp>

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace
{

	void addCode(pugi::xml_node _codesNode, MyGUI::Char _code, MyGUI::ResourceTrueTypeFont* _font, bool _isSubstitute)
	{
		pugi::xml_node codeNode = _codesNode.append_child("Code");

		if (_isSubstitute)
		{
			codeNode.append_attribute("index").set_value("substitute");
		}
		else
		{
			switch (_code)
			{
			case MyGUI::FontCodeType::Selected: codeNode.append_attribute("index").set_value("selected"); break;
			case MyGUI::FontCodeType::SelectedBack:
				codeNode.append_attribute("index").set_value("selected_back");
				break;
			case MyGUI::FontCodeType::Cursor: codeNode.append_attribute("index").set_value("cursor"); break;
			default: codeNode.append_attribute("index").set_value(_code); break;
			}
		}

		const MyGUI::GlyphInfo* info = _font->getGlyphInfo(_code);
		MyGUI::ITexture* texture = _font->getTextureFont();
		const float texWidth = static_cast<float>(texture->getWidth());
		const float texHeight = static_cast<float>(texture->getHeight());

		const MyGUI::FloatCoord coord(
			info->uvRect.left * texWidth,
			info->uvRect.top * texHeight,
			(info->uvRect.right - info->uvRect.left) * texWidth,
			(info->uvRect.bottom - info->uvRect.top) * texHeight);

		if (!coord.empty())
			codeNode.append_attribute("coord").set_value(MyGUI::utility::toString(coord).c_str());

		if (info->width != coord.width || info->height != coord.height)
			codeNode.append_attribute("size").set_value(
				MyGUI::utility::toString(MyGUI::FloatSize(info->width, info->height)).c_str());

		if (info->bearingX != 0.0f || info->bearingY != 0.0f)
			codeNode.append_attribute("bearing").set_value(
				MyGUI::utility::toString(MyGUI::FloatPoint(info->bearingX, info->bearingY)).c_str());

		if (info->advance != info->width)
			codeNode.append_attribute("advance").set_value(MyGUI::utility::toString(info->advance).c_str());

		for (const auto& range : _font->getCodePointRanges())
		{
			for (MyGUI::Char right = range.first; right <= range.second && right >= range.first; ++right)
			{
				const float kerning = _font->getKerning(_code, right);
				if (kerning != 0.0f)
				{
					pugi::xml_node kerningNode = codeNode.append_child("Kerning");
					kerningNode.append_attribute("right").set_value(right);
					kerningNode.append_attribute("offset").set_value(MyGUI::utility::toString(kerning).c_str());
				}
			}
		}
	}

	bool writeManualFont(
		pugi::xml_node _rootNode,
		MyGUI::ResourceTrueTypeFont* _font,
		const std::string& _fontName,
		const std::string& _textureName,
		const std::string& _shader)
	{
		pugi::xml_node resource = _rootNode.append_child("Resource");
		resource.append_attribute("type").set_value("ResourceManualFont");
		resource.append_attribute("name").set_value(_fontName.c_str());

		auto addProperty = [&](std::string_view _key, const std::string& _value)
		{
			pugi::xml_node prop = resource.append_child("Property");
			prop.append_attribute("key").set_value(std::string(_key).c_str());
			prop.append_attribute("value").set_value(_value.c_str());
		};

		addProperty("Source", _textureName);
		if (!_shader.empty())
			addProperty("Shader", _shader);
		addProperty("DefaultHeight", MyGUI::utility::toString(_font->getDefaultHeight()));

		pugi::xml_node codesNode = resource.append_child("Codes");

		const auto codePointRanges = _font->getCodePointRanges();
		const MyGUI::Char substituteCodePoint = _font->getSubstituteCodePoint();
		const bool isCustomSubstituteCodePoint = substituteCodePoint != MyGUI::FontCodeType::NotDefined;

		for (const auto& range : codePointRanges)
		{
			for (MyGUI::Char code = range.first; code <= range.second && code >= range.first; ++code)
			{
				if (code != substituteCodePoint || isCustomSubstituteCodePoint)
					addCode(codesNode, code, _font, false);
			}
		}

		addCode(codesNode, substituteCodePoint, _font, true);
		return true;
	}

	int usage(const char* _prog)
	{
		std::cerr << "Usage: " << _prog << " <input_source.xml> <output.xml> [additional_resource_dir...]\n\n"
				  << "Converts all ResourceTrueTypeFont entries in the input XML into\n"
				  << "ResourceManualFont entries in a single output XML. Each font gets its own\n"
				  << "PNG atlas named <FontName>.png next to the output XML.\n\n"
				  << "The input XML's directory is added as a resource location automatically\n"
				  << "so referenced font files (e.g. .ttf) can be found.\n";
		return 2;
	}

} // namespace

namespace tools
{

	class Application : public base::BaseManager
	{
	public:
		Application(fs::path _inputXml, fs::path _outputXml, std::vector<std::string> _extraDirs) :
			mInputXml(std::move(_inputXml)),
			mOutputXml(std::move(_outputXml)),
			mExtraDirs(std::move(_extraDirs))
		{
			setResourceFilename(std::string_view{});
		}

		int getExitCode() const
		{
			return mExitCode;
		}

		void createScene() override
		{
			pugi::xml_document sourceDoc;
			pugi::xml_parse_result parseResult = sourceDoc.load_file(mInputXml.string().c_str());
			if (!parseResult)
			{
				std::cerr << "Failed to parse input XML: " << parseResult.description() << "\n";
				mExitCode = 1;
				quit();
				return;
			}

			auto fontNodes = sourceDoc.select_nodes("MyGUI/Resource[@type='ResourceTrueTypeFont']");
			if (fontNodes.empty())
			{
				std::cerr << "No ResourceTrueTypeFont elements found in input XML.\n";
				mExitCode = 1;
				quit();
				return;
			}

			const fs::path inputPath(mInputXml);
			if (!MyGUI::ResourceManager::getInstance().load(inputPath.filename().string()))
			{
				std::cerr << "ResourceManager::load failed for: " << inputPath.filename() << "\n";
				mExitCode = 1;
				quit();
				return;
			}

			pugi::xml_document outDoc;
			auto declaration = outDoc.append_child(pugi::node_declaration);
			declaration.append_attribute("version") = "1.0";
			declaration.append_attribute("encoding") = "UTF-8";

			pugi::xml_node outRoot = outDoc.append_child("MyGUI");
			outRoot.append_attribute("type").set_value("Resource");
			outRoot.append_attribute("version").set_value("1.2");

			for (const auto& xpathNode : fontNodes)
			{
				pugi::xml_node resourceNode = xpathNode.node();
				const std::string fontName = resourceNode.attribute("name").value();
				if (fontName.empty())
				{
					std::cerr << "Skipping ResourceTrueTypeFont with no 'name' attribute.\n";
					continue;
				}
				std::string shader;
				if (pugi::xml_node shaderProperty = resourceNode.select_node("Property[@key='Shader']").node())
					shader = shaderProperty.attribute("value").value();

				MyGUI::IFont* iFont = MyGUI::FontManager::getInstance().getByName(fontName);
				auto* font = iFont != nullptr ? iFont->castType<MyGUI::ResourceTrueTypeFont>(false) : nullptr;
				if (font == nullptr)
				{
					std::cerr << "Font not registered after load: " << fontName << "\n";
					mExitCode = 1;
					continue;
				}

				MyGUI::ITexture* tex = font->getTextureFont();
				if (tex == nullptr)
				{
					std::cerr << "Font has no texture: " << fontName << "\n";
					mExitCode = 1;
					continue;
				}

				const fs::path pngPath = mOutputXml.parent_path() / (fontName + ".png");
				tex->saveToFile(pngPath.string());
				std::cout << "Wrote " << pngPath << "\n";

				writeManualFont(outRoot, font, fontName, fontName + ".png", shader);
			}

			if (!outDoc.save_file(mOutputXml.string().c_str(), "\t"))
			{
				std::cerr << "Failed to write XML: " << mOutputXml << "\n";
				mExitCode = 1;
			}
			else
			{
				std::cout << "Wrote " << mOutputXml << "\n";
			}

			quit();
		}

		void destroyScene() override
		{
		}

	private:
		void setupResources() override
		{
			base::BaseManager::setupResources();
			addResourceLocation(mInputXml.parent_path().string());
			addResourceLocation(mOutputXml.parent_path().string());
			for (const auto& dir : mExtraDirs)
				addResourceLocation(dir);
		}

		fs::path mInputXml;
		fs::path mOutputXml;
		std::vector<std::string> mExtraDirs;
		int mExitCode = 0;
	};

} // namespace tools

int main(int argc, char** argv)
{
	if (argc < 3)
		return usage(argv[0]);

	const fs::path inputXml = argv[1];
	const fs::path outputXml = argv[2];

	if (!fs::exists(inputXml))
	{
		std::cerr << "Input XML not found: " << inputXml << "\n";
		return 1;
	}
	std::error_code ec;
	fs::create_directories(outputXml.parent_path(), ec);
	if (ec)
	{
		std::cerr << "Failed to create output dir: " << outputXml.parent_path() << ": " << ec.message() << "\n";
		return 1;
	}

	std::vector<std::string> extraDirs;
	for (int i = 3; i < argc; ++i)
		extraDirs.push_back(argv[i]);

	tools::Application app(inputXml, outputXml, std::move(extraDirs));
	app.prepare();
	if (app.create())
	{
		app.destroy();
	}
	return app.getExitCode();
}
