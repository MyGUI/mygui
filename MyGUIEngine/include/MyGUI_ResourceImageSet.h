/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_RESOURCE_IMAGE_SET_H_
#define MYGUI_RESOURCE_IMAGE_SET_H_

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

#endif // MYGUI_RESOURCE_IMAGE_SET_H_
