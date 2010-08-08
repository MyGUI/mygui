/*!
	@file
	@author		Albert Semenov
	@date		08/2010
*/
#ifndef __PROPERTY_ADVISOR_H__
#define __PROPERTY_ADVISOR_H__

#include <MyGUI.h>
#include "SkinManager.h"

namespace tools
{

	class PropertyAdvisor
	{
	public:
		PropertyAdvisor() :
			mCurrentSkin(nullptr),
			mCurrentState(nullptr)
		{
		}

		virtual ~PropertyAdvisor()
		{
		}

		void initialiseAdvisor()
		{
			SkinManager::getInstance().eventChangeSelection += MyGUI::newDelegate(this, &PropertyAdvisor::notifySkinChangeSelection);
			adviceSkin();

			updateSkinProperties();
			updateStateProperties();
		}

		void shutdownAdvisor()
		{
			SkinManager::getInstance().eventChangeSelection -= MyGUI::newDelegate(this, &PropertyAdvisor::notifySkinChangeSelection);
			unadviceSkin();
		}

		virtual void updateSkinProperties() { }
		virtual void updateStateProperties() { }

		virtual void updateSkinProperty(Property* _sender, const MyGUI::UString& _owner) { }
		virtual void updateStateProperty(Property* _sender, const MyGUI::UString& _owner) { }

		SkinItem* getCurrentSkin() { return mCurrentSkin; }
		StateItem* getCurrentState() { return mCurrentState; }

	private:
		void notifySkinChangeSelection()
		{
			unadviceSkin();
			adviceSkin();

			updateSkinProperties();
			updateStateProperties();
		}

		void unadviceSkin()
		{
			if (mCurrentSkin != nullptr)
			{
				unadviceState();

				mCurrentSkin->getPropertySet()->eventChangeProperty -= MyGUI::newDelegate(this, &PropertyAdvisor::updateSkinProperty);
				mCurrentSkin->getStates().eventChangeSelection -= MyGUI::newDelegate(this, &PropertyAdvisor::notifyStateChangeSelection);
				mCurrentSkin = nullptr;
			}
		}

		void adviceSkin()
		{
			mCurrentSkin = SkinManager::getInstance().getItemSelected();

			if (mCurrentSkin != nullptr)
			{
				mCurrentSkin->getStates().eventChangeSelection += MyGUI::newDelegate(this, &PropertyAdvisor::notifyStateChangeSelection);
				mCurrentSkin->getPropertySet()->eventChangeProperty += MyGUI::newDelegate(this, &PropertyAdvisor::updateSkinProperty);

				adviceState();
			}
		}

		void notifyStateChangeSelection()
		{
			unadviceState();
			adviceState();

			updateStateProperties();
		}

		void adviceState()
		{
			mCurrentState = mCurrentSkin->getStates().getItemSelected();
			if (mCurrentState != nullptr)
			{
				mCurrentState->getPropertySet()->eventChangeProperty += MyGUI::newDelegate(this, &PropertyAdvisor::updateStateProperty);
			}
		}

		void unadviceState()
		{
			if (mCurrentState != nullptr)
			{
				mCurrentState->getPropertySet()->eventChangeProperty -= MyGUI::newDelegate(this, &PropertyAdvisor::updateStateProperty);
				mCurrentState = nullptr;
			}
		}

	private:
		SkinItem* mCurrentSkin;
		StateItem* mCurrentState;
	};

} // namespace tools

#endif // __PROPERTY_ADVISOR_H__
