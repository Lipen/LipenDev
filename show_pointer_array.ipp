template<typename T>
void show(T* arr, size_t size, std::string dbg="Some array", std::string delimiter="\n") {
	using std::cout;
	using std::endl;

	if (!dbg.empty()) cout << ">show::" << dbg << ':' << endl;

	if (delimiter != "\n") cout << '\t';  //

	for (size_t i = 0; i < size; ++i) {
		if (delimiter == "\n") cout << '\t';
		cout << arr[i];
		if (i+1 < size) cout << delimiter;
	}
	cout << endl;
}
