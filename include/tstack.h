#pragma once

#include <iostream>

// template TVector on dynamic memory
template<typename T>
class TVector
{
protected:
	size_t sz;
	size_t capacity;
	T* pMem;
public:
	TVector(size_t size = 0) : sz(size), capacity(sz * 2 + 2)
	{
		pMem = new T[capacity];
		if (pMem == nullptr)
			throw std::bad_alloc();
	}
	TVector(T* arr, size_t s) : sz(s), capacity(sz * 2 + 2)
	{
		if (arr == nullptr)
			throw std::logic_error("TVector ctor requires non - nullptr arg");
		pMem = new T[capacity];
		if (pMem == nullptr)
			throw std::bad_alloc();
		std::copy(arr, arr + sz, pMem);
	}
	TVector(const TVector& v) : sz(v.sz), capacity(v.capacity)
	{
		pMem = new T[capacity];
		if (pMem == nullptr)
			throw std::bad_alloc();
		std::copy(v.pMem, v.pMem + sz, pMem);
	}
	TVector(TVector&& v) noexcept
	{
		sz = 0;
		capacity = 0;
		pMem = nullptr;
		swap(*this, v);
	}
	~TVector()
	{
		delete[] pMem;
		pMem = nullptr;
	}
	TVector& operator=(const TVector& v)
	{
		if (this != &v)
		{
			if (sz != v.sz)
			{
				sz = v.sz;
				delete[] pMem;
				T* p = new T[sz];
				if (p == nullptr)
					throw std::bad_alloc();
				pMem = p;
				p = nullptr;
			}
			std::copy(v.pMem, v.pMem + sz, pMem);
		}
		return (*this);
	}
	TVector& operator=(TVector&& v) noexcept
	{
		sz = 0;
		capacity = 0;
		delete[] pMem;
		pMem = nullptr;
		swap(*this, v);
		return (*this);
	}

	size_t size() const noexcept { return sz; }
	size_t getCapacity() const noexcept{ return capacity; }
	bool isFull() const noexcept { return (sz == capacity); }
	bool isEmpty() const noexcept { return (sz == 0); }

	// indexation
	T& operator[] (size_t ind)
	{
		return pMem[ind];
	}
	const T& operator[] (size_t ind) const
	{
		return pMem[ind];
	}

	// comparation
	bool operator==(const TVector& v) const noexcept
	{
		if (sz == v.sz)
		{
			for (size_t i = 0; i < sz; i++)
				if (pMem[i] != v.pMem[i])
					return 0;
			return 1;
		}
		else return 0;
	}
	bool operator!=(const TVector& v) const noexcept
	{
		return !(*this == v);
	}

	friend void swap(TVector& lhs, TVector& rhs) noexcept
	{
		std::swap(lhs.sz, rhs.sz);
		std::swap(lhs.capacity, rhs.capacity);
		std::swap(lhs.pMem, rhs.pMem);
	}

	// methods
	void resize(size_t newsize)
	{
		T* p = new T[newsize * 2 + 2];
		if (p == nullptr)
			throw std::bad_alloc();
		std::copy(pMem, pMem + sz, p);
		delete[] pMem;
		pMem = p;
		p = nullptr;
		capacity = newsize * 2 + 2;
	}
	void push_back(T elem)
	{
		if (isFull())
			resize(sz + 1);
		pMem[sz++] = elem;
	}
	void pop_back()
	{
		if (sz != 0)
			sz--;
		else throw std::logic_error("Vector is empty");
	}
	void push_front(T elem)
	{
		insert(elem, 0);
	}
	void pop_front()
	{
		erase(0);
	}
	void insert(T elem, size_t ind)
	{
		if (ind < 0 || ind >= capacity)
			throw std::out_of_range("Index should be greater than zero and less than vector size");
		if (sz == capacity)
			resize(sz + 1);
		for (size_t i = sz; i > ind; i--)
			pMem[i] = pMem[i-1];
		pMem[ind] = elem;
		sz++;
	}
	void erase(size_t ind)
	{
		if (ind < 0 || ind >= capacity)
			throw std::out_of_range("Index should be greater than zero and less than vector size");
		if (sz == 0)
			throw std::logic_error("Vector is empty");
		for (size_t i = ind; i < sz; i++)
			pMem[i] = pMem[i + 1];
		sz--;
	}
};

