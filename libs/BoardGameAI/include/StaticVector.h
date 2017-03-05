#pragma once

//Source: https://gist.github.com/Rapptz/8153884

#include <type_traits>
#include <iterator>
#include <initializer_list>
#include <stdexcept>

template<typename T>
using StorageFor = typename std::aligned_storage<sizeof(T), std::alignment_of<T>::value>::type;

template<typename T, size_t N>
struct static_vector {
private:
	StorageFor<T> arr[N];
	size_t len = 0;

	constexpr const T& ref_at(size_t pos) const {
		return *static_cast<const T*>(static_cast<const void*>(&arr[pos]));
	}

	T& ref_at(size_t pos) {
		return *static_cast<T*>(static_cast<void*>(&arr[pos]));
	}

	constexpr const T* ptr_at(size_t pos) const {
		return static_cast<const T*>(static_cast<const void*>(&arr[pos]));
	}

	T* ptr_at(size_t pos) {
		return static_cast<T*>(static_cast<void*>(&arr[pos]));
	}
public:
	using value_type = T;
	using size_type = size_t;
	using difference_type = ptrdiff_t;
	using reference = T&;
	using const_reference = const T&;
	using pointer = T*;
	using const_pointer = const T*;
	using iterator = T*;
	using const_iterator = const T*;
	using reverse_iterator = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

	static_vector() 
	{

	}
	static_vector(std::initializer_list<T> l) {
		if (l.size() > N) {
			throw std::out_of_range("static_vector::static_vector");
		}

		for (auto&& i : l) {
			push_back(i);
		}
	}

	~static_vector() {
		clear();
	}

	// member access
	constexpr const_reference at(size_type pos) const {
		return pos > len ? throw std::out_of_range("static_vector::at") : ref_at(pos);
	}

	reference at(size_type pos) {
		return pos > len ? throw std::out_of_range("static_vector::at") : (*static_cast<T*>(static_cast<void*>(&arr[pos])));
		//return (*static_cast<T*>(static_cast<void*>(&arr[pos])));
	}

	constexpr const_reference operator[](size_type pos) const {
		return ref_at(pos);
	}

	reference operator[](size_type pos) {
		return ref_at(pos);
	}

	constexpr const_reference front() const {
		return ref_at(0);
	}

	reference front() {
		return ref_at(0);
	}

	constexpr const_reference back() const {
		return ref_at(len - 1);
	}

	reference back() {
		return ref_at(len - 1);
	}

	const_pointer data() const {
		return ptr_at(0);
	}

	pointer data() {
		return ptr_at(0);
	}

	// iterators
	iterator begin() {
		return data();
	}

	const_iterator begin() const {
		return data();
	}

	const_iterator cbegin() const {
		return data();
	}

	iterator end() {
		return data() + len;
	}

	const_iterator end() const {
		return data() + len;
	}

	const_iterator cend() const {
		return data() + len;
	}

	reverse_iterator rbegin() {
		return{ data() + len };
	}

	const_reverse_iterator rbegin() const {
		return{ data() + len };
	}

	const_reverse_iterator crbegin() const {
		return{ data() + len };
	}

	reverse_iterator rend() {
		return{ data() };
	}

	const_reverse_iterator rend() const {
		return{ data() };
	}

	const_reverse_iterator crend() const {
		return{ data() };
	}

	// capacity
	constexpr bool empty() const noexcept {
		return len == 0;
	}

	constexpr size_t size() const noexcept {
		return len;
	}

	constexpr size_t max_size() const noexcept {
		return N;
	}

	constexpr size_t capacity() const noexcept {
		return N;
	}

	// modifiers
	void pop_back() {
		static_cast<const T*>(static_cast<const void*>(&arr[len - 1]))->~T();
		--len;
	}

	void clear() {
		while (!empty()) {
			pop_back();
		}
	}

	void push_back(const_reference v) {
		// can't push back anymore
#ifdef _DEBUG
		if (size() == N) {
			throw std::out_of_range("static_vector::push_back");
		}
#endif

		::new(arr + len) T(v);
		++len;
	}

	void push_back(T&& v) {
		// can't push back anymore
#ifdef _DEBUG
		if (size() == N) {
			throw std::out_of_range("static_vector::push_back");
		}
#endif

		::new(arr + len) T(std::move(v));
		++len;
	}

	template<typename... Args>
	void emplace_back(Args&&... args) {
		// reached maximum
#ifdef _DEBUG
		if (size() == N) {
			throw std::out_of_range("static_vector::emplace_back");
		}
#endif

		::new(arr + len) T(std::forward<Args>(args)...);
		++len;
	}
};
