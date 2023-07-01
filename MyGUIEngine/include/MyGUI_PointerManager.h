/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_POINTER_MANAGER_H_
#define MYGUI_POINTER_MANAGER_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_Singleton.h"
#include "MyGUI_EventPair.h"
#include "MyGUI_IUnlinkWidget.h"
#include "MyGUI_ImageBox.h"
#include "MyGUI_IPointer.h"
#include "MyGUI_BackwardCompatibility.h"

namespace MyGUI
{

	class MYGUI_EXPORT PointerManager : public IUnlinkWidget, public MemberObsolete<PointerManager>
	{
		MYGUI_SINGLETON_DECLARATION(PointerManager);

	public:
		PointerManager();

		void initialise();
		void shutdown();

		/** Show or hide mouse pointer */
		void setVisible(bool _visible);
		/** Is mouse pointer visible */
		bool isVisible() const;

		/** Set pointer that will be shown
			@param _name of pointer
		*/
		void setPointer(std::string_view _name);
		/** Reset to default pointer */
		void resetToDefaultPointer();

		/** Get default pointer */
		const std::string& getDefaultPointer() const;
		/** Set default pointer */
		void setDefaultPointer(std::string_view _value);

		/** Get layer name where pointer rendered */
		const std::string& getLayerName() const;
		/** Set layer where pointer rendered */
		void setLayerName(std::string_view _value);

		/** Get pointer resource */
		IPointer* getByName(std::string_view _name) const;

		/*events:*/
		/** Event : Mouse pointer has been changed.\n
			signature : void method(std::string_view _pointerName)\n
			@param _pointerName Name of current mouse pointer
		*/
		EventPairConvertStringView<
			delegates::MultiDelegate<const std::string&>,
			delegates::MultiDelegate<std::string_view>>
			eventChangeMousePointer;

	private:
		void _unlinkWidget(Widget* _widget) override;
		void _load(xml::ElementPtr _node, std::string_view _file, Version _version);

		// создает виджет
		Widget* baseCreateWidget(
			WidgetStyle _style,
			std::string_view _type,
			std::string_view _skin,
			const IntCoord& _coord,
			Align _align,
			std::string_view _layer,
			std::string_view _name);

		// удаляет всех детей
		void _destroyAllChildWidget();

		void Update();

		void notifyFrameStart(float _time);
		void notifyChangeMouseFocus(Widget* _widget);
		void setPointer(std::string_view _name, Widget* _owner);

	private:
		// вектор всех детей виджетов
		VectorWidgetPtr mWidgetChild;

		std::string mDefaultName;
		IntPoint mPoint;
		IntPoint mOldPoint;
		bool mVisible{false};
		std::string mLayerName;
		std::string mSkinName;

		Widget* mWidgetOwner{nullptr};
		ImageBox* mMousePointer{nullptr};
		IPointer* mPointer{nullptr};
		std::string mCurrentMousePointer;

		bool mIsInitialise{false};
		std::string mXmlPointerTagName;
		std::string mXmlPropertyTagName;
		std::string mXmlDefaultPointerValue;
	};

} // namespace MyGUI

#endif // MYGUI_POINTER_MANAGER_H_
