// Smart Pointer

template <typename T>
smart_ptr<T>::smart_ptr() :
	_RefCounter(0),
	_Pointer(0)
{}

template <typename T>
smart_ptr<T>::smart_ptr(const smart_ptr<T>& SmartPtr) :
	_RefCounter(SmartPtr._RefCounter),
	_Pointer(SmartPtr._Pointer)
{
	if(_RefCounter != 0)
		++(*_RefCounter);
}

template <typename T>
smart_ptr<T>::smart_ptr(T* Pointer) :
	_Pointer(Pointer)
{
	_RefCounter = new int(1);
}

template <typename T>
smart_ptr<T>::~smart_ptr()
{
	_Release();
}

template <typename T>
smart_ptr<T>& smart_ptr<T>::operator=(const smart_ptr<T>& SmartPtr)
{
    smart_ptr<T> cache(*this); //cache this object before deleting the data- Avoid destroying information if the other subsist from this data
	_Release();

	_Pointer = SmartPtr._Pointer;
	_RefCounter = SmartPtr._RefCounter;
	if(_RefCounter != 0)
		++(*_RefCounter);

	return *this;
}

template <typename T>
smart_ptr<T>& smart_ptr<T>::operator=(T* Pointer)
{
    smart_ptr<T> buffer(*this);
	_Release();

	_Pointer = Pointer;
	_RefCounter = new int(1);

	return *this;
}

template <typename T>
bool smart_ptr<T>::operator==(const smart_ptr<T>& SmartPtr) const
{
	return _Pointer == SmartPtr._Pointer;
}

template <typename T>
bool smart_ptr<T>::operator!=(const smart_ptr<T>& SmartPtr) const
{
	return _Pointer != SmartPtr._Pointer;
}

template <typename T>
T& smart_ptr<T>::operator*()
{
	return *_Pointer;
}

template <typename T>
T* smart_ptr<T>::operator->()
{
	return _Pointer;
}
template <typename T>
T* smart_ptr<T>::get() const
{
	return _Pointer;
}
template <typename T>
const T& smart_ptr<T>::operator*() const
{
	return *_Pointer;
}

template <typename T>
const T* smart_ptr<T>::operator->() const
{
	return _Pointer;
}

template <typename T>
void smart_ptr<T>::_Release()
{
	if(_RefCounter != 0)
	{
		(*_RefCounter)--;
		if(*_RefCounter <= 0)
		{
			delete _RefCounter;
			if(_Pointer != 0)
			{
				delete _Pointer;
			}
		}
	}
}
//////////////
// Smart Pointer Array

template <typename T>
smart_ptr_ar<T>::smart_ptr_ar() :
	_RefCounter(0),
	_Pointer(0)
{}

template <typename T>
smart_ptr_ar<T>::smart_ptr_ar(const smart_ptr_ar<T>& SmartPtr) :
	_RefCounter(SmartPtr._RefCounter),
	_Pointer(SmartPtr._Pointer)
{
	if(_RefCounter != 0)
		++(*_RefCounter);
}

template <typename T>
smart_ptr_ar<T>::smart_ptr_ar(T* Pointer) :
	_Pointer(Pointer)
{
	_RefCounter = new int(1);
}

template <typename T>
smart_ptr_ar<T>::~smart_ptr_ar()
{
	_Release();
}

template <typename T>
smart_ptr_ar<T>& smart_ptr_ar<T>::operator=(const smart_ptr_ar<T>& SmartPtr)
{
    smart_ptr_ar<T> buffer(*this); //keep old data, maybe the future data depends on this..
	_Release();

	_Pointer = SmartPtr._Pointer;
	_RefCounter = SmartPtr._RefCounter;
	if(_RefCounter != 0)
		++(*_RefCounter);

	return *this;
}

template <typename T>
T& smart_ptr_ar<T>::operator[](unsigned i)
{

	return _Pointer[i];
}

template <typename T>
smart_ptr_ar<T>& smart_ptr_ar<T>::operator=(T* Pointer)
{
	_Release();

	_Pointer = Pointer;
	_RefCounter = new int(1);

	return *this;
}

template <typename T>
bool smart_ptr_ar<T>::operator==(const smart_ptr_ar<T>& SmartPtr) const
{
	return _Pointer == SmartPtr._Pointer;
}

template <typename T>
bool smart_ptr_ar<T>::operator!=(const smart_ptr_ar<T>& SmartPtr) const
{
	return _Pointer != SmartPtr._Pointer;
}

template <typename T>
T& smart_ptr_ar<T>::operator*()
{
	return *_Pointer;
}

template <typename T>
T* smart_ptr_ar<T>::operator->()
{
	return _Pointer;
}
template <typename T>
T* smart_ptr_ar<T>::get() const
{
	return _Pointer;
}
template <typename T>
const T& smart_ptr_ar<T>::operator*() const
{
	return *_Pointer;
}

template <typename T>
const T* smart_ptr_ar<T>::operator->() const
{
	return _Pointer;
}

template <typename T>
void smart_ptr_ar<T>::_Release()
{
	if(_RefCounter != 0)
	{
		(*_RefCounter)--;
		if(*_RefCounter <= 0)
		{
			delete _RefCounter;
			if(_Pointer != 0)
				delete[] _Pointer;
		}
	}
}

