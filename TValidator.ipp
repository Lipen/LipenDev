#include <iostream>
#include <initializer_list>
#include <utility>

using std::cout;
using std::endl;


template<typename T>
class TValidator final {
	T value_;

 public:
	TValidator() : value_(T()) {
		cout << "Empty constructor call" << endl;
	}

	explicit TValidator(T value) : value_(value) {
		cout << "Constructor call (" << value << ")" << endl;
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

	T get_value() const {
		return value_;
	}

	T& get_reference() {
		return value_;
	}


	friend std::ostream& operator<< (std::ostream& o, const TValidator& rhs) {
		o << "[TValidator: " << rhs.value_ << "]";
		return o;
	}
};
