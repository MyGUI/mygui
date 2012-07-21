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
		AnimationInfo();
		~AnimationInfo();

		const std::string& getTextureName() const;
		void setTextureName(const std::string& _value);

		const MyGUI::IntSize& getSize() const;
		void setSize(const MyGUI::IntSize& _value);

		float getRate() const;
		void setRate(float _value);

		typedef std::pair<MyGUI::IntPoint, size_t> PairFrame;
		typedef std::vector<PairFrame> VectorFrame;
		const VectorFrame& getFrames() const;
		void addFrame(const MyGUI::IntPoint& _frame, size_t _count);
		void clearFrames();

	private:
		std::string mAnimationName;
		MyGUI::IntSize mSize;
		float mRate;
		VectorFrame mFrames;
	};

}

#endif
