/*!
	@file
	@author		Albert Semenov
	@date		04/2009
	@module
*/
#include "Utility.h"
#include "RenderBox/RenderBoxWrap.h"

namespace MyGUI
{
	namespace Managed
	{
		namespace Extension
		{

			public ref struct RenderBoxWrapper
			{
			public:
				RenderBoxWrapper(System::IntPtr _ptr)
				{
					MyGUI::WidgetPtr widget = static_cast<MyGUI::WidgetPtr>(_ptr.ToPointer());
					mRenderBox = widget->castType<MyGUI::RenderBox>();
					mWrapper = new wraps::RenderBoxWrap(mRenderBox);
				}

				~RenderBoxWrapper()
				{
					if (mWrapper != 0)
					{
						delete mWrapper;
						mWrapper = 0;
					}
				}

			private:
				wraps::RenderBoxWrap* mWrapper;
				MyGUI::RenderBoxPtr mRenderBox;

			public:
				void InjectObject(System::String ^ _file, float _posx, float _posy, float _posz, float _orw, float _orx, float _ory, float _orz)
				{
					mWrapper->injectObject(string_utility::managed_to_utf8(_file), Ogre::Vector3(_posx, _posy, _posz), Ogre::Quaternion(_orw, _orx, _ory, _orz));
				}

				void InjectObject(System::String ^ _file)
				{
					mWrapper->injectObject(string_utility::managed_to_utf8(_file), Ogre::Vector3::ZERO, Ogre::Quaternion::IDENTITY);
				}

				void ClearScene()
				{
					mWrapper->clearScene();
				}

				void SetAnimation(System::String ^ _name)
				{
					mWrapper->setAnimation(string_utility::managed_to_utf8(_name));
				}

				/*void SetBackgroungColour(float _red, float _green, float _blue, float _alpha)
				{
					mWrapper->setBackgroungColour(Ogre::ColourValue(_red, _green, _blue, _alpha));
				}*/

				property bool AutoRotation
				{
					bool get() { return mWrapper->getAutoRotation(); }
					void set(bool _value) { mWrapper->setAutoRotation(_value); }
				}

				property bool MouseRotation
				{
					bool get() { return mWrapper->getMouseRotation(); }
					void set(bool _value) { mWrapper->setMouseRotation(_value); }
				}

				property bool ViewScale
				{
					bool get() { return mWrapper->getViewScale(); }
					void set(bool _value) { mWrapper->setViewScale(_value); }
				}

				property float RotationAngle
				{
					float get() { return mWrapper->getRotationAngle().valueDegrees(); }
					void set(float _value) { mWrapper->setRotationAngle(Ogre::Degree(_value)); }
				}

			};

		} // namespace Extension
	} // namespace Managed
} // namespace MyGUI
