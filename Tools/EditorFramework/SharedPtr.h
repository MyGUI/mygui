/*!
	@file
	@author		Albert Semenov
	@date		07/2012
*/

#ifndef _e34f5767_b55f_4017_bcb3_634673f41b2b_
#define _e34f5767_b55f_4017_bcb3_634673f41b2b_

namespace tools
{

#if __cplusplus >= 201103L || defined(EMSCRIPTEN)
	template <typename T> using weak_ptr = std::weak_ptr<T>;
	template <typename T> using shared_ptr = std::shared_ptr<T>;
#else
	template <typename T> class weak_ptr;

	template <typename T>
	class shared_ptr
	{
		friend class weak_ptr<T>;

	public:
		shared_ptr() : ptr(0), count(create_nil_counter()) { incref(); }
		~shared_ptr() { decref(); }

		shared_ptr(const shared_ptr<T>& o) : ptr(o.ptr), count(o.count) { incref(); }
		shared_ptr(T* p) : ptr(p), count(new unsigned(1)) { }
		explicit shared_ptr(const weak_ptr<T>& w) : ptr(w.ptr), count(w.count) { incref(); }

		shared_ptr<T>& operator=(T* o)
		{
			if (ptr == o)
				return *this;
			decref();
			ptr = o;
			incref();
			return *this;
		}

		shared_ptr<T>& operator=(const shared_ptr<T>& o)
		{
			if (ptr == o.ptr)
				return *this;
			decref();
			ptr = o.ptr;
			count = o.count;
			incref();
			return *this;
		}

		T* get() { return ptr; }
		T* operator->() { return ptr; }
		T& operator*()  { return *ptr; }

		const T* get() const { return ptr; }
		const T* operator->() const { return ptr; }
		const T& operator*() const { return *ptr; }

		// in C++11, we can't implicitly convert nullptr -> shared_ptr, so need to define pointer comparison operators
		bool operator==(const T* o) const { return ptr == o; }
		bool operator!=(const T* o) const { return ptr != o; }
		bool operator<(const T* o) const { return ptr < o; }

		bool operator==(const shared_ptr<T>& o) const { return ptr == o.ptr; }
		bool operator!=(const shared_ptr<T>& o) const { return ptr != o.ptr; }
		bool operator<(const shared_ptr<T>& o) const { return ptr < o.ptr; }

		unsigned refcount() const { return *count; }

	private:
		/* special case, null pointer (nil-code) */
		static unsigned* create_nil_counter() { static unsigned nil_counter(1); return &nil_counter; }

		void decref() { if (--(*count) == 0) { delete ptr; delete count; } }
		void incref() { ++(*count); }

	private:
		T* ptr;
		unsigned* count;
	};

	template <typename T>
	class weak_ptr
	{
		friend class shared_ptr<T>;

	public:
		weak_ptr() : ptr(0), count(shared_ptr<T>::create_nil_counter()) { }
		explicit weak_ptr( const shared_ptr<T>& s) : ptr(s.ptr), count(s.count) { }

		shared_ptr<T> lock() const { return shared_ptr<T>(*this); }


		T* get() { return ptr; }
		T* operator->() { return ptr; }
		T& operator*()  { return *ptr; }

		const T* get() const { return ptr; }
		const T* operator->() const { return ptr; }
		const T& operator*() const { return *ptr; }

		bool operator==(const T* o) const { return ptr == o; }
		bool operator!=(const T* o) const { return ptr != o; }
		bool operator<(const T* o) const { return ptr < o; }

		bool operator==(const shared_ptr<T>& o) const { return ptr == o.ptr; }
		bool operator!=(const shared_ptr<T>& o) const { return ptr != o.ptr; }
		bool operator<(const shared_ptr<T>& o) const { return ptr < o.ptr; }

		unsigned refcount() const { return *count; }

	private:
		T* ptr;
		unsigned* count;
	};
#endif

}

#endif
