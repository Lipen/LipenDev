/* Copyright (c) 2016, Lipen */
#include <iostream>  // cout, endl
#include <utility>   // swap
// TAGS: smart pointers, shared_ptr, weak_ptr, shared pointer, weak pointer

#include "../TValidator.ipp"

using std::cout;
using std::endl;

template<typename T>
class weak_ptr;


class shared_ptr_count {
	long* ref_count;
	// nullptr means non-acquired/released,
	// amount of references to object otherwise

 public:
 	/* Default ctor */
	shared_ptr_count()
	: ref_count(nullptr)
	{
		cout << "shared_ptr_count :: Default ctor" << endl;
	}

	/* Copy ctor */
	shared_ptr_count(const shared_ptr_count& rhs)
	: ref_count(rhs.ref_count)
	{
		cout << "shared_ptr_count :: Copy ctor("; if (ref_count) cout << ref_count << " = " << *ref_count; else cout << "nullptr"; cout << ")" << endl;
	}


	/* Swap method for copy-and-swap idiom */
	void swap(shared_ptr_count& rhs) noexcept {
		cout << "shared_ptr_count :: Swap" << endl;
		std::swap(ref_count, rhs.ref_count);
	}

	/* Getter for reference counter */
	long use_count() const noexcept {
		return ref_count ? *ref_count : 0;
	}

	/* Acquire/share pointer`s ownership, initializing the reference counter */
	template<typename U>
	void acquire(U* p) noexcept(false) {  // may throw bad_alloc
		cout << "shared_ptr_count :: Acquire(";if (p) cout << p << " = " << *p; else cout << "nullptr"; cout << ")" << endl;

		if (p != nullptr) {
			if (ref_count == nullptr) {
				// TODO: check for std::bad_alloc
				ref_count = new long(1);
			}
			else {
				++(*ref_count);
			}
		}
	}

	/* Release pointer`s ownership, destroying the object when appropriate */
	template<typename U>
	void release(U* p) noexcept {
		cout << "shared_ptr_count :: Release(";if (p) cout << p << " = " << *p; else cout << "nullptr"; cout << ")" << endl;
		if (ref_count != nullptr) {
			--(*ref_count);
			if (*ref_count == 0) {
				delete p;
				delete ref_count;
			}
			ref_count = nullptr;
		}
	}
};


template<typename T>
class shared_ptr {
	T* data;
	shared_ptr_count spc;

	friend class weak_ptr<T>;

 public:
	/* Default ctor */
	shared_ptr(void)
	: data(nullptr), spc()
	{
		cout << "shared_ptr :: Default ctor" << endl;
	}

	/* Standard ctor */
	explicit shared_ptr(T* p)
	: spc()
	{
		cout << "shared_ptr :: Standard T* ctor" << endl;
		acquire(p);
	}

	/* Ctor for weak pointer lock */
	template<typename U>
	explicit shared_ptr(const weak_ptr<U>& rhs)
	: spc(rhs.spc)
	{
		cout << "shared_ptr :: Ctor for weak pointer lock" << endl;
		acquire(rhs.data);
	}

	/* Copy ctor for copy-and-swap idiom */
	shared_ptr(const shared_ptr& rhs) noexcept
	: spc(rhs.spc)
	{
		cout << "shared_ptr :: Copy ctor" << endl;
		acquire(rhs.data);
	}

	/* Destructor */
	~shared_ptr() {
		cout << "~shared_ptr :: Destructor" << endl;
		release();
	}


	/* Assign operator using copy-and-swap idiom */
	shared_ptr& operator= (shared_ptr rhs) noexcept {
		cout << "shared_ptr :: operator=" << endl;
		swap(rhs);
		return *this;
	}

	/* Swap method for copy-and-swap idiom */
	void swap(shared_ptr& rhs) noexcept {
		cout << "shared_ptr :: Swap" << endl;
		std::swap(data, rhs.data);
		spc.swap(rhs.spc);
	}

	/* Reset */
	void reset(void) noexcept {
		cout << "shared_ptr :: Reset(void)" << endl;
		release();
	}

	/* Reset and re-acquire */
	void reset(T* p) noexcept(false) {
		cout << "shared_ptr :: Reset(" << p; if (p) cout << " = " << *p; cout << ")" << endl;
		release();
		acquire(p);
	}

	/* Operations of reference counter */
	operator bool() const noexcept {
		return spc.use_count() > 0;
	}
	bool unique() const noexcept {
		return spc.use_count() == 1;
	}
	long use_count() const noexcept {
		return spc.use_count();
	}

	/* Operations of underlying pointer */
	T& operator* () const noexcept {
		return *data;
	}
	T* operator-> () const noexcept {
		return data;
	}
	T* get() const noexcept {
		return data;
	}

