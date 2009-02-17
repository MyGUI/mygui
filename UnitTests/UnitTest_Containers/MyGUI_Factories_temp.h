
#include "MyGUI_Prerequest.h"
#include "MyGUI_FlowContainer.h"
#include "MyGUI_IWidgetFactory.h"

namespace MyGUI
{
	namespace factory
	{
		// DELETE
		template <typename T>
		class NonDllBaseWidgetFactory : public IWidgetFactory
		{
		public:
			NonDllBaseWidgetFactory()
			{
				// регестрируем себя
				MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
				manager.registerFactory(this);
			}

			~NonDllBaseWidgetFactory()
			{
				// удаляем себя
				MyGUI::WidgetManager & manager = MyGUI::WidgetManager::getInstance();
				manager.unregisterFactory(this);
			}

			const std::string & getTypeName()
			{
				return T::getClassTypeName();
			}

			WidgetPtr createWidget(WidgetStyle _style, const std::string& _skin, const IntCoord& _coord, Align _align, WidgetPtr _parent, ICroppedRectangle * _croppedParent, IWidgetCreator * _creator, const std::string& _name)
			{
				return new T(_style, _coord, _align, SkinManager::getInstance().getSkin(_skin), _parent, _croppedParent, _creator, _name);
			}

			bool isFalseType(WidgetPtr _ptr, const std::string &_key)
			{
				if (!_ptr->isType<T>()) {
					MYGUI_LOG(Error, "Property '" << _key << "' is not supported by '" << _ptr->getTypeName() << "' widget");
					return true;
				}
				return false;
			}
		};

		class /*MYGUI_EXPORT*/ FlowContainerFactory : public NonDllBaseWidgetFactory< FlowContainer >
		{
		public:
			
		};

		class /*MYGUI_EXPORT*/ SpacerFactory : public NonDllBaseWidgetFactory< Spacer >
		{
		public:
			
		};

	} // namespace factory
}