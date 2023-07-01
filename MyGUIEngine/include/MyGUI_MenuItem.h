/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_MENU_ITEM_H_
#define MYGUI_MENU_ITEM_H_

#include "MyGUI_Prerequest.h"
#include "MyGUI_Button.h"
#include "MyGUI_MenuControl.h"
#include "MyGUI_IItem.h"

namespace MyGUI
{

	/** \brief @wpage{MenuItem}
		MenuItem widget description should be here.
	*/
	class MYGUI_EXPORT MenuItem : public Button, public IItem, public MemberObsolete<MenuItem>
	{
		MYGUI_RTTI_DERIVED(MenuItem)

	public:
		/** @copydoc TextBox::setCaption(const UString& _value) */
		void setCaption(const UString& _value) override;

		/** @copydoc TextBox::setFontName(std::string_view _value) */
		void setFontName(std::string_view _value) override;

		/** @copydoc TextBox::setFontHeight(int _value) */
		void setFontHeight(int _value) override;

		//! Replace an item name
		void setItemName(const UString& _value);
		//! Get item name
		const UString& getItemName() const;

		//! Replace an item name
		void setItemData(Any _data);

		//! Get item data
		template<typename ValueType>
		ValueType* getItemData(bool _throw = true)
		{
			return mOwner->getItemData<ValueType>(this, _throw);
		}

		//! Remove item
		void removeItem();

		//! Replace an item id at a specified position
		void setItemId(std::string_view _id);
		//! Get item id from specified position
		const std::string& getItemId() const;

		//! Get item index
		size_t getItemIndex() const;

		/** Create child item (submenu), MenuItem can have only one child */
		MenuControl* createItemChild();

		/** Create specific type child item (submenu), MenuItem can have only one child */
		template<typename Type>
		Type* createItemChildT()
		{
			return mOwner->createItemChildT<Type>(this);
		}

		/** Set item type (see MenuItemType) */
		void setItemType(MenuItemType _type);
		/** Get item type (see MenuItemType) */
		MenuItemType getItemType() const;

		/** Hide or show child item (submenu) */
		void setItemChildVisible(bool _visible);

		/** Get parent MenuControl */
		MenuControl* getMenuCtrlParent() const;

		/** Get child item (submenu) */
		MenuControl* getItemChild() const;

		bool getItemChecked() const;
		void setItemChecked(bool _value);

		/*internal:*/
		IItemContainer* _getItemContainer() const override;
		IntSize _getContentSize() const;

	protected:
		void initialiseOverride() override;
		void shutdownOverride() override;

		void setPropertyOverride(std::string_view _key, std::string_view _value) override;

		void onWidgetCreated(Widget* _widget) override;

	private:
		void updateCheck();

	private:
		MenuControl* mOwner{nullptr};
		IntSize mMinSize{10, 10};
		Widget* mCheck{nullptr};
		bool mCheckValue{false};
	};

} // namespace MyGUI

#endif // MYGUI_MENU_ITEM_H_
