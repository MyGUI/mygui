#ifndef MYGUI_OSG_IMAGE_LOADER_H_
#define MYGUI_OSG_IMAGE_LOADER_H_

#include <osg/ref_ptr>
#include <string>

namespace osg
{

	class Image;

}

namespace MyGUI
{

	// PNG always uses osgdb_png, preserving straight-alpha RGB on every platform.
	// Other formats use osgDB's normal plugin selection. Loaded images retain their OSG origin.
	osg::ref_ptr<osg::Image> loadOsgImage(const std::string& _filename);
	bool saveOsgImage(const osg::Image& _image, const std::string& _filename);

}

#endif
