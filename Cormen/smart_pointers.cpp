/* Copyright (c) 2016, Lipen */
#include <iostream>  // cout, endl
#include <ostream>   // ostream
#include <utility>   // swap
// TAGS: smart pointers, shared_ptr, weak_ptr, unique_ptr, shared pointer, weak pointer, unique pointer

#include "../TValidator.ipp"

using std::cout;
using std::endl;

// Forward declaration
template<typename T>
class weak_ptr;


class shared_ptr_count {
	long* ref_count;
	// nullptr means non-acquired/released,
	// amount of references to object otherwise

 public:
	/* Default ctor */
	shared_ptr_count() noexcept
	: ref_count(nullptr)
	{
		cout << "shared_ptr_count :: Default ctor" << endl;
	}

	/* Copy ctor */
	shared_ptr_count(const shared_ptr_count &rhs) noexcept
	: ref_count(rhs.ref_count)
	{
		cout << "shared_ptr_count :: Copy ctor("; if (ref_count) cout << ref_count << " = " << *ref_count; else cout << "nullptr"; cout << ")" << endl;
	}


	/* Swap method for copy-and-swap idiom */
	void swap(shared_ptr_count &rhs) noexcept {
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
				ref_count = new long(1);  // TODO: check for std::bad_alloc
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
	shared_ptr(void) noexcept
	: data(nullptr)
	, spc()
	{
		cout << "shared_ptr :: Default ctor" << endl;
	}

	/* Standard ctor */
	explicit shared_ptr(T* p) noexcept(false)
	: spc()
	{
		cout << "shared_ptr :: Standard T* ctor" << endl;
		acquire(p);  // may throw bad_alloc
	}

	/* Ctor for weak pointer lock */
	template<typename U>
	explicit shared_ptr(const weak_ptr<U> &rhs) /* noexcept(false) */
	: spc(rhs.spc)
	{
		// TODO: throw bad_weak_ptr if (rhs.expired() == true)
		cout << "shared_ptr :: Ctor for weak pointer lock" << endl;
		acquire(rhs.data);  // may throw bad_alloc???  Nobody knows...
	}

	/* Copy ctor for copy-and-swap idiom */
	shared_ptr(const shared_ptr &rhs) noexcept
	: spc(rhs.spc)
	{
		cout << "shared_ptr :: Copy ctor" << endl;
		acquire(rhs.data);  // will never throw bad_alloc
	}

	/* Destructor */
	~shared_ptr() {
		cout << "~shared_ptr :: Destructor" << endl;
		release();
	}


	/* Assignment operator using copy-and-swap idiom */
	shared_ptr& operator= (shared_ptr rhs) noexcept {
		cout << "shared_ptr :: operator=" << endl;
		swap(rhs);
		return *this;
	}

	/* Swap method for copy-and-swap idiom */
	void swap(shared_ptr &rhs) noexcept {
		cout << "shared_ptr :: Swap" << endl;
		std::swap(data, rhs.data);
		spc.swap(rhs.spc);
	}

	/* Reset */
	void reset() noexcept {
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
	void acquire(T* p) noexcept(false) {  // may throw bad_alloc
		cout << "shared_ptr :: Acquire(p = "; if (p) cout << p << " = " << *p; else cout << "nullptr"; cout << ")" << endl;
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
	weak_ptr() noexcept
	: data(nullptr), spc()
	{
		cout << "weak_ptr :: Default ctor" << endl;
	}

	/* Standard ctor */
	template<typename U>
	weak_ptr(const shared_ptr<U> &rhs) noexcept
	: data(rhs.data), spc(rhs.spc)
	{
		cout << "weak_ptr :: Standard shared_ptr<U>& ctor" << endl;
	}

	/* Copy ctor */
	weak_ptr(const weak_ptr &rhs) noexcept
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
	weak_ptr& operator= (const weak_ptr<U> &rhs) noexcept {
		cout << "weak_ptr :: operator= (weak_ptr&)" << endl;
		data = rhs.lock().get();
		spc = rhs.spc;
		return *this;
	}

	/* Assign-to-shared_ptr operator */
	template<typename U>
	weak_ptr& operator= (const shared_ptr<U> &rhs) noexcept {
		cout << "weak_ptr :: operator= (shared_ptr&)" << endl;
		data = rhs.data;
		spc = rhs.spc;
		return *this;
	}

	/* Swap */
	void swap(weak_ptr<T> &rhs) noexcept {
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
		return expired() ? shared_ptr<T>() : shared_ptr<T>(*this);
	}

	/* Operations of reference counter */
	bool expired() const noexcept {
		return spc.use_count() == 0;
	}
	long use_count() const noexcept {
		return spc.use_count();
	}

	/* Operations of underlying pointer aren`t allowed */
	// Only for debug!
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


template<typename T>
class unique_ptr {
	T* data;

 public:
	/* Default ctor */
	unique_ptr() noexcept
	: data(nullptr)
	{
		cout << "unique_ptr :: Default ctor" << endl;
	}

	/* Standard ctor */
	explicit unique_ptr(T* p) noexcept
	: data(p)
	{
		cout << "unique_ptr :: Standard T* ctor" << endl;
	}

	/* Copy ctor for copy-and-swap idiom */
	unique_ptr(const unique_ptr &rhs) noexcept
	: data(rhs.data)
	{
		cout << "unique_ptr :: Copy ctor (data = " << data << " = "; if (data) cout << *data; else cout << "nullptr"; cout << ")" << endl;
		const_cast<unique_ptr&>(rhs).data = nullptr;  // Transfer ownership
	}

	/* Destructor */
	~unique_ptr() {
		cout << "~unique_ptr :: Destructor" << endl;
		destroy();
	}


	/* Assignment operator using the copy-and-swap idiom */
	unique_ptr& operator= (unique_ptr rhs) noexcept {
		cout << "unique_ptr :: operator=" << endl;
		swap(rhs);
		return *this;
	}

	/* Swap method for copy-and-swap idiom */
	void swap(unique_ptr &rhs) noexcept {
		cout << "unique_ptr :: Swap" << endl;
		std::swap(data, rhs.data);
	}

	/* Reset */
	void reset() noexcept {
		cout << "unique_ptr :: Reset(void)" << endl;
		destroy();
	}

	/* Reset and re-acquire */
	void reset(T* p) noexcept {
		cout << "unique_ptr :: Reset(" << p << " = "; if (p) cout << *p; else cout << "nullptr"; cout << ")" << endl;
		destroy();
		data = p;
	}

	/* Release pointer without destroying */
	T* release() noexcept {
		cout << "unique_ptr :: Release" << endl;
		T* data_old = data;
		data = nullptr;
		return data_old;
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
	operator bool() const noexcept {
		return data != nullptr;
	}

 private:
	/* Release and destroy underlying pointer */
	void destroy() noexcept {
		cout << "unique_ptr :: Destroy" << endl;
		delete data;
		data = nullptr;
	}
};


template<typename T>
std::ostream& operator<< (std::ostream &o, const shared_ptr<T> &rhs) {
	o << "[get() = " << rhs.get() << " = ";
	if (rhs)
		o << *rhs;
	else
		o << "nullptr";
	return o << "; use_count() == " << rhs.use_count() << "]";
}

template<typename T>
std::ostream& operator<< (std::ostream &o, const weak_ptr<T> &rhs) {
	o << "[get() = " << rhs.get() << " = ";
	if (rhs.get() != nullptr) {
		if (!rhs.expired())
			o << *rhs;
		else
			o << "expired";
	}
	else {
		o << "nullptr";
	}
	return o << "; use_count() == " << rhs.use_count() << "]";
}

template<typename T>
std::ostream& operator<< (std::ostream &o, const unique_ptr<T> &rhs) {
	o << "[get() = " << rhs.get() << " = ";
	if (rhs)
		o << *rhs;
	else
		o << "nullptr";
	return o << "]";
}


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

	cout << ">	Assign `sh2 = sh1`" << endl;
	sh2 = sh1;
	cout << "sh1: " << sh1 << endl;
	cout << "sh2: " << sh2 << endl;
	cout << "----------" << endl;

	cout << ">	Copy shared_ptr `shared_ptr sh4(sh2)`" << endl;
	shared_ptr<T> sh4(sh2);
	cout << "sh2: " << sh2 << endl;
	cout << "sh4: " << sh4 << endl;
	cout << "----------" << endl;

	cout << ">	Now reset one shared_ptr - sh4" << endl;
	sh4.reset();
	cout << "sh1: " << sh1 << endl;
	cout << "sh2: " << sh2 << endl;
	cout << "sh4: " << sh4 << endl;
	cout << "----------" << endl;

	cout << ">	Now reset another shared_ptr - sh2" << endl;
	sh2.reset();
	cout << "sh1: " << sh1 << endl;
	cout << "sh2: " << sh2 << endl;
	cout << "sh4: " << sh4 << endl;
	cout << "----------" << endl;

	cout << ">	Now reset last shared_ptr - sh1" << endl;
	sh1.reset();
	cout << "sh1: " << sh1 << endl;
	cout << "sh2: " << sh2 << endl;
	cout << "sh4: " << sh4 << endl;
	cout << "----------" << endl;

	cout << ">	Test scoped shared_ptr" << endl;
	{
	shared_ptr<T> sh3(new T(42));
	cout << "sh3: " << sh3 << endl;
	}
	cout << "----------" << endl;

	cout << ">	Copy and assign to empty shared_ptr" << endl;
	shared_ptr<T> sh8, sh10;
	cout << "sh8: " << sh8 << endl;
	cout << "sh10: " << sh10 << endl;
	cout << ">Two empty shared_ptr created. Copying..." << endl;
	shared_ptr<T> sh9(sh8);
	cout << "sh9: " << sh9 << endl;
	cout << ">Empty shared_ptr copied. Assigning..." << endl;
	sh8 = sh10;
	cout << "sh8: " << sh8 << endl;
	cout << ">Empty shared_ptr assigned" << endl;
	cout << "----------" << endl;


	cout << "\n>	Now testing weak pointers. Empty weak_ptr" << endl;
	weak_ptr<T> wp1;
	cout << "wp1: " << wp1 << endl;
	cout << "----------" << endl;

	cout << ">	Create shared_ptr and init weak_ptr with it" << endl;
	shared_ptr<T> sh5(new T(-2000));
	cout << "sh5: " << sh5 << endl;
	weak_ptr<T> wp2(sh5);
	cout << "sh5: " << sh5 << endl;
	cout << "wp2: " << wp2 << endl;
	cout << "----------" << endl;

	cout << ">	Now remove last created shared_ptr" << endl;
	sh5.reset();
	cout << "sh5: " << sh5 << endl;
	cout << "wp2: " << wp2 << endl;
	cout << "----------" << endl;

	cout << ">	Init one shared_ptr, weak_ptr for it, and another shared_ptr locked to created weak_ptr" << endl;
	shared_ptr<T> sh6(new T(999));
	weak_ptr<T> wp3(sh6);
	shared_ptr<T> sh7 = wp3.lock();
	cout << "sh6: " << sh6 << endl;
	cout << "wp3: " << wp3 << endl;
	cout << "sh7: " << sh7 << endl;
	cout << "----------" << endl;

	cout << ">	Now remove last shared_ptr(weaken)" << endl;
	sh7.reset();
	cout << "sh6: " << sh6 << endl;
	cout << "wp3: " << wp3 << endl;
	cout << "sh7: " << sh7 << endl;
	cout << "----------" << endl;

	cout << ">	Lock empty weak_ptr" << endl;
	weak_ptr<T> wp4;
	shared_ptr<T> sh11 = wp4.lock();
	cout << "sh11: " << sh11 << endl;
	cout << "----------" << endl;

	cout << ">	Lock expired weak_ptr" << endl;
	shared_ptr<T> sh12(new T(-66));
	cout << "sh12: " << sh12 << endl;
	weak_ptr<T> wp5(sh12);
	cout << "wp5: " << wp5 << endl;
	sh12.reset();
	cout << "sh12: " << sh12 << endl;
	cout << "wp5: " << wp5 << endl;
	cout << ">Now locking..." << endl;
	shared_ptr<T> sh13 = wp5.lock();
	cout << "sh12: " << sh12 << endl;
	cout << "sh13: " << sh13 << endl;
	cout << "----------" << endl;


	cout << "\n>	Now testing unique pointers. Empty unique_ptr" << endl;
	unique_ptr<T> up1;
	cout << "----------" << endl;

	cout << ">	Re-acquire some new data" << endl;
	up1.reset(new T(4444));
	cout << "up1: " << up1 << endl;
	cout << "----------" << endl;

	cout << ">	Copy unique_ptr - unique_ptr up2(up1)" << endl;
	unique_ptr<T> up2(up1);
	cout << "up1: " << up1 << endl;
	cout << "up2: " << up2 << endl;
	cout << "----------" << endl;

	cout << ">	Test scoped unique_ptr" << endl;
	{
	unique_ptr<T> up3(new T(-12345));
	cout << "up3: " << up3 << endl;
	}
	cout << "----------" << endl;

	cout << ">	Destroy unique_ptr `up1.reset()`" << endl;
	up1.reset();
	cout << "up1: " << up1 << endl;
	cout << "----------" << endl;

	cout << ">	Assign unique_ptr `up1 = up2`" << endl;
	up1 = up2;
	cout << "up1: " << up1 << endl;
	cout << "up2: " << up2 << endl;
	cout << "----------" << endl;

	cout << ">	Test scoped reset const unique_ptr (via const_cast)" << endl;
	{
	const unique_ptr<T> up4(new T(11111));
	const_cast<unique_ptr<T>&>(up4).reset();
	}
	cout << "----------" << endl;

	cout << ">	Release const unique_ptr (via const_cast)" << endl;
	const unique_ptr<T> up5(new T(22222));
	const_cast<unique_ptr<T>&>(up5).release();
	cout << "up5: " << up5 << endl;
	cout << "----------" << endl;

	cout << ">	Test scoped const unique_ptr" << endl;
	{
	const unique_ptr<T> up6(new T(33333));
	}
	cout << "----------" << endl;

	cout << ">	Release unique_ptr `T* p = up1.release()`" << endl;
	T* p = up1.release();
	cout << "up1: " << up1 << endl;
	cout << "p = "; if (p) cout << *p; else cout << "nullptr"; cout << endl;
	cout << "----------" << endl;


	cout << "\nReturning from main_wrapped." << endl;
	return 0;
}
