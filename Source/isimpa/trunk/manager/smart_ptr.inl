// Smart Pointer

template <typename T, bool Array>
smart_ptr<T, Array>::smart_ptr() :
	_RefCounter(0),
	_Pointer(0)
{}

template <typename T, bool Array>
smart_ptr<T, Array>::smart_ptr(const smart_ptr<T, Array>& SmartPtr) :
	_RefCounter(SmartPtr._RefCounter),
	_Pointer(SmartPtr._Pointer)
{
	if(_RefCounter != 0)
		++(*_RefCounter);
}

template <typename T, bool Array>
smart_ptr<T, Array>::smart_ptr(T* Pointer) :
	_Pointer(Pointer)
{
	_RefCounter = new int(1);
}

template <typename T, bool Array>
smart_ptr<T, Array>::~smart_ptr()
{
	_Release();
}

template <typename T, bool Array>
smart_ptr<T, Array>& smart_ptr<T, Array>::operator=(const smart_ptr<T, Array>& SmartPtr)
{
	_Release();

	_Pointer = SmartPtr._Pointer;
	_RefCounter = SmartPtr._RefCounter;
	if(_RefCounter != 0)
		++(*_RefCounter);

	return *this;
}

template <typename T, bool Array>
T& smart_ptr<T, Array>::operator[](unsigned i)
{

	return _Pointer[i];
}

template <typename T, bool Array>
smart_ptr<T, Array>& smart_ptr<T, Array>::operator=(T* Pointer)
{
	_Release();

	_Pointer = Pointer;
	_RefCounter = new int(1);

	return *this;
}

template <typename T, bool Array>
bool smart_ptr<T, Array>::operator==(const smart_ptr<T, Array>& SmartPtr) const
{
	return _Pointer == SmartPtr._Pointer;
}

template <typename T, bool Array>
bool smart_ptr<T, Array>::operator!=(const smart_ptr<T, Array>& SmartPtr) const
{
	return _Pointer != SmartPtr._Pointer;
}

template <typename T, bool Array>
T& smart_ptr<T, Array>::operator*()
{
	return *_Pointer;
}

template <typename T, bool Array>
T* smart_ptr<T, Array>::operator->()
{
	return _Pointer;
}
template <typename T, bool Array>
T* smart_ptr<T, Array>::get() const
{
	return _Pointer;
}
template <typename T, bool Array>
const T& smart_ptr<T, Array>::operator*() const
{
	return *_Pointer;
}

template <typename T, bool Array>
const T* smart_ptr<T, Array>::operator->() const
{
	return _Pointer;
}

template <typename T, bool Array>
void smart_ptr<T, Array>::_Release()
{
	if(_RefCounter != 0)
	{
		(*_RefCounter)--;
		if(*_RefCounter <= 0)
		{
			if(Array)
			{
				delete _RefCounter;
				if(_Pointer != 0)
					delete[] _Pointer;
			}
			else
			{
				delete _RefCounter;
				if(_Pointer != 0)
					delete _Pointer;
			}
		}
	}
}
