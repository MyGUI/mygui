#ifndef __VERTEXT_BUFFER_H__
#define __VERTEXT_BUFFER_H__

#include <Ogre.h>
#include <MyGUI.h>

namespace demo
{
	class RenderManager;

	class VertexBuffer
	{
	public:
		VertexBuffer();
		virtual ~VertexBuffer();

		const std::string& getTextureName();
		void setTextureName(const std::string& _value);

		Ogre::RenderOperation& getRenderOperation();

		bool getEmpty();

		const MyGUI::IntCoord& getCoord();
		void setCoord(const MyGUI::IntCoord& _value);

		void doRender(RenderManager* _renderManager);

		void setManualMode(bool _value);

	private:
		void createVertexBuffer();
		void destroyVertexBuffer();

		void update();

	private:
		std::string mTextureName;
		Ogre::RenderOperation mRenderOperation;
		Ogre::HardwareVertexBufferSharedPtr mVertexBuffer;
		size_t mVertexCount;
		MyGUI::IntCoord mCoord;
		MyGUI::FloatRect mCurrentTexture;
		MyGUI::uint32 mCurrentColour;
		bool mManualMode;
	};

} // namespace demo

#endif // __VERTEXT_BUFFER_H__
