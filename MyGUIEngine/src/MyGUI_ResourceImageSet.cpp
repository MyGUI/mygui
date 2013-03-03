/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#include "MyGUI_Precompiled.h"
#include "MyGUI_ResourceImageSet.h"
#include "MyGUI_ResourceManager.h"
#include "MyGUI_LanguageManager.h"
#include "MyGUI_Constants.h"

namespace MyGUI
{

	std::vector<IntPoint> ResourceImageSet::mFramesEmpty;

	ResourceImageSet::ResourceImageSet()
	{
	}

	ResourceImageSet::~ResourceImageSet()
	{
	}

	void ResourceImageSet::deserialization(xml::ElementPtr _node, Version _version)
	{
		Base::deserialization(_node, _version);

		// берем детей и крутимся, основной цикл
		xml::ElementEnumerator group_node = _node->getElementEnumerator();
		while (group_node.next("Group"))
		{
			GroupImage group;
			group.name = group_node->findAttribute("name");

			group.texture = group_node->findAttribute("texture");
			// поддержка замены тегов
			if (_version >= Version(1, 1))
			{
				group.texture = LanguageManager::getInstance().replaceTags(group.texture);
			}

			group.size = IntSize::parse(group_node->findAttribute("size"));

			xml::ElementEnumerator index_node = group_node->getElementEnumerator();
			while (index_node.next("Index"))
			{
				IndexImage index;
				index.name = index_node->findAttribute("name");
				index.rate = utility::parseFloat(index_node->findAttribute("rate"));

				xml::ElementEnumerator frame_node = index_node->getElementEnumerator();
				while (frame_node.next("Frame"))
				{
					size_t count = utility::parseSizeT(frame_node->findAttribute("count"));
					const IntPoint& point = IntPoint::parse(frame_node->findAttribute("point"));
					if ((count < 1) || (count > 256)) count = 1;
					while (count > 0)
					{
						index.frames.push_back(point);
						-- count;
					}
				}

				group.indexes.push_back(index);
			}

			AddGroupImage(group);
		}
	}

	ImageIndexInfo ResourceImageSet::getIndexInfo(const std::string& _group, const std::string& _index)
	{
		size_t index_group = getGroupIndex(_group);
		if (index_group != ITEM_NONE)
		{
			GroupImage& group = mGroups[index_group];
			size_t index_image = getImageIndex(group, _index);
			if (index_image != ITEM_NONE)
			{
				IndexImage& index = group.indexes[index_image];
				return ImageIndexInfo(group.texture, group.size, index.rate, index.frames);
			}
		}
		return ImageIndexInfo(Constants::getEmptyString(), Constants::getZeroIntSize(), 0, mFramesEmpty);
	}

	ImageIndexInfo ResourceImageSet::getIndexInfo(size_t _group, const std::string& _index)
	{
		if (_group < mGroups.size())
		{
			GroupImage& group = mGroups[_group];
			size_t index_image = getImageIndex(group, _index);
			if (index_image != ITEM_NONE)
			{
				IndexImage& index = group.indexes[index_image];
				return ImageIndexInfo(group.texture, group.size, index.rate, index.frames);
			}
		}
		return ImageIndexInfo(Constants::getEmptyString(), Constants::getZeroIntSize(), 0, mFramesEmpty);
	}

	ImageIndexInfo ResourceImageSet::getIndexInfo(const std::string& _group, size_t _index)
	{
		size_t index_group = getGroupIndex(_group);
		if (index_group != ITEM_NONE)
		{
			GroupImage& group = mGroups[index_group];
			if (_index < group.indexes.size())
			{
				IndexImage& index = group.indexes[_index];
				return ImageIndexInfo(group.texture, group.size, index.rate, index.frames);
			}
		}
		return ImageIndexInfo(Constants::getEmptyString(), Constants::getZeroIntSize(), 0, mFramesEmpty);
	}

	ImageIndexInfo ResourceImageSet::getIndexInfo(size_t _group, size_t _index)
	{
		if (_group < mGroups.size())
		{
			GroupImage& group = mGroups[_group];
			if (_index < group.indexes.size())
			{
				IndexImage& index = group.indexes[_index];
				return ImageIndexInfo(group.texture, group.size, index.rate, index.frames);
			}
		}
		return ImageIndexInfo(Constants::getEmptyString(), Constants::getZeroIntSize(), 0, mFramesEmpty);
	}

	ImageIndexInfo ResourceImageSet::getIndexInfo(const IntSize& _group, size_t _index)
	{
		size_t index_group = getGroupIndex(_group);
		if (index_group != ITEM_NONE)
		{
			GroupImage& group = mGroups[index_group];
			if (_index < group.indexes.size())
			{
				IndexImage& index = group.indexes[_index];
				return ImageIndexInfo(group.texture, group.size, index.rate, index.frames);
			}
		}
		return ImageIndexInfo(Constants::getEmptyString(), Constants::getZeroIntSize(), 0, mFramesEmpty);
	}

	ImageIndexInfo ResourceImageSet::getIndexInfo(const IntSize& _group, const std::string& _index)
	{
		size_t index_group = getGroupIndex(_group);
		if (index_group != ITEM_NONE)
		{
			GroupImage& group = mGroups[index_group];
			size_t index_image = getImageIndex(group, _index);
			if (index_image != ITEM_NONE)
			{
				IndexImage& index = group.indexes[index_image];
				return ImageIndexInfo(group.texture, group.size, index.rate, index.frames);
			}
		}
		return ImageIndexInfo(Constants::getEmptyString(), Constants::getZeroIntSize(), 0, mFramesEmpty);
	}

	size_t ResourceImageSet::getGroupIndex(const std::string& _name)
	{
		for (size_t index = 0; index < mGroups.size(); ++index)
		{
			if (mGroups[index].name == _name)
				return index;
		}
		return ITEM_NONE;
	}

	size_t ResourceImageSet::getGroupIndex(const IntSize& _size)
	{
		for (size_t index = 0; index < mGroups.size(); ++index)
		{
			if (mGroups[index].size == _size)
				return index;
		}
		return ITEM_NONE;
	}

	size_t ResourceImageSet::getImageIndex(GroupImage& _group, const std::string& _name)
	{
		VectorIndexImage& indexes = _group.indexes;
		for (size_t index = 0; index < indexes.size(); ++index)
		{
			if (indexes[index].name == _name)
				return index;
		}
		return ITEM_NONE;
	}

	const IntSize& ResourceImageSet::getGroupSize(size_t _index)
	{
		if (_index >= mGroups.size())
			return Constants::getZeroIntSize();
		return mGroups[_index].size;
	}

	const IntSize& ResourceImageSet::getGroupSize(const std::string& _group)
	{
		for (size_t index = 0; index < mGroups.size(); ++index)
		{
			if (mGroups[index].name == _group)
				return mGroups[index].size;
		}
		return Constants::getZeroIntSize();
	}

	EnumeratorGroupImage ResourceImageSet::getEnumerator() const
	{
		return EnumeratorGroupImage(mGroups);
	}

	void ResourceImageSet::AddGroupImage(const GroupImage& _group)
	{
		mGroups.push_back(_group);
	}

} // namespace MyGUI
