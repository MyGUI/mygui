/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#include "Precompiled.h"
#include "AnimationInfo.h"

namespace tools
{

	const std::string& AnimationInfo::getTextureName() const
	{
		return mAnimationName;
	}

	void AnimationInfo::setTextureName(std::string_view _value)
	{
		mAnimationName = _value;
	}

	const MyGUI::IntSize& AnimationInfo::getSize() const
	{
		return mSize;
	}

	void AnimationInfo::setSize(const MyGUI::IntSize& _value)
	{
		mSize = _value;
	}

	float AnimationInfo::getRate() const
	{
		return mRate;
	}

	void AnimationInfo::setRate(float _value)
	{
		mRate = _value;
	}

	const AnimationInfo::VectorFrame& AnimationInfo::getFrames() const
	{
		return mFrames;
	}

	void AnimationInfo::addFrame(const MyGUI::IntPoint& _frame, size_t _count)
	{
		_count = _count < 1 ? 1 : _count;
		mFrames.emplace_back(_frame, _count);
	}

	void AnimationInfo::clearFrames()
	{
		mFrames.clear();
	}

}
