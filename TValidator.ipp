#include <iostream>
#include <initializer_list>
#include <utility>
#include <cstdarg>

using std::cout;
using std::endl;


template<typename T>
class TValidator final {
	T value_;

	void print_args(auto &t) {  // calls last
		cout << t << ")" << endl;
	}
	template<typename... Args>
	void print_args(auto &t, Args&&... args) {  // calls recursive
		cout << t << ", ";
		print_args(args...);
	}

 public:
	TValidator() : value_(T()) {
		cout << "Empty constructor call" << endl;
	}

	explicit TValidator(T value) : value_(value) {
		cout << "Constructor call (" << value << ")" << endl;
	}

	template<typename... Args>
	TValidator( Args&&... args ) : value_( T(std::forward<Args>(args)...) ) {
		cout << "Emplace constructor call (";
		print_args(args...);
	}

	TValidator(std::initializer_list<T> l) : value_(*l.begin()) {
		cout << "Initializer list (of size " << l.size() << ") constructor call (" << *l.begin() << ")" << endl;
	}

	~TValidator() {
		cout << "Destructor call (" << value_ << ")" << endl;
	}

	TValidator(const TValidator& other) : value_(other.value_) {
		cout << "Copy constructor call (" << other.value_ << ")" << endl;
		// call std::copy on pointer-arrays, if neccessary
	}

	TValidator(TValidator&& other) : value_(std::move(other.value_)) {
		cout << "Move constructor call (" << other.value_ << ")" << endl;
	}


	TValidator& operator= (const TValidator& rhs) {
		if (this == &rhs) {
			cout << "Self-assignment call! (" << rhs.value_ << ")" << endl;
			return *this;
		}

		cout << "Assignment (operator=) call (" << rhs.value_ << ")" << endl;
		value_ = rhs.value_;
		return *this;
	}

	TValidator& operator= (TValidator&& rhs) {
		cout << "Move assignment (operator=) call (" << rhs.value_ << ")" << endl;
		value_ = std::move(rhs.value_);
		return *this;
	}


	// Implicit setter ;Q
	TValidator& operator= (T value) {
		cout << "Implicit setter (operator=) call (" << value << ")" << endl;
		value_ = value;
		return *this;
	}


	// Implicit cast
	operator T () {
		cout << "Implicitly casting to " << value_ << endl;
		return value_;
	}


	// Some getters
	T get_value() const {
		return value_;
	}

	T* operator-> () {
		cout << "Operator-> call (" << value_ << ")" << endl;
		return &value_;
	}
	T* operator-> () const {
		cout << "Operator-> const call (" << value_ << ")" << endl;
		return &value_;
	}

	T& operator* () {
		cout << "Operator* call (" << value_ << ")" << endl;
		return value_;
	}
	T& operator* () const {
		cout << "Operator* const call (" << value_ << ")" << endl;
		return value_;
	}


	// // TODO: Access operator (only for arrays (template<typename T*>))
	// T& operator[](size_t i) {
	// 	return value_[i];
	// }
	// const T& operator[](size_t i) const {
	// 	return value_[i];
	// }


	// Printable interface
	friend std::ostream& operator<< (std::ostream& o, const TValidator& rhs) {
		o << "[TValidator: " << rhs.value_ << "]";
		return o;
	}
};
