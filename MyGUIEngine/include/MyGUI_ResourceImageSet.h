/*!
	@file
	@author		Albert Semenov
	@date		09/2008
*/
/*
	This file is part of MyGUI.

	MyGUI is free software: you can redistribute it and/or modify
	it under the terms of the GNU Lesser General Public License as published by
	the Free Software Foundation, either version 3 of the License, or
	(at your option) any later version.

	MyGUI is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU Lesser General Public License for more details.

	You should have received a copy of the GNU Lesser General Public License
	along with MyGUI.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef __MYGUI_RESOURCE_IMAGE_SET_H__
#define __MYGUI_RESOURCE_IMAGE_SET_H__

#include "MyGUI_Prerequest.h"
#include "MyGUI_Macros.h"
#include "MyGUI_XmlDocument.h"
#include "MyGUI_IResource.h"
#include "MyGUI_ImageInfo.h"
#include "MyGUI_Enumerator.h"
#include "MyGUI_ResourceManager.h"
#include "MyGUI_GenericFactory.h"
#include "MyGUI_ResourceImageSetData.h"

namespace MyGUI
{

	class ResourceImageSet;
	typedef ResourceImageSet* ResourceImageSetPtr;

	class MYGUI_EXPORT ResourceImageSet :
		public IResource
	{
		friend class GenericFactory<ResourceImageSet>;

		MYGUI_RTTI_DERIVED( ResourceImageSet )

	public:
		ImageIndexInfo getIndexInfo(const std::string& _group, const std::string& _index);
		ImageIndexInfo getIndexInfo(size_t _group, const std::string& _index);
		ImageIndexInfo getIndexInfo(const std::string& _group, size_t _index);
		ImageIndexInfo getIndexInfo(size_t _group, size_t _index);
		ImageIndexInfo getIndexInfo(const IntSize& _group, size_t _index);
		ImageIndexInfo getIndexInfo(const IntSize& _group, const std::string& _index);

		/** Get groups Enumerator */
		EnumeratorGroupImage getEnumerator() const;

		void AddGroupImage(const GroupImage& _group);

	private:
		ResourceImageSet();
		virtual ~ResourceImageSet();

		virtual void deserialization(xml::ElementPtr _node, Version _version);

		size_t getGroupIndex(const std::string& _name);
		size_t getGroupIndex(const IntSize& _size);
		size_t getImageIndex(GroupImage& _group, const std::string& _name);
		const IntSize& getGroupSize(size_t _index);
		const IntSize& getGroupSize(const std::string& _group);

	private:
		VectorGroupImage mGroups;

		static std::vector<IntPoint> mFramesEmpty;
	};

} // namespace MyGUI

#endif // __MYGUI_RESOURCE_IMAGE_SET_H__
