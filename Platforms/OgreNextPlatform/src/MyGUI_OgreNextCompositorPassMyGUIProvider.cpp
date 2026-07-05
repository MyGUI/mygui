#include "MyGUI_OgreNextCompositorPassMyGUIProvider.h"

#include "MyGUI_OgreNextCompositorPassMyGUI.h"
#include "MyGUI_OgreNextCompositorPassMyGUIDef.h"

#include <OgreScriptCompiler.h>
#include <OgreScriptTranslator.h>

namespace MyGUI
{
	const Ogre::IdString CompositorPassMyGUIProvider::kCustomId("mygui");

	Ogre::CompositorPassDef* CompositorPassMyGUIProvider::addPassDef(
		Ogre::CompositorPassType /*passType*/,
		Ogre::IdString customId,
		Ogre::CompositorTargetDef* parentTargetDef,
		Ogre::CompositorNodeDef* /*parentNodeDef*/)
	{
		if (customId == kCustomId)
			return OGRE_NEW CompositorPassMyGUIDef(parentTargetDef);
		return nullptr;
	}

	Ogre::CompositorPass* CompositorPassMyGUIProvider::addPass(
		const Ogre::CompositorPassDef* definition,
		Ogre::Camera* defaultCamera,
		Ogre::CompositorNode* parentNode,
		const Ogre::RenderTargetViewDef* rtvDef,
		Ogre::SceneManager* sceneManager)
	{
		if (definition->getCustomId() != kCustomId.getU32Value())
			return nullptr;

		OGRE_ASSERT_HIGH(dynamic_cast<const CompositorPassMyGUIDef*>(definition));
		const auto* myguiDef = static_cast<const CompositorPassMyGUIDef*>(definition);
		return OGRE_NEW CompositorPassMyGUI(myguiDef, defaultCamera, sceneManager, rtvDef, parentNode);
	}

	namespace
	{
		bool scriptTranslatorGetBoolean(const Ogre::AbstractNodePtr& node, bool* result)
		{
			if (node->type != Ogre::ANT_ATOM)
				return false;
			const auto* atom = static_cast<const Ogre::AtomAbstractNode*>(node.get());
			if (atom->id == 1 || atom->id == 2)
			{
				*result = (atom->id == 1);
				return true;
			}
			return false;
		}
	}

	void CompositorPassMyGUIProvider::translateCustomPass(
		Ogre::ScriptCompiler* compiler,
		const Ogre::AbstractNodePtr& node,
		Ogre::IdString customId,
		Ogre::CompositorPassDef* customPassDef)
	{
		if (customId != kCustomId)
			return;

		auto* myguiDef = static_cast<CompositorPassMyGUIDef*>(customPassDef);

		auto* obj = reinterpret_cast<Ogre::ObjectAbstractNode*>(node.get());
		obj->context = Ogre::Any(static_cast<Ogre::CompositorPassDef*>(myguiDef));

		for (const auto& child : obj->children)
		{
			if (child->type == Ogre::ANT_OBJECT)
			{
				auto* childObj = reinterpret_cast<Ogre::ObjectAbstractNode*>(child.get());
				if (childObj->id == Ogre::ID_LOAD)
				{
					Ogre::CompositorLoadActionTranslator t;
					t.translate(compiler, child);
				}
				else if (childObj->id == Ogre::ID_STORE)
				{
					Ogre::CompositorStoreActionTranslator t;
					t.translate(compiler, child);
				}
			}
			else if (child->type == Ogre::ANT_PROPERTY)
			{
				const auto* prop = reinterpret_cast<const Ogre::PropertyAbstractNode*>(child.get());
				if (prop->name == "sets_resolution")
				{
					if (prop->values.size() != 1 ||
						!scriptTranslatorGetBoolean(prop->values.front(), &myguiDef->mSetsResolution))
					{
						compiler->addError(
							Ogre::ScriptCompiler::CE_STRINGEXPECTED,
							obj->file,
							obj->line,
							"sets_resolution expects a boolean value");
					}
				}
			}
		}
	}

} // namespace MyGUI
