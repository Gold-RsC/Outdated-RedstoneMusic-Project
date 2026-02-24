#ifndef BASICTYPE_H
#define BASICTYPE_H
#include<iostream>
template<typename T>
void swap(T&a,T&b){
	T tmp(a);
	a=b;
	b=tmp;
}
template<typename...T>
class Package;
template<>
class Package<> {};
template<typename Va,typename...T>
class Package<Va,T...>:Package<T...> {
	private:
		Va _value;
	public:
		Package(Va arg,T...args)
			:_value(arg),Package<T...>(args...) {}
		Va&value(void) {
			return _value;
		}
		Package<T...>&next(void) {
			return *this;
		}
};
template<typename Va>
std::ostream&operator<<(std::ostream&out,Package<Va> a) {
	return out<<a.value();
}
template<typename Va,typename...T>
std::ostream&operator<<(std::ostream&out,Package<Va,T...> a) {
	return out<<a.value()<<a.next();
}
#include<vector>
template<typename T>
class Array{
	private:
		T*_data;
		size_t _capacity,_size;
	public:
		static int INITMEMORY;
		Array(void){
			_data=new T[INITMEMORY];
			_capacity=INITMEMORY;
			_size=0;
		}
		Array(size_t t){
			_data=new T[t];
			_capacity=t;
			_size=0;
		}
		Array(const Array&another){
			_data=new T[another._size];
			for(_capacity=another._size;_size<_capacity;_size++){
				_data[_size]=another._data[_size];
			}
		}
		Array(std::initializer_list<T> list){
			_data=new T[list.size()];
			_capacity=list.size();
			for(T a:list){
				_data[_size++]=a;
			}
		}
		~Array(void){
			delete[] _data;
		}
		void resize(size_t t){
			T*newData=new T[t];
			_capacity=_size=_size<t?_size:t;
			for(size_t i=0;i<_size;i++){
				newData[i]=_data[i];
			}
			delete[] _data;
			_data=newData;
		}
		void reserve(void){
			if(_size==_capacity){
				return;
			}
			T*newData=new T[_size];
			_capacity=_size;
			for(size_t i=0;i<_size;i++){
				newData[i]=_data[i];
			}
			delete[] _data;
			_data=newData;
		}
		T&at(int n){
			if(n<_capacity&&n>=0){
				return _data[n];
			}
			throw "out_of_range";
			return (T)NULL;
		}
		void clear(void){
			_size=0;
		}
		void clear(T a){
			for(int i=0;i<_size;i++){
				_data[i]=a;
			}
			_size=0;
		}
		bool empty(void){
			return _size==0;
		}
		size_t size(void){
			return _size;
		}
		size_t capacity(void){
			return _capacity;
		}
		void swap(Array<T>&another){
			swap(*this,another);
		}
		
};
template<typename T>
int Array<T>::INITMEMORY=10;
#endif

