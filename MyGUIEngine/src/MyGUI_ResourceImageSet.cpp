/*!
	@file
	@author		Albert Semenov
	@date		09/2008
	@module
*/
#include "MyGUI_Prerequest.h"
#include "MyGUI_ResourceImageSet.h"
#include "MyGUI_ResourceManager.h"

namespace MyGUI
{

	//MYGUI_RESOURCE_IMPLEMENT(ResourceImageSet, IResource);

	std::string ResourceImageSet::mTextureEmpty;
	IntSize ResourceImageSet::mSizeEmpty;
	std::vector<IntPoint> ResourceImageSet::mFramesEmpty;

	ResourceImageSet::ResourceImageSet(xml::xmlNodeIterator _node) :
		IResource(_node)
	{
		// берем детей и крутимся, основной цикл
		xml::xmlNodeIterator group_node = _node->getNodeIterator();
		while (group_node.nextNode("Group")) {

			GroupImage group;
			group.name = group_node->findAttribute("name");
			group.texture = group_node->findAttribute("texture");
			group.size = IntSize::parse(group_node->findAttribute("size"));
			
			xml::xmlNodeIterator index_node = group_node->getNodeIterator();
			while (index_node.nextNode("Index")) {
				IndexImage index;
				index.name = index_node->findAttribute("name");
				index.rate = utility::parseFloat(index_node->findAttribute("rate"));

				xml::xmlNodeIterator frame_node = index_node->getNodeIterator();
				while (frame_node.nextNode("Frame")) {
					size_t count = utility::parseSizeT(frame_node->findAttribute("count"));
					const IntPoint & point = IntPoint::parse(frame_node->findAttribute("point"));
					if ((count < 1) || (count > 256)) count = 1;
					while (count > 0) {
						index.frames.push_back(point);
						-- count;
					};
				};

				group.indexes.push_back(index);
			};

			mGroups.push_back(group);
		};
	}

	ImageIndexInfo ResourceImageSet::getIndexInfo(const std::string & _group, const std::string & _index)
	{
		size_t index_group = getGroupIndex(_group);
		if (index_group != ITEM_NONE) {
			GroupImage & group = mGroups[index_group];
			size_t index_image = getImageIndex(group, _index);
			if (index_image != ITEM_NONE) {
				IndexImage & index = group.indexes[index_image];
				return ImageIndexInfo(group.texture, group.size, index.rate, index.frames);
			}
		}
		return ImageIndexInfo(mTextureEmpty, mSizeEmpty, 0, mFramesEmpty);
	}

	ImageIndexInfo ResourceImageSet::getIndexInfo(size_t _group, const std::string & _index)
	{
		if (_group < mGroups.size()) {
			GroupImage & group = mGroups[_group];
			size_t index_image = getImageIndex(group, _index);
			if (index_image != ITEM_NONE) {
				IndexImage & index = group.indexes[index_image];
				return ImageIndexInfo(group.texture, group.size, index.rate, index.frames);
			}
		}
		return ImageIndexInfo(mTextureEmpty, mSizeEmpty, 0, mFramesEmpty);
	}

	ImageIndexInfo ResourceImageSet::getIndexInfo(const std::string & _group, size_t _index)
	{
		size_t index_group = getGroupIndex(_group);
		if (index_group != ITEM_NONE) {
			GroupImage & group = mGroups[index_group];
			if (_index < group.indexes.size()) {
				IndexImage & index = group.indexes[_index];
				return ImageIndexInfo(group.texture, group.size, index.rate, index.frames);
			}
		}
		return ImageIndexInfo(mTextureEmpty, mSizeEmpty, 0, mFramesEmpty);
	}

	ImageIndexInfo ResourceImageSet::getIndexInfo(size_t _group, size_t _index)
	{
		if (_group < mGroups.size()) {
			GroupImage & group = mGroups[_group];
			if (_index < group.indexes.size()) {
				IndexImage & index = group.indexes[_index];
				return ImageIndexInfo(group.texture, group.size, index.rate, index.frames);
			}
		}
		return ImageIndexInfo(mTextureEmpty, mSizeEmpty, 0, mFramesEmpty);
	}

	ImageIndexInfo ResourceImageSet::getIndexInfo(const IntSize & _group, size_t _index)
	{
		size_t index_group = getGroupIndex(_group);
		if (index_group != ITEM_NONE) {
			GroupImage & group = mGroups[index_group];
			if (_index < group.indexes.size()) {
				IndexImage & index = group.indexes[_index];
				return ImageIndexInfo(group.texture, group.size, index.rate, index.frames);
			}
		}
		return ImageIndexInfo(mTextureEmpty, mSizeEmpty, 0, mFramesEmpty);
	}

	ImageIndexInfo ResourceImageSet::getIndexInfo(const IntSize & _group, const std::string & _index)
	{
		size_t index_group = getGroupIndex(_group);
		if (index_group != ITEM_NONE) {
			GroupImage & group = mGroups[index_group];
			size_t index_image = getImageIndex(group, _index);
			if (index_image != ITEM_NONE) {
				IndexImage & index = group.indexes[index_image];
				return ImageIndexInfo(group.texture, group.size, index.rate, index.frames);
			}
		}
		return ImageIndexInfo(mTextureEmpty, mSizeEmpty, 0, mFramesEmpty);
	}

} // namespace MyGUI