template<typename T>
class Stack : private TVector<T>
{
	using TVector<T>::sz;
	using TVector<T>::capacity;
	using TVector<T>::pMem;
	using TVector<T>::push_back;
	using TVector<T>::pop_back;
	using TVector<T>::resize;
public:
	using TVector<T>::isFull;
	using TVector<T>::isEmpty;
	using TVector<T>::size;
	using TVector<T>::getCapacity;
	Stack() : TVector<T>(0) { };
	void push(T elem)
	{
		push_back(elem);
	}
	void pop()
	{
		pop_back();
	}
	T top()
	{
		if (isEmpty())
			throw std::logic_error("Stack is empty");
		return pMem[sz - 1];
	}
};

template<typename T>
class Queue : private TVector<T>
{
	using TVector<T>::sz;
	using TVector<T>::capacity;
	using TVector<T>::pMem;
	using TVector<T>::push_back;
	using TVector<T>::resize;
	size_t front;
	size_t back;
public:
	using TVector<T>::isFull;
	using TVector<T>::isEmpty;
	using TVector<T>::size;
	using TVector<T>::getCapacity;
	Queue() : TVector<T>(0), front(0), back(0) { }
	void push(T elem)
	{
		push_back(elem);
			if (back == capacity)
				back = 0;
			else
				back++;
	}
	void pop()
	{
		if (isEmpty())
			throw std::logic_error("Queue is empty");
		if (front == capacity)
			front = 0;
		else
			front++;
		sz--;
	}
	T getFront() const noexcept { return pMem[front]; }
	T getBack() const noexcept { if (back == 0) return pMem[capacity - 1]; return pMem[back - 1]; }
	size_t getBackIndex() const noexcept { return back; }
};

template<typename T>
class QueueOnStacks : private TVector<Stack<T>>
{
	using TVector<Stack<T>>::pMem;
	size_t sz;
	size_t capacity;
public:
	QueueOnStacks() : TVector<Stack<T>>(0), sz(pMem[0].size() + pMem[1].size()), capacity(pMem[0].getCapacity() + pMem[1].getCapacity()) { }
	size_t size() const noexcept { return sz; }
	size_t getCapacity() const noexcept { return capacity; }
	bool isFull() const noexcept { return (pMem[0].isFull() && pMem[1].isFull()); }
	bool isEmpty() const noexcept { return (pMem[0].isEmpty() && pMem[1].isEmpty()); }
	void push(T elem)
	{
		pMem[1].push(elem);
		sz++;
	}
	void pop()
	{
		if (pMem[0].isEmpty())
		{
			while (!pMem[1].isEmpty())
			{
				T tmp = pMem[1].top();
				pMem[0].push(tmp);
				pMem[1].pop();
			}
			pMem[0].pop();
		}
		else
			pMem[0].pop();
		sz--;
	}
	T front()
	{
		if (!pMem[1].isEmpty()) 
		{
			T tmp{ };
			while (!pMem[1].isEmpty())
			{
				tmp = pMem[1].top();
				pMem[1].pop();
			}
			return tmp;
		}
		if (!pMem[0].isEmpty())
			return pMem[0].top();
		return 0;
	}
	T back()
	{
		if (!pMem[0].isEmpty())
		{
			T tmp{ };
			while (!pMem[0].isEmpty()) 
			{
				tmp = pMem[0].top();
				pMem[0].pop();
			}
			return tmp;
		}
		if (!pMem[1].isEmpty())
			return pMem[1].top();
		return 0;
	}
};