 private:
 	/* Acquire pointer */
	void acquire(T* p) noexcept(false) {
		cout << "shared_ptr :: Acquire(" << p; if (p) cout << " = " << *p; cout << ")" << endl;
		spc.acquire(p);  // may throw bad_alloc
		data = p;
	}

	/* Release underlying pointer */
	void release() noexcept {
		cout << "shared_ptr :: Release for data = "; if (data) cout << data << " == " << *data; else cout << "nullptr"; cout << endl;
		spc.release(data);
		data = nullptr;
	}
};


template<typename T>
class weak_ptr {
	T* data;
	shared_ptr_count spc;

	friend class shared_ptr<T>;

 public:
 	/* Default ctor */
	weak_ptr()
	: data(nullptr), spc()
	{
		cout << "weak_ptr :: Default ctor" << endl;
	}

	/* Standart ctor */
	template<typename U>
	weak_ptr(const shared_ptr<U>& rhs)
	: data(rhs.data), spc(rhs.spc)
	{
		cout << "weak_ptr :: Standard shared_ptr<U>& ctor" << endl;
	}

	/* Copy ctor */
	weak_ptr(const weak_ptr& rhs)
	: spc(rhs.spc)
	{
		cout << "weak_ptr :: Copy ctor" << endl;
		data = rhs.lock().get();
	}

	/* Destructor */
	~weak_ptr() {
		cout << "~weak_ptr :: Destructor" << endl;
	}


	/* Assign-to-another-weak_ptr operator */
	template<typename U>
	weak_ptr& operator= (const weak_ptr<U>& rhs) noexcept {
		cout << "weak_ptr :: operator= (weak_ptr&)" << endl;
		data = rhs.lock().get();
		spc = rhs.spc;
		return *this;
	}

	/* Assign-to-shared_ptr operator */
	template<typename U>
	weak_ptr& operator= (const shared_ptr<U>& rhs) noexcept {
		cout << "weak_ptr :: operator= (shared_ptr&)" << endl;
		data = rhs.data;
		spc = rhs.spc;
		return *this;
	}

	/* Swap */
	void swap(weak_ptr<T>& rhs) noexcept {
		cout << "weak_ptr :: Swap" << endl;
		std::swap(data, rhs.data);
		spc.swap(rhs.spc);
	}

	/* Reset */
	void reset() noexcept {
		cout << "weak_ptr :: Reset" << endl;
		weak_ptr<T>().swap(*this);
	}

	/* Lock */
	shared_ptr<T> lock() const noexcept {
		cout << "weak_ptr :: Lock" << endl;
		return shared_ptr<T>(*this);
	}

	/* Operations of reference counter */
	bool expired() const noexcept {
		return spc.use_count() == 0;
	}
	long use_count() const noexcept {
		return spc.use_count();
	}

	/* Operations of underlying pointer */
	T& operator* () const noexcept {
		return *data;
	}
	T* operator-> () const noexcept {
		return data;
	}
	T* get() const noexcept {
		return data;
	}
};


int main_wrapped();
int main() {
	try {
		cout << "main_wrapped() ..." << endl;
		int e = main_wrapped();
		if (e == 0) {
			cout << "\nOK" << endl;
		}
		else {
			cout << "\nMain returned errcode = " << e << endl;
		}
	}
	catch (const char* msg) {
		cout << "\nException caught: " << msg << endl;
	}
	catch (...) {
		cout << "\nSome exception caught." << endl;
	}
}


