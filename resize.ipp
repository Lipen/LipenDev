template<typename T>
T* resize(const T* arr, size_t size, size_t new_size) {
	T* new_arr = new T[new_size]();  // () forces empty conturctors?

	size_t size_min = (size < new_size) ? size : new_size;
	for (size_t i = 0; i < size_min; ++i) {
		new_arr[i] = arr[i];
	}

	delete[] arr;
	arr = 0;

	return new_arr;
}
