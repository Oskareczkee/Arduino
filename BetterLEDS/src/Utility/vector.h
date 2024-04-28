//custom vector implementation for our project
#pragma once

/// @brief Vector implementation for arduino
///			It lacks some functionalities like Iterator and initializer_list support, as Arduino does not implement cpp standard library
///			Returns default value on error, but this can be changed using change_error_val()
/// @tparam T 
template <class T>
class vector
{
protected:
	const static int VECTOR_INITIAL_SIZE = 1;
	T* arr;
	T error_val=T();
	//capacity is reserved memory in vector, size number of elements in vector
	int _capacity, _size;

	//copies content and resizes vector
	void resize(){
		_capacity *= 2;
		T* temp = new T[_capacity];

		//copy elements from arr to new array
		for (int x = 0; x < _size; x++)
			temp[x] = arr[x];

		//delete arr, temp becomes new arr
		delete[] arr;
		arr = temp;
	}

public:
	vector() : arr(new T[VECTOR_INITIAL_SIZE]), _capacity(VECTOR_INITIAL_SIZE), _size(0) {}
	vector(const vector& other) : arr(new T[other._capacity]), _capacity(other._capacity), _size(other._size)
	{
		for (int x = 0; x < _size; x++)
			arr[x] = other.arr[x];
	}
	
	~vector(){
		delete[] arr;
	}

	void push_back(T data){
		//vector is full
		if (_size == _capacity)
			resize();
		arr[_size] = data;
		_size++;
	}

	void pop_back(){
		_size--;
	}

	void erase(const unsigned int& index){
		if(index<0 || index > _size)return;

		memmove(arr + index + 1, arr+index, sizeof(arr[0])* (_size-index));//override memory in the erased area
		pop_back();//delete last element, othwerwise duplicate will be created
	}

	void clear(){
		_size=0;
	}

	int size() const{
		return _size;
	}

	void shrink_to_fit(){
		if(_size==0)
			_capacity=1;
		else
			_capacity = _size;
	}

	T& operator[](const unsigned int index) const{
		if (index < 0 || index >= _size)
			//TODO: This code is dangerous, consider other solution, this code returns error_val and user can override it easily by accident or not by accident
			return (T&)error_val;

		return arr[index];
	}

	void change_error_val(const T& val){
		error_val = val;
	}

	T* get_array_ptr(){
		return arr;
	}
};