int main_wrapped() {
	using T = TValidator<int>;
	// using T = int;

	cout << ">	Simple shared_ptr on T(5)" << endl;
	T* x1 = new T(5);
	shared_ptr<T> sh1(x1);
	cout << "----------" << endl;

	cout << ">	Empty shared_ptr" << endl;
	shared_ptr<T> sh2;
	cout << "----------" << endl;

	cout << ">	Now stress `sh2 = sh1`" << endl;
	sh2 = sh1;
	cout << "sh1.get() = " << sh1.get() << " = " << *sh1.get() << endl;
	cout << "sh2.get() = " << sh2.get() << " = " << *sh2.get() << endl;
	cout << "sh1.use_count() = " << sh1.use_count() << endl;
	cout << "sh2.use_count() = " << sh2.use_count() << endl;
	cout << "----------" << endl;

	cout << ">	Copy shared_ptr - shared_ptr sh4(sh2)" << endl;
	shared_ptr<T> sh4(sh2);
	cout << "sh2.use_count() = " << sh2.use_count() << endl;
	cout << "sh4.use_count() = " << sh4.use_count() << endl;
	cout << "sh2.get() = " << sh2.get() << " = " << *sh2.get() << endl;
	cout << "sh4.get() = " << sh4.get() << " = " << *sh4.get() << endl;
	cout << "----------" << endl;

	cout << ">	Now reset one shared_ptr - sh4" << endl;
	sh4.reset();
	cout << "sh1.use_count() = " << sh1.use_count() << endl;
	cout << "sh2.use_count() = " << sh2.use_count() << endl;
	cout << "sh4.use_count() = " << sh4.use_count() << endl;
	cout << "sh1.get() = " << sh1.get() << " = " << *sh1.get() << endl;
	cout << "sh2.get() = " << sh2.get() << " = " << *sh2.get() << endl;
	cout << "sh4.get() = " << sh4.get() << endl;
	cout << "----------" << endl;

	cout << ">	Now reset another shared_ptr - sh2" << endl;
	sh2.reset();
	cout << "sh1.use_count() = " << sh1.use_count() << endl;
	cout << "sh2.use_count() = " << sh2.use_count() << endl;
	cout << "sh4.use_count() = " << sh4.use_count() << endl;
	cout << "sh1.get() = " << sh1.get() << " = " << *sh1.get() << endl;
	cout << "sh2.get() = " << sh2.get() << endl;
	cout << "sh4.get() = " << sh4.get() << endl;
	cout << "----------" << endl;

	cout << ">	Now reset last shared_ptr - sh1" << endl;
	sh1.reset();
	cout << "sh1.use_count() = " << sh1.use_count() << endl;
	cout << "sh2.use_count() = " << sh2.use_count() << endl;
	cout << "sh4.use_count() = " << sh4.use_count() << endl;
	cout << "sh1.get() = " << sh1.get() << endl;
	cout << "sh2.get() = " << sh2.get() << endl;
	cout << "sh4.get() = " << sh4.get() << endl;
	cout << "----------" << endl;

	cout << ">	Test scoped shared_ptr" << endl;
	{
	shared_ptr<T> sh3(new T(42));
	cout << "sh3.get() = " << sh3.get() << " = " << *sh3.get() << endl;
	}
	cout << "----------" << endl;


	cout << ">	Now testing weak pointers. Empty weak_ptr" << endl;
	weak_ptr<T> wp1;
	cout << "----------" << endl;

	cout << ">	Create shared_ptr and init weak_ptr with it" << endl;
	shared_ptr<T> sh5(new T(-2000));
	cout << "sh5.use_count() = " << sh5.use_count() << endl;
	cout << "sh5.get() = " << sh5.get() << endl;
	weak_ptr<T> wp2(sh5);
	cout << "sh5.use_count() = " << sh5.use_count() << endl;
	cout << "sh5.get() = " << sh5.get() << " = " << *sh5.get() << endl;
	cout << "wp2.use_count() = " << wp2.use_count() << endl;
	cout << "wp2.get() = " << wp2.get() << " = " << *wp2.get() << endl;
	cout << "----------" << endl;

	cout << ">	Now remove last created shared_ptr" << endl;
	sh5.reset();
	cout << "sh5.use_count() = " << sh5.use_count() << endl;
	cout << "sh5.get() = " << sh5.get() << endl;
	cout << "wp2.use_count() = " << wp2.use_count() << endl;
	cout << "wp2.get() = " << wp2.get() << " = " << *wp2.get() << endl;
	cout << "----------" << endl;

	cout << ">	Init one shared_ptr, weak_ptr for it, and another shared_ptr locked to created weak_ptr" << endl;
	shared_ptr<T> sh6(new T(999));
	weak_ptr<T> wp3(sh6);
	shared_ptr<T> sh7 = wp3.lock();
	cout << "sh6.use_count() = " << sh6.use_count() << endl;
	cout << "wp3.use_count() = " << wp3.use_count() << endl;
	cout << "sh7.use_count() = " << sh7.use_count() << endl;
	cout << "sh6.get() = " << sh6.get() << endl;
	cout << "wp3.get() = " << wp3.get() << endl;
	cout << "sh7.get() = " << sh7.get() << endl;
	cout << "----------" << endl;

	cout << ">	Now remove last shared_ptr(weaken)" << endl;
	sh7.reset();
	cout << "sh6.use_count() = " << sh6.use_count() << endl;
	cout << "wp3.use_count() = " << wp3.use_count() << endl;
	cout << "sh7.use_count() = " << sh7.use_count() << endl;
	cout << "sh6.get() = " << sh6.get() << endl;
	cout << "wp3.get() = " << wp3.get() << endl;
	cout << "sh7.get() = " << sh7.get() << endl;
	cout << "----------" << endl;

	cout << "\nReturning from main_wrapped." << endl;
	return 0;
}
