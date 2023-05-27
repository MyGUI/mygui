/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _54864042_de71_4232_9301_bc7ffdf3357a_
#define _54864042_de71_4232_9301_bc7ffdf3357a_

#include <MyGUI.h>

namespace tools
{

	class AnimationInfo
	{
	public:
		const std::string& getTextureName() const;
		void setTextureName(std::string_view _value);

		const MyGUI::IntSize& getSize() const;
		void setSize(const MyGUI::IntSize& _value);

		float getRate() const;
		void setRate(float _value);

		using PairFrame = std::pair<MyGUI::IntPoint, size_t>;
		using VectorFrame = std::vector<PairFrame>;
		const VectorFrame& getFrames() const;
		void addFrame(const MyGUI::IntPoint& _frame, size_t _count);
		void clearFrames();

	private:
		std::string mAnimationName;
		MyGUI::IntSize mSize;
		float mRate{0};
		VectorFrame mFrames;
	};

}

#endif
