/*
 * This source file is part of MyGUI. For the latest info, see http://mygui.info/
 * Distributed under the MIT License
 * (See accompanying file COPYING.MIT or copy at http://opensource.org/licenses/MIT)
 */

#ifndef MYGUI_ALLOCATOR_H_
#define MYGUI_ALLOCATOR_H_

#include <memory>
#include <limits>

namespace MyGUI
{

	template<typename T>
	class Allocator
	{
	public:
		//    typedefs
		typedef T value_type;
		typedef value_type* pointer;
		typedef const value_type* const_pointer;
		typedef value_type& reference;
		typedef const value_type& const_reference;
		typedef std::size_t size_type;
		typedef std::ptrdiff_t difference_type;

	public:
		//    convert an allocator<T> to allocator<U>
		template<typename U>
		struct rebind
		{
			typedef Allocator<U> other;
		};

	public:
		inline explicit Allocator() { }
		inline ~Allocator() { }
		template<typename U>
		inline explicit Allocator(Allocator<U> const&) { }

		//    address
		inline pointer address(reference r)
		{
			return &r;
		}
		inline const_pointer address(const_reference r)
		{
			return &r;
		}

		//    memory allocation
		inline pointer allocate(size_type cnt, typename std::allocator<void>::const_pointer = 0)
		{
			return reinterpret_cast<pointer>(::operator new (cnt * sizeof (T)));
		}
		inline void deallocate(pointer p, size_type)
		{
			::operator delete (p);
		}

		//    size
		inline size_type max_size() const
		{
			return (std::numeric_limits<size_type>::max)() / sizeof(T);
		}

		//    construction/destruction
		inline void construct(pointer p, const T& t)
		{
			new (p) T(t);
		}
		inline void destroy(pointer p)
		{
			p->~T();
		}

		inline bool operator==(Allocator const&)
		{
			return true;
		}
		inline bool operator!=(Allocator const& a)
		{
			return !operator==(a);
		}
	};

} // namespace MyGUI

#endif // MYGUI_ALLOCATOR_H_
