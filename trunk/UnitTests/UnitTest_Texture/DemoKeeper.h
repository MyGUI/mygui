/*!
	@file
	@author		Albert Semenov
	@date		08/2008
	@module
*/
#ifndef __DEMO_KEEPER_H__
#define __DEMO_KEEPER_H__

#include "Base/BaseManager.h"
#include "MyGUI_Texture.h"

namespace demo
{

	struct PaintInfo
	{
		int x, y;
		Ogre::ColourValue colour;
	};
	typedef std::vector<PaintInfo> VectorPintInfo;

	class DemoKeeper : public base::BaseManager
	{
	public:
		virtual void createScene();
		virtual void destroyScene();
		virtual bool keyPressed( const OIS::KeyEvent &arg );

	private:
		void requestUpdateTexture2( MyGUI::TexturePtr texture );

		MyGUI::TexturePtr mTexture1;
		MyGUI::TexturePtr mTexture2;

		MyGUI::StaticImagePtr mImage1;
		MyGUI::StaticImagePtr mImage2;

		VectorPintInfo mPaintData;

	};

} // namespace demo

#endif // __DEMO_KEEPER_H__
