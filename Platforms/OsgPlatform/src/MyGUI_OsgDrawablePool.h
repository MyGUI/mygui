#pragma once

#include <osg/ref_ptr>
#include <osg/Camera>
#include <cstddef>
#include <utility>
#include <vector>

namespace MyGUI
{

	// Called only during update. Detached drawables owned solely by this pool
	// cannot be acquired by a cull/draw traversal and can be safely repopulated.
	template<typename T>
	class OsgDrawablePool : public osg::Referenced
	{
	public:
		std::vector<osg::ref_ptr<T>> retired;
		osg::ref_ptr<T> acquire(const T& _prototype)
		{
			osg::ref_ptr<T> next;
			size_t spareCount = 0;
			size_t spareBytes = 0;
			constexpr size_t spareByteLimit = 1024 * 1024;
			for (auto it = retired.begin(); it != retired.end();)
			{
				if ((*it)->referenceCount() == 1)
				{
					// Release arrays/textures promptly, but preserve vector capacity and VAOs.
					(*it)->clearBatches();
					if (!next)
					{
						next = std::move(*it);
						it = retired.erase(it);
						continue;
					}
					const size_t bytes = (*it)->getBatchCapacityBytes();
					if (spareCount == 2 || bytes > spareByteLimit - spareBytes)
					{
						it = retired.erase(it);
						continue;
					}
					++spareCount;
					spareBytes += bytes;
				}
				++it;
			}
			if (!next)
				next = new T(_prototype);
			return next;
		}
	};

	// Pooled drawables are detached from the scene graph, but their per-context
	// GL objects must still participate in normal scene/context teardown.
	template<typename T>
	class OsgPoolCamera : public osg::Camera
	{
	public:
		explicit OsgPoolCamera(OsgDrawablePool<T>* _pool) :
			mPool(_pool)
		{
		}
		void releaseGLObjects(osg::State* _state = nullptr) const override
		{
			osg::Camera::releaseGLObjects(_state);
			for (const auto& drawable : mPool->retired)
				drawable->releaseGLObjects(_state);
		}

	private:
		osg::ref_ptr<OsgDrawablePool<T>> mPool;
	};

}
