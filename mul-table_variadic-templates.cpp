#include <iostream>
#include <utility>

using std::cout;
using std::cin;
using std::endl;


template<char ...s>
using char_sequence = std::integer_sequence<char, s...>;


template<char ...s>
constexpr auto new_line(char_sequence<s...>) {
	return char_sequence<s..., '\n'>{};
}

template<char ...s>
constexpr auto new_line() {
	return char_sequence<'\n'>{};
}


template<typename T, T ...s, T ...v>
constexpr auto concat_sequence(std::integer_sequence<T, s...>, std::integer_sequence<T, v...>) {
	return std::integer_sequence<T, s..., v...>{};
}


template<char ...s>
constexpr static char const make_char_string[] = {s..., '\0'};

template<char ...s>
constexpr auto const & make_char_string_from_sequence(char_sequence<s...>) {
	return make_char_string<s...>;
}


constexpr char make_digit_char(const size_t digit, const size_t power_of_ten=1, const char zero=' ') {
	return char(digit>=power_of_ten ? digit/power_of_ten+'0' : zero);
}


template<size_t num>
constexpr auto make_chars_from_num() {
	static_assert(num < 1000, "cannot handle large numbers yet :c");
	return char_sequence<' ',
		make_digit_char(num, 100),
		make_digit_char(num%100, 10, num>=100 ? '0' : ' '),
		char(num%10 + '0')
	>{};
}


template<size_t N, size_t ...I>
constexpr auto add(std::index_sequence<I...>) {
	return std::index_sequence<(N+I)...>{};
}

template<size_t factor, size_t ...cols>
constexpr auto multiply(std::index_sequence<cols...>) {
	return std::index_sequence<(factor*cols)...>{};
}


// forward decl
template<size_t ...elts>
struct convert_to_charseq_impl;

template<size_t n, size_t ...rest>
constexpr auto convert_to_charseq() {
	return concat_sequence(make_chars_from_num<n>(),
						   convert_to_charseq_impl<rest...>{}());
}


template<size_t ...rows>
constexpr auto makerow_charseq(std::index_sequence<rows...>) {
	return new_line(convert_to_charseq<rows...>());
}


template<size_t row, size_t num>
constexpr auto makerow() {
	constexpr auto indeces =
		multiply<row>( add<1>(std::make_index_sequence<num>{}) );
	return makerow_charseq(indeces);
}


template<size_t ...elts>
struct convert_to_charseq_impl {
	constexpr auto operator() () const {
		return convert_to_charseq<elts...>();
	}
};

template<>
struct convert_to_charseq_impl<> {
	constexpr auto operator() () const {
		return char_sequence<>{};
	}
};

// forward decl
template<size_t n, size_t ...rows>
struct make_rows_as_charseq_impl;

template<size_t n, size_t row, size_t ...rest>
constexpr auto make_rows_as_charseq() {
	return concat_sequence(
		makerow<row, n>(),
		make_rows_as_charseq_impl<n, rest...>{}());
}

template<size_t n, size_t ...rows>
struct make_rows_as_charseq_impl {
	constexpr auto operator() () const {
		return make_rows_as_charseq<n, rows...>();
	}
};

template<size_t n>
struct make_rows_as_charseq_impl<n> {
	constexpr auto operator() () const {
		return char_sequence<>{};
	}
};


template<size_t n, size_t ...rows>
constexpr auto make_rows(std::index_sequence<rows...>) {
	return make_rows_as_charseq<n, rows...>();
}


template<size_t dim>
constexpr auto multiplication_table =
	make_char_string_from_sequence(
		concat_sequence(
			new_line(),
			make_rows<dim>(
				add<1>(
					std::make_index_sequence<dim>{}))));


int main() {
	cout << multiplication_table<18>;
}
