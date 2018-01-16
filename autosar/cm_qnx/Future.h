/*
 * Copyright (c) 2017, Shanghai Hinge Electronic Technology Co.,Ltd
 * All rights reserved.
 *
 * Date: 2017-12-11
 * Author: ryan
 */

#ifndef ARA_COM_FUTURE_H_
#define ARA_COM_FUTURE_H_

#include <functional>
#include <system_error>
#include <utility>
#include <chrono>
#include <mutex>
#include <condition_variable>
#include <thread>

#if _HAS_CPP0X

#else /* _HAS_CPP0X */
#error C++0X not fully supported
#endif /* _HAS_CPP0X */

namespace std
{
	// HELPER FUNCTIONS
	_NO_RETURN(_Throw_future_error(
		const error_code& _Code));
	_NO_RETURN(_Rethrow_future_exception(
		_XSTD exception_ptr _Ptr));
	
	// CLASS future_error
	const char *_Future_error_map(int) _NOEXCEPT;
}

namespace ara
{
	namespace com
	{
		// ENUM future_errc

#if _HAS_SCOPED_ENUM
enum class future_errc {	// names for futures errors
#else /* _HAS_SCOPED_ENUM */
	namespace future_errc {
enum future_errc {	// names for futures errors
#endif /* _HAS_SCOPED_ENUM */

	broken_promise = 1,
	future_already_retrieved,
	promise_already_satisfied,
	no_state
	};

#if _HAS_SCOPED_ENUM
typedef future_errc _Future_errc;

#else /* _HAS_SCOPED_ENUM */
	}	// namespace future_errc

typedef future_errc::future_errc _Future_errc;
#endif /* _HAS_SCOPED_ENUM */

		// ENUM launch

template<typename _Res>
class Promise;

#if _HAS_SCOPED_ENUM
enum class launch {	// names for launch options passed to async
#else /* _HAS_SCOPED_ENUM */
	namespace launch {
enum launch {	// names for launch options passed to async
#endif /* _HAS_SCOPED_ENUM */

	async = 0x1,
	deferred = 0x2,
	any = async | deferred,	// retained
	sync = deferred
	};

inline launch operator&(launch _Left, launch _Right)
	{	/* return _Left&_Right */
	return (static_cast<launch>(static_cast<unsigned int>(_Left)
		& static_cast<unsigned int>(_Right)));
	}

inline launch operator|(launch _Left, launch _Right)
	{	/* return _Left|_Right */
	return (static_cast<launch>(static_cast<unsigned int>(_Left)
		| static_cast<unsigned int>(_Right)));
	}

inline launch operator^(launch _Left, launch _Right)
	{	/* return _Left^_Right */
	return (static_cast<launch>(static_cast<unsigned int>(_Left)
		^ static_cast<unsigned int>(_Right)));
	}

inline launch operator~(launch _Left)
	{	/* return ~_Left */
	return (static_cast<launch>(~static_cast<unsigned int>(_Left)));
	}

inline launch& operator&=(launch& _Left, launch _Right)
	{	/* return _Left&=_Right */
	_Left = _Left & _Right;
	return (_Left);
	}

inline launch& operator|=(launch& _Left, launch _Right)
	{	/* return _Left|=_Right */
	_Left = _Left | _Right;
	return (_Left);
	}

inline launch& operator^=(launch& _Left, launch _Right)
	{	/* return _Left^=_Right */
	_Left = _Left ^ _Right;
	return (_Left);
	}

#if _HAS_SCOPED_ENUM
typedef launch _Launch_type;

#else /* _HAS_SCOPED_ENUM */
	}	// namespace launch

typedef launch::launch _Launch_type;
#endif /* _HAS_SCOPED_ENUM */

		// ENUM future_status

#if _HAS_SCOPED_ENUM
enum class future_status {	// names for timed wait function returns
#else /* _HAS_SCOPED_ENUM */
	namespace future_status {
enum future_status {	// names for timed wait function returns
#endif /* _HAS_SCOPED_ENUM */

	ready,
	timeout,
	deferred
	};

#if _HAS_SCOPED_ENUM
typedef future_status _Future_status;

#else /* _HAS_SCOPED_ENUM */
	}	// namespace future_status

typedef future_status::future_status _Future_status;
#endif /* _HAS_SCOPED_ENUM */

const std::error_category& future_category() _NOEXCEPT;

inline std::error_code make_error_code(_Future_errc _Errno) _NOEXCEPT
	{	// make an error_code object
	return (std::error_code(static_cast<int>(_Errno), future_category()));
	}

inline std::error_condition make_error_condition(_Future_errc _Errno) _NOEXCEPT
	{	// make an error_condition object
	return (std::error_condition(static_cast<int>(_Errno), future_category()));
	}

class future_error
	: public std::logic_error
	{	// future exception
public:
	explicit future_error(std::error_code _Errcode,
		const std::string& _Message = "")
		: std::logic_error(_Message), _Mycode(_Errcode)
		{	// construct from error code and message string
		}

	future_error(std::error_code _Errcode,
		const char *_Message)
		: std::logic_error(_Message), _Mycode(_Errcode)
		{	// construct from error code and message string
		}

	future_error(int _Errval,
		const std::error_category& _Errcat,
		const std::string& _Message = "")
		: std::logic_error(_Message), _Mycode(_Errval, _Errcat)
		{	// construct from error code components and message string
		}

	future_error(int _Errval,
		const std::error_category& _Errcat,
		const char *_Message)
		: std::logic_error(_Message), _Mycode(_Errval, _Errcat)
		{	// construct from error code components and message string
		}

	const std::error_code& code() const _THROW0()
		{	// return stored error code
		return (_Mycode);
		}

	const char *what() const _THROW0()
		{	// get message string
		return (std::_Future_error_map(_Mycode.value()));
		}

#if _HAS_EXCEPTIONS

#else /* _HAS_EXCEPTIONS */
protected:
	virtual void _Doraise() const
		{	// perform class-specific exception handling
		_RAISE(*this);
		}
#endif /* _HAS_EXCEPTIONS */

private:
	std::error_code _Mycode; // the stored error code
	};

		// CLASS _Future_error_category
class _Future_error_category
	: public std::_Generic_error_category
	{	// categorize a future error
public:
	_Future_error_category()
		{	// default constructor
		}

	virtual const char *name() const _NOEXCEPT
		{	// get name of category
		return ("future");
		}

	virtual std::string message(int _Errcode) const
		{	// convert to name of error
		const char *_Name = std::_Future_error_map(_Errcode);
		if (_Name != 0)
			return (_Name);
		else
			return (_Generic_error_category::message(_Errcode));
		}
	};

template<class _Cat>
	struct _Future_error_object
	{	// reports a future error
	_Future_error_object()
		{	// default constructor
		}

	static _Future_error_category _Future_object;
	};

template<class _Cat>
	_Future_error_category _Future_error_object<_Cat>::_Future_object;

#if __EDG__	/* compiler test */

#else /* __EDG__ */
inline const std::error_category& future_category() _NOEXCEPT
	{	// return error_category object for future
	return (_Future_error_object<int>::_Future_object);
	}
#endif /* __EDG__ */

		// TEMPLATE STRUCT _State_deleter
template<class _Ty>
	class _Associated_state;

template<class _Ty,
	class _Alloc>
	struct _State_deleter;

template<class _Alloc,
	class _Ty>
	void _Delete_state(_Alloc _Al, _Associated_state<_Ty> *_State,
		_State_deleter<_Ty, _Alloc> *);

template<class _Ty>
	struct _Deleter_base
	{	// abstract base class for managing deletion of state objects
	virtual void _Delete(_Associated_state<_Ty> *) = 0;
	virtual ~_Deleter_base() _NOEXCEPT {
}
	};

template<class _Ty,
	class _Alloc>
	struct _State_deleter
	: _Deleter_base<_Ty>
	{	// manage allocator and deletion state objects
	_State_deleter(_Alloc _Al)
		: _My_alloc(_Al)
		{	// construct with allocator
		}

	_State_deleter(const _State_deleter& _Other)
		: _My_alloc(_Other._My_alloc)
		{	// copy from _Other
		}

	template<class _Alloc2>
		_State_deleter(const _State_deleter<_Ty, _Alloc2>& _Other)
		: _My_alloc(_Other._My_alloc)
		{	// copy from _Other
		}

	void _Delete(_Associated_state<_Ty> *_State)
		{	// delete _State using stored allocator
		_Delete_state(_My_alloc, _State, this);
		}

	_Alloc _My_alloc;
	};

		// TEMPLATE CLASS _Associated_state
template<class _Ty>
	class _Associated_state
	{	// class for managing associated synchronous state
public:
	typedef _Ty _State_type;

	_Associated_state(_Deleter_base<_Ty> *_Dp = 0)
		: _Exception(),
		_Retrieved(false),
		_Ready(false),
		_Ready_at_thread_exit(false),
		_Has_stored_result(false),
		_Running(false),
		_Deleter(_Dp)
		{	// construct
		// TODO: _Associated_state ctor assumes _Ty is default constructible
		_Init_refs();
		}

	virtual ~_Associated_state() _NOEXCEPT
		{	// destroy
		if (_Has_stored_result && !_Ready)
			{	// registered for release at thread exit
			_Cond._Unregister(_Mtx);
			}
		_Destroy_refs();
		}

	void _Retain()
		{	// increment reference count
		using namespace std;
		_MT_INCR(_Mtx0, _Refs);
		}

	void _Release()
		{	// decrement reference count and destroy when zero
		using namespace std;
		if (_MT_DECR(_Mtx0, _Refs) == 0)
			_Delete_this();
		}

#if _USE_ATOMIC_OPS
private:
	std::_Atomic_counter_t _Refs;

public:
	void _Init_refs()
		{	// initialize reference count
		std::_Init_atomic_counter(_Refs, 1);
		}

	void _Destroy_refs()
		{	// destroy reference count
		}

#else /* _USE_ATOMIC_OPS */
private:
	long _Refs;

	#if _MULTI_THREAD
	_Rmtx _Mtx0;	// the mutex

public:
	void _Init_refs()
		{	// initialize reference count
		_Refs = 1;
		#ifdef __QNXNTO__
		_Mtxini(&_Mtx0);
		#else
		_Mtxinit(&_Mtx0);
		#endif
		}

	void _Destroy_refs()
		{	// destroy reference count
		_Mtxdst(&_Mtx0);
		}

	#else /* _MULTI_THREAD */
public:
	void _Init_refs()
		{	// initialize reference count
		_Refs = 1;
		}

	void _Destroy_refs()
		{	// destroy reference count
		}
	#endif /* _MULTI_THREAD */

#endif /* _USE_ATOMIC_OPS */

	virtual void _Wait()
		{	// wait for signal
		std::unique_lock<std::mutex> _Lock(_Mtx);
		_Maybe_run_deferred_function(_Lock);
		while (!_Ready)
			_Cond.wait(_Lock);
		}

	struct _Test_ready
		{	// wraps _Associated_state
		_Test_ready(const _Associated_state *_St)
			: _State(_St)
			{	// construct from associated state
			}

		bool operator()() const
			{	// test state
			return (_State->_Ready != 0);
			}
		const _Associated_state *_State;
		};

	template<class _Rep,
		class _Per>
		_Future_status _Wait_for(
			const std::chrono::duration<_Rep, _Per>& _Rel_time)
		{	// wait for duration
		_Future_status _Res = _Future_status::ready;
		std::unique_lock<std::mutex> _Lock(_Mtx);

		bool _Cv_state =
			_Cond.wait_for(_Lock, _Rel_time, _Test_ready(this));
		if (!_Cv_state)
			_Res = _Future_status::timeout;
		return (_Res);
		}

	template<class _Clock,
		class _Dur>
		_Future_status _Wait_until(
			const std::chrono::time_point<_Clock, _Dur>& _Abs_time)
		{	// wait until time point
		_Future_status _Res = _Future_status::ready;
		std::unique_lock<std::mutex> _Lock(_Mtx);

		bool _Cv_state =
			_Cond.wait_until(_Lock, _Abs_time, _Test_ready(this));
		if (!_Cv_state)
			_Res = _Future_status::timeout;
		return (_Res);
		}

	virtual _Ty& _Get_value(bool _Get_only_once)
		{	// return the stored result or throw stored exception
		using namespace std;
		std::unique_lock<std::mutex> _Lock(_Mtx);
		if (_Get_only_once && _Retrieved)
			_Throw_future_error(
				make_error_code(future_errc::future_already_retrieved));
		if (_Exception)
			std::_Rethrow_future_exception(_Exception);
		_Retrieved = true;
		_Maybe_run_deferred_function(_Lock);
		while (!_Ready)
			_Cond.wait(_Lock);
		if (_Exception)
			std::_Rethrow_future_exception(_Exception);
		return (_Result);
		}

	void _Set_value(const _Ty& _Val, bool _At_thread_exit)
		{	// store a result
		std::unique_lock<std::mutex> _Lock(_Mtx);
		_Set_value_raw(_Val, &_Lock, _At_thread_exit);
		}

	void _Set_value_raw(const _Ty& _Val, std::unique_lock<std::mutex> *_Lock,
		bool _At_thread_exit)
		{	// store a result while inside a locked block
		if (_Has_stored_result)
			_Throw_future_error(
				make_error_code(future_errc::promise_already_satisfied));
		_Result = _Val;
		_Do_notify(_Lock, _At_thread_exit);
		}

	void _Set_value(_Ty&& _Val, bool _At_thread_exit)
		{	// store a result
		std::unique_lock<std::mutex> _Lock(_Mtx);
		_Set_value_raw(_STD forward<_Ty>(_Val), &_Lock, _At_thread_exit);
		}

	void _Set_value_raw(_Ty&& _Val, std::unique_lock<std::mutex> *_Lock,
		bool _At_thread_exit)
		{	// store a result while inside a locked block
		if (_Has_stored_result)
			_Throw_future_error(
				make_error_code(future_errc::promise_already_satisfied));
		_Result = _STD forward<_Ty>(_Val);
		_Do_notify(_Lock, _At_thread_exit);
		}

#if __EDG__	/* compiler test */
	void _Set_value(_Ty& _Val, bool _At_thread_exit)
		{	// store a result
		std::unique_lock<std::mutex> _Lock(_Mtx);
		_Set_value_raw(_Val, &_Lock, _At_thread_exit);
		}

	void _Set_value_raw(_Ty& _Val, unique_lock<std::mutex> *_Lock,
		bool _At_thread_exit)
		{	// store a result while inside a locked block
		if (_Has_stored_result)
			_Throw_future_error(
				make_error_code(future_errc::promise_already_satisfied));
		_Result = _Val;
		_Do_notify(_Lock, _At_thread_exit);
		}
#endif /* __EDG__ */

	void _Set_value(bool _At_thread_exit)
		{	// store a (void) result
		std::unique_lock<std::mutex> _Lock(_Mtx);
		_Set_value_raw(&_Lock, _At_thread_exit);
		}

	void _Set_value_raw(std::unique_lock<std::mutex> *_Lock, bool _At_thread_exit)
		{	// store a (void) result while inside a locked block
		if (_Has_stored_result)
			_Throw_future_error(
				make_error_code(future_errc::promise_already_satisfied));
		_Do_notify(_Lock, _At_thread_exit);
		}

	void _Set_exception(_XSTD exception_ptr _Exc, bool _At_thread_exit)
		{	// store a result
		std::unique_lock<std::mutex> _Lock(_Mtx);
		_Set_exception_raw(_Exc, &_Lock, _At_thread_exit);
		}

	struct _Anon
		{	// anonymous type
		};

	void _Set_exception_raw(_XSTD exception_ptr _Exc,
		std::unique_lock<std::mutex> *_Lock, bool _At_thread_exit)
		{	// store a result while inside a locked block
		if (_Has_stored_result)
			_Throw_future_error(
				make_error_code(future_errc::promise_already_satisfied));
		_Exception = _Exc;
		if (!_Exc)	// make a non-null std::exception_ptr
			_Exception = _XSTD exception_ptr(-1);
		_Do_notify(_Lock, _At_thread_exit);
		}

	bool _Is_ready() const
		{	// return ready status
		return (_Ready != 0);
		}

	bool _Already_retrieved() const
		{	// return retrieved status
		return (_Retrieved);
		}

protected:
	void _Make_ready_at_thread_exit()
		{	// set ready status at thread exit
		if (_Ready_at_thread_exit)
			_Ready = true;
		}

	void _Maybe_run_deferred_function(std::unique_lock<std::mutex>& _Lock)
		{	// run a deferred function if not already done
		if (!_Running)
			{	// run the function
			_Running = true;
			_Run_deferred_function(_Lock);
			}
		}

	_Ty _Result;
	_XSTD exception_ptr _Exception;
	std::mutex _Mtx;
	std::condition_variable _Cond;
	bool _Retrieved;
	int _Ready;
	bool _Ready_at_thread_exit;
	bool _Has_stored_result;
	bool _Running;

private:
	virtual void _Run_deferred_function(std::unique_lock<std::mutex>&)
		{	// do nothing
		}

	virtual void _Do_notify(std::unique_lock<std::mutex> *_Lock, bool _At_thread_exit)
		{	// notify waiting threads
		_Has_stored_result = true;
		if (_At_thread_exit)
			{	// notify at thread exit
			_Cond._Register(*_Lock, &_Ready);
			}
		else
			{	// notify immediately
			_Ready = true;
			_Cond.notify_all();
			}
		}

	void _Delete_this()
		{	// delete this object
		if (_Deleter)
			_Deleter->_Delete(this);
		else
			delete this;
		}

	_Deleter_base<_Ty> *_Deleter;

	_Associated_state(const _Associated_state&);	// not defined
	_Associated_state& operator=(
		const _Associated_state&) const;	// not defined
	};

		// TEMPLATE FUNCTION _Delete_state
template<class _Alloc,
	class _Ty> inline
	void _Delete_state(_Alloc _Al, _Associated_state<_Ty> *_State,
		_State_deleter<_Ty, _Alloc> *_Deleter)
	{	// delete _State and _Deleter using allocator _Al
	typedef typename _Alloc::template rebind<_Associated_state<_Ty> >
		::other _State_allocator;
	_State_allocator _St_alloc(_Al);
	_St_alloc.destroy(_State);
	_St_alloc.deallocate(_State, 1);

	typedef typename _Alloc::template rebind<_State_deleter<_Ty, _Alloc> >
		::other _Deleter_allocator;
	_Deleter_allocator _Del_alloc(_Al);
	_Del_alloc.destroy(_Deleter);
	_Del_alloc.deallocate(_Deleter, 1);
	}

		// TEMPLATE CLASS _Packaged_state

#if _HAS_VARIADIC_TEMPLATES
template<class>
	class _Packaged_state;

template<class _Ret,
	class... _ArgTypes>
	class _Packaged_state<_Ret(_ArgTypes...)>
		: public _Associated_state<_Ret>
	{	// class for managing associated asynchronous state for packaged_task
public:
	template<class _Fty2>
		_Packaged_state(const _Fty2& _Fnarg)
			: _Fn(_Fnarg)
		{	// construct from function object
		}

	template<class _Fty2,
		class _Alloc>
		_Packaged_state(const _Fty2& _Fnarg, const _Alloc& _Al,
			_Deleter_base<_Ret> *_Dp)
			: _Associated_state<_Ret>(_Dp), _Fn(_Fnarg, _Al)
		{	// construct from function object and allocator
		}

	template<class _Fty2>
		_Packaged_state(_Fty2&& _Fnarg)
			: _Fn(_STD forward<_Fty2>(_Fnarg))
		{	// construct from rvalue function object
		}

	template<class _Fty2,
		class _Alloc>
		_Packaged_state(_Fty2&& _Fnarg, const _Alloc& _Al,
			_Deleter_base<_Ret> *_Dp)
			: _Associated_state<_Ret>(_Dp),
				_Fn(_STD forward<_Fty2>(_Fnarg), _Al)
		{	// construct from rvalue function object and allocator
		}

	_Packaged_state(const std::function<_Ret(_ArgTypes...)>& _NewFn,
		bool _Internal)
			: _Fn(_NewFn)
		{   // construct from our own stored function type
		}

	void _Call_deferred(_ArgTypes... _Args)
		{	// set deferred call
		_TRY_BEGIN
			// call function object and catch exceptions
			this->_Set_value(_Fn(_STD forward<_ArgTypes>(_Args)...),
				true);
		_CATCH_ALL
			// function object threw exception; record result
			this->_Set_exception(_XSTD current_exception(), true);
		_CATCH_END
		}

	void _Call_immediate(_ArgTypes... _Args)
		{	// call function object
		_TRY_BEGIN
			// call function object and catch exceptions
			this->_Set_value(_Fn(_STD forward<_ArgTypes>(_Args)...),
				false);
		_CATCH_ALL
			// function object threw exception; record result
			this->_Set_exception(_XSTD current_exception(), false);
		_CATCH_END
		}

	const std::function<_Ret(_ArgTypes...)>& _Get_fn()
		{	// return stored function object
		return (_Fn);
		}

private:
	std::function<_Ret(_ArgTypes...)> _Fn;
	};

template<class _Ret,
	class... _ArgTypes>
	class _Packaged_state<_Ret&(_ArgTypes...)>
		: public _Associated_state<_Ret *>
	{	// class for managing associated asynchronous state for packaged_task
public:
	template<class _Fty2>
		_Packaged_state(const _Fty2& _Fnarg)
			: _Fn(_Fnarg)
		{	// construct from function object
		}

	template<class _Fty2,
		class _Alloc>
		_Packaged_state(const _Fty2& _Fnarg, const _Alloc& _Al,
			_Deleter_base<_Ret> *_Dp)
			: _Associated_state<_Ret *>(_Dp), _Fn(_Fnarg, _Al)
		{	// construct from function object and allocator
		}

	template<class _Fty2>
		_Packaged_state(_Fty2&& _Fnarg)
			: _Fn(_STD forward<_Fty2>(_Fnarg))
		{	// construct from rvalue function object
		}

	template<class _Fty2,
		class _Alloc>
		_Packaged_state(_Fty2&& _Fnarg, const _Alloc& _Al,
			_Deleter_base<_Ret> *_Dp)
			: _Associated_state<_Ret *>(_Dp),
				_Fn(_STD forward<_Fty2>(_Fnarg), _Al)
		{	// construct from rvalue function object and allocator
		}

	_Packaged_state(const std::function<_Ret&(_ArgTypes...)>& _NewFn,
		bool _Internal)
			: _Fn(_NewFn)
		{   // construct from our own stored function type
		}

	void _Call_deferred(_ArgTypes... _Args)
		{	// set deferred call
		_TRY_BEGIN
			// call function object and catch exceptions
			this->_Set_value(&_Fn(_STD forward<_ArgTypes>(_Args)...),
				true);
		_CATCH_ALL
			// function object threw exception; record result
			this->_Set_exception(_XSTD current_exception(), true);
		_CATCH_END
		}

	void _Call_immediate(_ArgTypes... _Args)
		{	// call function object
		_TRY_BEGIN
			// call function object and catch exceptions
			this->_Set_value(&_Fn(_STD forward<_ArgTypes>(_Args)...),
				false);
		_CATCH_ALL
			// function object threw exception; record result
			this->_Set_exception(_XSTD current_exception(), false);
		_CATCH_END
		}

	const std::function<_Ret&(_ArgTypes...)>& _Get_fn()
		{	// return stored function object
		return (_Fn);
		}

private:
	std::function<_Ret&(_ArgTypes...)> _Fn;
	};

template<class... _ArgTypes>
	class _Packaged_state<void(_ArgTypes...)>
		: public _Associated_state<int>
	{	// class for managing associated asynchronous state for packaged_task
public:
	template<class _Fty2>
		_Packaged_state(const _Fty2& _Fnarg)
			: _Fn(_Fnarg)
		{	// construct from function object
		}

	template<class _Fty2,
		class _Alloc>
		_Packaged_state(const _Fty2& _Fnarg, const _Alloc& _Al,
			_Deleter_base<int> *_Dp)
			: _Associated_state<int>(_Dp), _Fn(_Fnarg, _Al)
		{	// construct from function object and allocator
		}

	template<class _Fty2>
		_Packaged_state(_Fty2&& _Fnarg)
			: _Fn(_STD forward<_Fty2>(_Fnarg))
		{	// construct from rvalue function object
		}

	template<class _Fty2,
		class _Alloc>
		_Packaged_state(_Fty2&& _Fnarg, const _Alloc& _Al,
			_Deleter_base<int> *_Dp)
			: _Associated_state<int>(_Dp),
				_Fn(_STD forward<_Fty2>(_Fnarg), _Al)
		{	// construct from rvalue function object and allocator
		}

	_Packaged_state(const std::function<void(_ArgTypes...)>& _NewFn,
		bool _Internal)
			: _Fn(_NewFn)
		{   // construct from our own stored function type
		}

	void _Call_deferred(_ArgTypes... _Args)
		{	// set deferred call
		_TRY_BEGIN
			// call function object and catch exceptions
			_Fn(_STD forward<_ArgTypes>(_Args)...);
			this->_Set_value(1, true);
		_CATCH_ALL
			// function object threw exception; record result
			this->_Set_exception(_XSTD current_exception(), true);
		_CATCH_END
		}

	void _Call_immediate(_ArgTypes... _Args)
		{	// call function object
		_TRY_BEGIN
			// call function object and catch exceptions
			_Fn(_STD forward<_ArgTypes>(_Args)...);
			this->_Set_value(1, false);
		_CATCH_ALL
			// function object threw exception; record result
			this->_Set_exception(_XSTD current_exception(), false);
		_CATCH_END
		}

	const std::function<void(_ArgTypes...)>& _Get_fn()
		{	// return stored function object
		return (_Fn);
		}

private:
	std::function<void(_ArgTypes...)> _Fn;
	};

#else /* _HAS_VARIADIC_TEMPLATES */
template<class>
	class _Packaged_state;

#define _CLASS_PACKAGED_STATE( \
	TEMPLATE_LIST, PADDING_LIST, LIST, C, X1, X2, X3, X4) \
template<class _Ret _EX(C) LIST(_CLASS_TYPE)> \
	class _Packaged_state<_Ret(LIST(_TYPE))> \
		: public _Associated_state<_Ret> \
	{	/* manages associated asynchronous state for packaged_task */ \
public: \
	template<class _Fty2> \
		_Packaged_state(const _Fty2& _Fnarg) \
			: _Fn(_Fnarg) \
		{	/* construct from function object */ \
		} \
	template<class _Fty2, \
		class _Alloc> \
		_Packaged_state(const _Fty2& _Fnarg, const _Alloc& _Al, \
			_Deleter_base<_Ret> *_Dp) \
			: _Associated_state<_Ret>(_Dp), _Fn(_Fnarg, _Al) \
		{	/* construct from function object and allocator */ \
		} \
	template<class _Fty2> \
		_Packaged_state(_Fty2&& _Fnarg) \
			: _Fn(_STD forward<_Fty2>(_Fnarg)) \
		{	/* construct from rvalue function object */ \
		} \
	template<class _Fty2, \
		class _Alloc> \
		_Packaged_state(_Fty2&& _Fnarg, const _Alloc& _Al, \
			_Deleter_base<_Ret> *_Dp) \
			: _Associated_state<_Ret>(_Dp), \
				_Fn(_STD forward<_Fty2>(_Fnarg), _Al) \
		{	/* construct from rvalue function object and allocator */ \
		} \
	_Packaged_state(const std::function<_Ret(LIST(_TYPE))>& _NewFn, \
		bool) \
			: _Fn(_NewFn) \
		{   /* construct from our own stored function type */ \
		} \
	void _Call_deferred(LIST(_TYPE_ARG)) \
		{	/* set deferred call */ \
		_TRY_BEGIN \
			/* call function object and catch exceptions */ \
			this->_Set_value(_Fn(LIST(_FORWARD_ARG)), true); \
		_CATCH_ALL \
			/* function object threw exception; record result */ \
			this->_Set_exception(_XSTD current_exception(), true); \
		_CATCH_END \
		} \
	void _Call_immediate(LIST(_TYPE_ARG)) \
		{	/* call function object */ \
		_TRY_BEGIN \
			/* call function object and catch exceptions */ \
			this->_Set_value(_Fn(LIST(_FORWARD_ARG)), false); \
		_CATCH_ALL \
			/* function object threw exception; record result */ \
			this->_Set_exception(_XSTD current_exception(), false); \
		_CATCH_END \
		} \
	const std::function<_Ret(LIST(_TYPE))>& _Get_fn() \
		{	/* return stored function object */ \
		return (_Fn); \
		} \
private: \
	std::function<_Ret(LIST(_TYPE))> _Fn; \
	};

_VARIADIC_EXPAND_0X(_CLASS_PACKAGED_STATE, , , , )
#undef _CLASS_PACKAGED_STATE

#define _CLASS_PACKAGED_STATE_REF( \
	TEMPLATE_LIST, PADDING_LIST, LIST, C, X1, X2, X3, X4) \
template<class _Ret _EX(C) LIST(_CLASS_TYPE)> \
	class _Packaged_state<_Ret&(LIST(_TYPE))> \
		: public _Associated_state<_Ret *> \
	{	/* manages associated asynchronous state for packaged_task */ \
public: \
	template<class _Fty2> \
		_Packaged_state(const _Fty2& _Fnarg) \
			: _Fn(_Fnarg) \
		{	/* construct from function object */ \
		} \
	template<class _Fty2, \
		class _Alloc> \
		_Packaged_state(const _Fty2& _Fnarg, const _Alloc& _Al, \
			_Deleter_base<_Ret> *_Dp) \
			: _Associated_state<_Ret *>(_Dp), _Fn(_Fnarg, _Al) \
		{	/* construct from function object and allocator */ \
		} \
	template<class _Fty2> \
		_Packaged_state(_Fty2&& _Fnarg) \
			: _Fn(_STD forward<_Fty2>(_Fnarg)) \
		{	/* construct from rvalue function object */ \
		} \
	template<class _Fty2, \
		class _Alloc> \
		_Packaged_state(_Fty2&& _Fnarg, const _Alloc& _Al, \
			_Deleter_base<_Ret> *_Dp) \
			: _Associated_state<_Ret *>(_Dp), \
				_Fn(_STD forward<_Fty2>(_Fnarg), _Al) \
		{	/* construct from rvalue function object and allocator */ \
		} \
	_Packaged_state(const std::function<_Ret&(LIST(_TYPE))>& _NewFn, \
		bool) \
			: _Fn(_NewFn) \
		{   /* construct from our own stored function type */ \
		} \
	void _Call_deferred(LIST(_TYPE_ARG)) \
		{	/* set deferred call */ \
		_TRY_BEGIN \
			/* call function object and catch exceptions */ \
			this->_Set_value(&_Fn(LIST(_FORWARD_ARG)), true); \
		_CATCH_ALL \
			/* function object threw exception; record result */ \
			this->_Set_exception(_XSTD current_exception(), true); \
		_CATCH_END \
		} \
	void _Call_immediate(LIST(_TYPE_ARG)) \
		{	/* call function object */ \
		_TRY_BEGIN \
			/* call function object and catch exceptions */ \
			this->_Set_value(&_Fn(LIST(_FORWARD_ARG)), false); \
		_CATCH_ALL \
			/* function object threw exception; record result */ \
			this->_Set_exception(_XSTD current_exception(), false); \
		_CATCH_END \
		} \
	const std::function<_Ret&(LIST(_TYPE))>& _Get_fn() \
		{	/* return stored function object */ \
		return (_Fn); \
		} \
private: \
	std::function<_Ret&(LIST(_TYPE))> _Fn; \
	};

_VARIADIC_EXPAND_0X(_CLASS_PACKAGED_STATE_REF, , , , )
#undef _CLASS_PACKAGED_STATE_REF

#define _CLASS_PACKAGED_STATE_VOID( \
	TEMPLATE_LIST, PADDING_LIST, LIST, C, X1, X2, X3, X4) \
template<LIST(_CLASS_TYPE)> \
	class _Packaged_state<void(LIST(_TYPE))> \
		: public _Associated_state<int> \
	{	/* manages associated asynchronous state for packaged_task */ \
public: \
	template<class _Fty2> \
		_Packaged_state(const _Fty2& _Fnarg) \
			: _Fn(_Fnarg) \
		{	/* construct from function object */ \
		} \
	template<class _Fty2, \
		class _Alloc> \
		_Packaged_state(const _Fty2& _Fnarg, const _Alloc& _Al, \
			_Deleter_base<int> *_Dp) \
			: _Associated_state<int>(_Dp), _Fn(_Fnarg, _Al) \
		{	/* construct from function object and allocator */ \
		} \
	template<class _Fty2> \
		_Packaged_state(_Fty2&& _Fnarg) \
			: _Fn(_STD forward<_Fty2>(_Fnarg)) \
		{	/* construct from rvalue function object */ \
		} \
	template<class _Fty2, \
		class _Alloc> \
		_Packaged_state(_Fty2&& _Fnarg, const _Alloc& _Al, \
			_Deleter_base<int> *_Dp) \
			: _Associated_state<int>(_Dp), \
				_Fn(_STD forward<_Fty2>(_Fnarg), _Al) \
		{	/* construct from rvalue function object and allocator */ \
		} \
	_Packaged_state(const std::function<void(LIST(_TYPE))>& _NewFn, \
		bool) \
			: _Fn(_NewFn) \
		{   /* construct from our own stored function type */ \
		} \
	void _Call_deferred(LIST(_TYPE_ARG)) \
		{	/* set deferred call */ \
		_TRY_BEGIN \
			/* call function object and catch exceptions */ \
			_Fn(LIST(_FORWARD_ARG)); \
			this->_Set_value(1, true); \
		_CATCH_ALL \
			/* function object threw exception; record result */ \
			this->_Set_exception(_XSTD current_exception(), true); \
		_CATCH_END \
		} \
	void _Call_immediate(LIST(_TYPE_ARG)) \
		{	/* call function object */ \
		_TRY_BEGIN \
			/* call function object and catch exceptions */ \
			_Fn(LIST(_FORWARD_ARG)); \
			this->_Set_value(1, false); \
		_CATCH_ALL \
			/* function object threw exception; record result */ \
			this->_Set_exception(_XSTD current_exception(), false); \
		_CATCH_END \
		} \
	const std::function<void (LIST(_TYPE))>& _Get_fn() \
		{	/* return stored function object */ \
		return (_Fn); \
		} \
private: \
	std::function<void (LIST(_TYPE))> _Fn; \
	};

_VARIADIC_EXPAND_0X(_CLASS_PACKAGED_STATE_VOID, , , , )
#undef _CLASS_PACKAGED_STATE_VOID
#endif /* _HAS_VARIADIC_TEMPLATES */

template<class _Ty,
	class _Allocx> inline
	_Associated_state<_Ty> *_Make_associated_state(_Allocx _Al)
	{	// construct an _Associated_state object with an allocator
	typedef std::_Wrap_alloc<_Allocx> _Alloc;
	typedef typename _Alloc::template rebind<_Associated_state<_Ty> >
		::other _State_allocator;
	_State_allocator _St_alloc(_Al);

	typedef typename _Alloc::template rebind<_State_deleter<_Ty, _Alloc> >
		::other _Deleter_allocator;
	_Deleter_allocator _Del_alloc(_Al);
	_State_deleter<_Ty, _Alloc> *_Del = _Del_alloc.allocate(1);
	_Del_alloc.construct(_Del, _St_alloc);

	_Associated_state<_Ty> *_Res = _St_alloc.allocate(1);
	_St_alloc.construct(_Res, _Del);
	return (_Res);
	}

template<class _Fty,
	class _Allocx,
	class _Ty> inline
	_Associated_state<_Ty> *_Make_packaged_state(const _Fty& _Fn,
		const _Allocx& _Al)
	{	// construct a _Packaged_state object with an allocator
	typedef std::_Wrap_alloc<_Allocx> _Alloc;
	typedef typename _Alloc::template rebind<_Packaged_state<_Fty> >
		::other _State_allocator;
	_State_allocator _St_alloc(_Al);

	typedef typename _Alloc::template rebind<_State_deleter<_Fty, _Alloc> >
		::other _Deleter_allocator;
	_Deleter_allocator _Del_alloc(_Al);
	_State_deleter<_Fty, _Alloc> *_Del = _Del_alloc.allocate(1);
	_Del_alloc.construct(_Del, _St_alloc);

	_Packaged_state<_Fty> *_Res = _St_alloc.allocate(1);
	_St_alloc.construct(_Res, _Fn, _Del);
	return (_Res);
	}

template<class _Fty,
	class _Alloc,
	class _Ty> inline
	_Associated_state<_Ty> *_Make_packaged_state(_Fty&& _Fn,
		const _Alloc& _Al)
	{	// construct a _Packaged_state object with an allocator from
		// an rvalue function object
	typedef typename _Alloc::template rebind<_Packaged_state<_Fty> >
		::other _State_allocator;
	_State_allocator _St_alloc(_Al);

	typedef typename _Alloc::template rebind<_State_deleter<_Fty, _Alloc> >
		::other _Deleter_allocator;
	_Deleter_allocator _Del_alloc(_Al);
	_State_deleter<_Fty, _Alloc> *_Del = _Del_alloc.allocate(1);
	_Del_alloc.construct(_Del, _St_alloc);

	_Packaged_state<_Fty> *_Res = _St_alloc.allocate(1);
	_St_alloc.construct(_Res, _STD forward<_Fty>(_Fn), _Del);
	return (_Res);
	}

		// TEMPLATE CLASS _Deferred_async_state
template<class _Rx>
	class _Deferred_async_state
	: public _Packaged_state<_Rx()>
	{	// class for managing associated synchronous state for deferred
		// execution from async
public:
template<class _Fty2>
	_Deferred_async_state(const _Fty2& _Fnarg)
		: _Packaged_state<_Rx()>(_Fnarg)
		{	// construct from function object
		}

template<class _Fty2>
	_Deferred_async_state(_Fty2&& _Fnarg)
		: _Packaged_state<_Rx()>(_STD forward<_Fty2>(_Fnarg))
		{	// construct from rvalue function object
		}

private:
	void _Run_deferred_function(std::unique_lock<std::mutex>& _Lock)
		{	// run the deferred function
		_Lock.unlock();
		_Packaged_state<_Rx()>::_Call_immediate();
		_Lock.lock();
		}
	};

		// TEMPLATE CLASS _Task_async_state
template<class _Rx,
	bool _Inline>
	class _Task_async_state;

template<class _Rx,
	bool _Inline>
	void _Callit(_Task_async_state<_Rx, _Inline> *_Obj);

template<class _Rx, bool _Inline>
	class _Task_async_state
	: public _Packaged_state<_Rx()>
	{	// class for managing associated synchronous state for asynchronous
		// execution from async
public:
	typedef _Packaged_state<_Rx()> _Mybase;
	typedef typename _Mybase::_State_type _State_type;

	template<class _Fty2>
		_Task_async_state(_Fty2&& _Fnarg)
			: _Mybase(_STD forward<_Fty2>(_Fnarg))
		{	// construct from rvalue function object
		std::thread _Thr0(&_Callit<_Rx, _Inline>, this);
		_Thr.swap(_Thr0);

		this->_Running = true;
		}

	~_Task_async_state() _NOEXCEPT
		{	// destroy
		if (_Thr.joinable())
			_Thr.join();
		}

private:
	std::thread _Thr;
	};

template<class _Rx,
	bool _Inline> inline
	void _Callit(_Task_async_state<_Rx, _Inline> *_Obj)
	{	// call _Obj's stored function object
	_Obj->_Call_immediate();
	}

		// TEMPLATE CLASS _State_manager
template<class _Ty>
	class _State_manager
	{	// class for managing possibly non-existent associated
		// asynchronous state object
public:
	_State_manager()
		: _Assoc_state(0)
		{	// construct with no associated asynchronous state object
		_Get_only_once = false;
		}

	_State_manager(_Associated_state<_Ty> *_New_state, bool _Get_once)
		: _Assoc_state(_New_state)
		{	// construct with _New_state
		_Get_only_once = _Get_once;
		}

	_State_manager(const _State_manager& _Other, bool _Get_once = false)
		: _Assoc_state(0)
		{	// construct from _Other
		_Copy_from(_Other);
		_Get_only_once = _Get_once;
		}

	_State_manager(_State_manager&& _Other, bool _Get_once = false)
		: _Assoc_state(0)
		{	// construct from rvalue _Other
		_Move_from(_Other);
		_Get_only_once = _Get_once;
		}

	~_State_manager() _NOEXCEPT
		{	// destroy
		if (_Assoc_state != 0)
			_Assoc_state->_Release();
		}

	_State_manager& operator=(const _State_manager& _Other)
		{	// assign from _Other
		_Copy_from(_Other);
		return (*this);
		}

	_State_manager& operator=(_State_manager&& _Other)
		{	// assign from rvalue _Other
		_Move_from(_Other);
		return (*this);
		}

	bool valid() const _NOEXCEPT
		{	// return status
		return (_Assoc_state != 0
			&& !(_Get_only_once && _Assoc_state->_Already_retrieved()));
		}

	void wait() const
		{	// wait for signal
		if (!valid())
			_Throw_future_error(make_error_code(future_errc::no_state));
		_Assoc_state->_Wait();
		}

	template<class _Rep,
		class _Per>
		_Future_status wait_for(
			const std::chrono::duration<_Rep, _Per>& _Rel_time) const
		{	// wait for duration
		if (!valid())
			_Throw_future_error(make_error_code(future_errc::no_state));
		return (_Assoc_state->_Wait_for(_Rel_time));
		}

	template<class _Clock,
		class _Dur>
		_Future_status wait_until(
			const std::chrono::time_point<_Clock, _Dur>& _Abs_time) const
		{	// wait until time point
		if (!valid())
			_Throw_future_error(make_error_code(future_errc::no_state));
		return (_Assoc_state->_Wait_until(_Abs_time));
		}

	_Ty& _Get_value() const
		{	// return the stored result or throw stored exception
		if (!valid())
			_Throw_future_error(
				make_error_code(future_errc::no_state));
		return (_Assoc_state->_Get_value(_Get_only_once));
		}

	void _Set_value(const _Ty& _Val, bool _Defer)
		{	// store a result
		if (!valid())
			_Throw_future_error(
				make_error_code(future_errc::no_state));
		_Assoc_state->_Set_value(_Val, _Defer);
		}

	void _Set_value(_Ty&& _Val, bool _Defer)
		{	// store a result
		if (!valid())
			_Throw_future_error(
				make_error_code(future_errc::no_state));
		_Assoc_state->_Set_value(_STD forward<_Ty>(_Val), _Defer);
		}

#if __EDG__	/* compiler test */
	void _Set_value(_Ty& _Val, bool _Defer)
		{	// store a result
		if (!valid())
			_Throw_future_error(
				make_error_code(future_errc::no_state));
		_Assoc_state->_Set_value(_Val, _Defer);
		}
#endif /* __EDG__ */

	void _Set_exception(_XSTD exception_ptr _Exc, bool _Defer)
		{	// store a result
		if (!valid())
			_Throw_future_error(
				make_error_code(future_errc::no_state));
		_Assoc_state->_Set_exception(_Exc, _Defer);
		}

	void _Swap(_State_manager& _Other)
		{	// exchange with _Other
		_STD swap(_Assoc_state, _Other._Assoc_state);
		}

	_Associated_state<_Ty> *_Ptr() const
		{	// return pointer to stored associated asynchronous state object
		return (_Assoc_state);
		}

	void _Copy_from(const _State_manager& _Other)
		{	// copy stored associated asynchronous state object from _Other
		if (this != &_Other)
			{	// different, copy
			if (_Assoc_state)
				_Assoc_state->_Release();
			if (_Other._Assoc_state == 0)
				_Assoc_state = 0;
			else
				{	// do the copy
				_Other._Assoc_state->_Retain();
				_Assoc_state = _Other._Assoc_state;
				_Get_only_once = _Other._Get_only_once;
				}
			}
		}

	void _Move_from(_State_manager& _Other)
		{	// move stored associated asynchronous state object from _Other
		if (this != &_Other)
			{	// different, move
			if (_Assoc_state)
				_Assoc_state->_Release();
			_Assoc_state = _Other._Assoc_state;
			_Other._Assoc_state = 0;
			_Get_only_once = _Other._Get_only_once;
			}
		}

	bool _Is_ready() const
		{	// return status
		return (_Assoc_state && _Assoc_state->_Is_ready());
		}
	
	bool is_ready() const
		{	// return status
		return (_Assoc_state && _Assoc_state->_Is_ready());
		}

private:
	_Associated_state<_Ty> *_Assoc_state;
	bool _Get_only_once;
	};

	// TEMPLATE CLASS future
template<class _Ty>
	class shared_future;

template<class _Ty>
	class Future
		: public _State_manager<_Ty>
	{	// class that defines a non-copyable asynchronous return object
		// that holds a value
	friend class Promise<_Ty>;
	
	typedef _State_manager<_Ty> _Mybase;
	
	std::function<void()> _M_dtor_handler;
	
	void set_dtor_handler(std::function<void()> handler)
	{
		_M_dtor_handler = handler;
	}
public:
	Future() _NOEXCEPT
		{	// construct
		}

	Future(Future&& _Other) _NOEXCEPT
		: _Mybase(_STD forward<Future>(_Other), true)
		{	// construct from rvalue future object
		}

	Future& operator=(Future&& _Right) _NOEXCEPT
		{	// assign from rvalue future object
		_Mybase::operator=(_STD forward<Future>(_Right));
		return (*this);
		}

	Future(const _Mybase& _State, std::_Nil)
		: _Mybase(_State, true)
		{	// construct from associated asynchronous state object
		}

	~Future() _NOEXCEPT
		{	// destroy
			if (_M_dtor_handler)
			{
				_M_dtor_handler();
			}
		}

	_Ty get()
		{	// block until ready then return the stored result or
			// throw the stored exception
		return (_STD move(this->_Get_value()));
		}
		
	// Set a continuation for when the shared state is ready
	template <class F>
	auto then(F&& func) -> Future<decltype(func(std::move(*this)))>
	{
		if (this->is_ready())
		{
			func(std::move(*this));
		}
		else
		{
			this->set_continuation([&func,this](){
				func(std::move(*this));
			});
		}
		
		return Future<decltype(func(std::move(*this)))>();
	}

	shared_future<_Ty> share()
		{	// return state as shared_future
		return (shared_future<_Ty>(_STD move(*this)));
		}

#if _HAS_FUNCTION_DELETE
	Future(const Future&) = delete;
	Future& operator=(const Future&) = delete;

#else /* _HAS_FUNCTION_DELETE */
private:
	Future(const Future& _Other);
	Future& operator=(const Future& _Right);
#endif /* _HAS_FUNCTION_DELETE */
	};

template<class _Ty>
	class Future<_Ty&>
		: public _State_manager<_Ty *>
	{	// class that defines a non-copyable asynchronous return object
		// that holds a reference
	friend class Promise<_Ty&>;
	
	typedef _State_manager<_Ty *> _Mybase;
	
	std::function<void()> _M_dtor_handler;
	
	void set_dtor_handler(std::function<void()> handler)
	{
		_M_dtor_handler = handler;
	}
public:
	Future() _NOEXCEPT
		{	// construct
		}

	Future(Future&& _Other) _NOEXCEPT
		: _Mybase(_STD forward<Future>(_Other), true)
		{	// construct from rvalue future object
		}

	Future& operator=(Future&& _Right) _NOEXCEPT
		{	// assign from rvalue future object
		_Mybase::operator=(_STD forward<Future>(_Right));
		return (*this);
		}

	Future(const _Mybase& _State, std::_Nil)
		: _Mybase(_State, true)
		{	// construct from associated asynchronous state object
		}

	~Future() _NOEXCEPT
		{	// destroy
			if (_M_dtor_handler)
			{
				_M_dtor_handler();
			}
		}

	_Ty& get()
		{	// block until ready then return the stored result or
			// throw the stored exception
		return (*this->_Get_value());
		}
	// Set a continuation for when the shared state is ready
	template <class F>
	auto then(F&& func) -> Future<decltype(func(std::move(*this)))>
	{
		if (this->is_ready())
		{
			func(*this);
			return Future<decltype(func(*this))>();
		}
		
		return Future<decltype(func(*this))>();
	}

	shared_future<_Ty&> share()
		{	// return state as shared_future
		return (shared_future<_Ty&>(_STD move(*this)));
		}

#if _HAS_FUNCTION_DELETE
	Future(const Future&) = delete;
	Future& operator=(const Future&) = delete;

#else /* _HAS_FUNCTION_DELETE */
private:
	Future(const Future& _Other);
	Future& operator=(const Future& _Right);
#endif /* _HAS_FUNCTION_DELETE */
	};

template<>
	class Future<void>
		: public _State_manager<int>
	{	// class that defines a non-copyable asynchronous return object
		// that does not hold a value
	friend class Promise<void>;
	typedef _State_manager<int> _Mybase;
	
	std::function<void()> _M_dtor_handler;
	
	void set_dtor_handler(std::function<void()> handler)
	{
		_M_dtor_handler = handler;
	}
public:
	Future() _NOEXCEPT
		{	// construct
		}

	Future(Future&& _Other) _NOEXCEPT
		: _Mybase(_STD forward<Future>(_Other), true)
		{	// construct from rvalue future object
		}

	Future& operator=(Future&& _Right) _NOEXCEPT
		{	// assign from rvalue future object
		_Mybase::operator=(_STD forward<Future>(_Right));
		return (*this);
		}

	Future(const _Mybase& _State, std::_Nil)
		: _Mybase(_State, true)
		{	// construct from associated asynchronous state object
		}

	~Future() _NOEXCEPT
		{	// destroy
			if (_M_dtor_handler)
			{
				_M_dtor_handler();
			}
		}

	void get()
		{	// block until ready then return or
			// throw the stored exception
		this->_Get_value();
		}
		
	// Set a continuation for when the shared state is ready
	template <class F>
	auto then(F&& func) -> Future<decltype(func(std::move(*this)))>
	{
		if (this->is_ready())
		{
			func(*this);
			return Future<decltype(func(*this))>();
		}
		
		return Future<decltype(func(*this))>();
	}

	shared_future<void> share();

#if _HAS_FUNCTION_DELETE
	Future(const Future&) = delete;
	Future& operator=(const Future&) = delete;

#else /* _HAS_FUNCTION_DELETE */
private:
	Future(const Future& _Other);
	Future& operator=(const Future& _Right);
#endif /* _HAS_FUNCTION_DELETE */
	};

	// TEMPLATE CLASS shared_future
template<class _Ty>
	class shared_future
	: public _State_manager<_Ty>
	{	// class that defines a copyable asynchronous return object
		// that holds a value
	typedef _State_manager<_Ty> _Mybase;

public:
	shared_future() _NOEXCEPT
		{	// construct
		}

	shared_future(const shared_future& _Other)
		: _Mybase(_Other)
		{	// construct from shared_future object
		}

	shared_future& operator=(const shared_future& _Right)
		{	// assign from shared_future object
		_Mybase::operator=(_Right);
		return (*this);
		}

	shared_future(Future<_Ty>&& _Other) _NOEXCEPT
		: _Mybase(_STD forward<_Mybase>(_Other))
		{	// construct from rvalue future object
		}

	shared_future(shared_future&& _Other) _NOEXCEPT
		: _Mybase(_STD forward<shared_future>(_Other))
		{	// construct from rvalue shared_future object
		}

	shared_future& operator=(shared_future&& _Right) _NOEXCEPT
		{	// assign from shared_future rvalue object
		_Mybase::operator=(_STD forward<shared_future>(_Right));
		return (*this);
		}

#if __EDG__	/* compiler test */
	shared_future(shared_future& _Other)
		: _Mybase(const_cast<const shared_future&>(_Other))
		{	// construct from rvalue shared_future object
		}

	shared_future& operator=(shared_future& _Right)
		{	// assign from shared_future rvalue object
		_Mybase::operator=(const_cast<const shared_future&>(_Right));
		return (*this);
		}
#endif /* __EDG__ */

	~shared_future() _NOEXCEPT
		{	// destroy
		}

	const _Ty& get() const
		{	// block until ready then return the stored result or
			// throw the stored exception
		return (this->_Get_value());
		}
	};

template<class _Ty>
	class shared_future<_Ty&>
		: public _State_manager<_Ty *>
	{	// class that defines a copyable asynchronous return object
		// that holds a reference
	typedef _State_manager<_Ty *> _Mybase;

public:
	shared_future() _NOEXCEPT
		{	// construct
		}

	shared_future(const shared_future& _Other)
		: _Mybase(_Other)
		{	// construct from shared_future object
		}

	shared_future& operator=(const shared_future& _Right)
		{	// assign from shared_future object
		_Mybase::operator=(_Right);
		return (*this);
		}

	shared_future(Future<_Ty&>&& _Other) _NOEXCEPT
		: _Mybase(_STD forward<_Mybase>(_Other))
		{	// construct from rvalue future object
		}

	shared_future(shared_future&& _Other) _NOEXCEPT
		: _Mybase(_STD forward<shared_future>(_Other))
		{	// construct from rvalue shared_future object
		}

	shared_future& operator=(shared_future&& _Right) _NOEXCEPT
		{	// assign from rvalue shared_future object
		_Mybase::operator=(_STD forward<shared_future>(_Right));
		return (*this);
		}

#if __EDG__	/* compiler test */
	shared_future(shared_future& _Other)
		: _Mybase(const_cast<const shared_future&>(_Other))
		{	// construct from rvalue shared_future object
		}

	shared_future& operator=(shared_future& _Right)
		{	// assign from rvalue shared_future object
		_Mybase::operator=(const_cast<const shared_future&>(_Right));
		return (*this);
		}
#endif /* __EDG__ */

	~shared_future() _NOEXCEPT
		{	// destroy
		}

	_Ty& get() const
		{	// block until ready then return the stored result or
			// throw the stored exception
		return (*this->_Get_value());
		}
	};

template<>
	class shared_future<void>
	: public _State_manager<int>
	{	// class that defines a copyable asynchronous return object
		// that does not hold a value
	typedef _State_manager<int> _Mybase;

public:
	shared_future() _NOEXCEPT
		{	// construct
		}

	shared_future(const shared_future& _Other)
		: _Mybase(_Other)
		{	// construct from shared_future object
		}

	shared_future& operator=(const shared_future& _Right)
		{	// assign from shared_future object
		_Mybase::operator=(_Right);
		return (*this);
		}

	shared_future(shared_future&& _Other) _NOEXCEPT
		: _Mybase(_STD forward<shared_future>(_Other))
		{	// construct from rvalue shared_future object
		}

	shared_future(Future<void>&& _Other) _NOEXCEPT
		: _Mybase(_STD forward<_Mybase>(_Other))
		{	// construct from rvalue future object
		}

	shared_future& operator=(shared_future&& _Right)
		{	// assign from rvalue shared_future object
		_Mybase::operator=(_STD forward<shared_future>(_Right));
		return (*this);
		}

#if __EDG__	/* compiler test */
	shared_future(shared_future& _Other)
		: _Mybase(const_cast<const shared_future&>(_Other))
		{	// construct from rvalue shared_future object
		}

	shared_future& operator=(shared_future& _Right)
		{	// assign from rvalue shared_future object
		_Mybase::operator=(const_cast<const shared_future&>(_Right));
		return (*this);
		}
#endif /* __EDG__ */

	~shared_future() _NOEXCEPT
		{	// destroy
		}

	void get() const
		{	// block until ready then return or
			// throw the stored exception
		this->_Get_value();
		}
	};

	// DEFINITION OF future<void>::share()
inline shared_future<void> Future<void>::share()
	{	// return state as shared_future
	return (shared_future<void>(_STD move(*this)));
	}

	// TEMPLATE CLASS _Promise
template<class _Ty>
	class _Promise
	{	// class that implements core of promise
public:
	_Promise(_Associated_state<_Ty> *_State_ptr)
		: _State(_State_ptr, false),
			_Future_retrieved(false)
		{	// construct from associated asynchronous state object
		}

#if _HAS_FUNCTION_DELETE
	_Promise(const _Promise&) = delete;
	_Promise& operator=(const _Promise&) = delete;
#endif /* _HAS_FUNCTION_DELETE */

	_Promise(_Promise&& _Other)
		: _State(_STD forward<_State_manager<_Ty> >(_Other._State)),
			_Future_retrieved(_Other._Future_retrieved)
		{	// construct from rvalue _Promise object
		}

	_Promise& operator=(_Promise&& _Other)
		{	// assign from rvalue _Promise object
		_State = _STD move(_Other._State);
		_Future_retrieved = _Other._Future_retrieved;
		return (*this);
		}

	~_Promise() _NOEXCEPT
		{	// destroy
		}

	void _Swap(_Promise& _Other)
		{	// exchange with _Other
		_State._Swap(_Other._State);
		_STD swap(_Future_retrieved, _Other._Future_retrieved);
		}

	const _State_manager<_Ty>& _Get_state() const
		{	// return reference to associated asynchronous state object
		return (_State);
		}
	_State_manager<_Ty>& _Get_state()
		{	// return reference to associated asynchronous state object
		return (_State);
		}

	_State_manager<_Ty>& _Get_state_for_set()
		{	// return reference to associated asynchronous state object, or
			// throw exception if not valid for setting state
		if (!_State.valid())
			_Throw_future_error(
				make_error_code(future_errc::no_state));
		return (_State);
		}

	_State_manager<_Ty>& _Get_state_for_future()
		{	// return reference to associated asynchronous state object, or
			// throw exception if not valid for retrieving future
		if (!_State.valid())
			_Throw_future_error(
				make_error_code(future_errc::no_state));
		if (_Future_retrieved)
			_Throw_future_error(
				make_error_code(future_errc::future_already_retrieved));
		_Future_retrieved = true;
		return (_State);
		}

	bool _Is_valid() const
		{	// return status
		return (_State.valid());
		}

	bool _Is_ready() const
		{	// return status
		return (_State._Is_ready());
		}

private:
	_State_manager<_Ty> _State;
	bool _Future_retrieved;

#if _HAS_FUNCTION_DELETE

#else /* _HAS_FUNCTION_DELETE */
	_Promise(const _Promise&);	// not defined
	_Promise& operator=(const _Promise&);	// not defined
#endif /* _HAS_FUNCTION_DELETE */
	};

	// TEMPLATE CLASS promise
template<class _Ty>
	class Promise
	{	// class that defines an asynchronous provider that holds a value
public:
	Promise()
		: _MyPromise(new _Associated_state<_Ty>)
		{	// construct
		}

	template<class _Alloc>
		Promise(std::allocator_arg_t, const _Alloc& _Al)
		: _MyPromise(_Make_associated_state<_Ty>(_Al))
		{	// construct with allocator
		}

#if _HAS_FUNCTION_DELETE
	Promise(const Promise&) = delete;
	Promise& operator=(const Promise&) = delete;
#endif /* _HAS_FUNCTION_DELETE */

	Promise(Promise&& _Other) _NOEXCEPT
		: _MyPromise(_STD forward<_Promise<_Ty> >(_Other._MyPromise))
		{	// construct from rvalue promise object
		}

	Promise& operator=(Promise&& _Other) _NOEXCEPT
		{	// assign from rvalue promise object
		_MyPromise = _STD forward<_Promise<_Ty> >(_Other._MyPromise);
		return (*this);
		}

	~Promise() _NOEXCEPT
		{	// destroy
		if (_MyPromise._Is_valid() && !_MyPromise._Is_ready())
			{	// exception if destroyed before function object returns
			future_error _Fut(make_error_code(future_errc::broken_promise));
			_MyPromise._Get_state()
				._Set_exception(_XSTD make_exception_ptr(_Fut), false);
			}
		}

	void swap(Promise& _Other) _NOEXCEPT
		{	// exchange with _Other
		_MyPromise._Swap(_Other._MyPromise);
		}

	Future<_Ty> get_future()
		{	// return a future object that shares the associated
			// asynchronous state
			//return (Future<_Ty>(_MyPromise._Get_state_for_future(), std::_Nil_obj));
			Future<_Ty> f(_MyPromise._Get_state_for_future(), std::_Nil_obj);
			f.set_dtor_handler(_M_future_dtor_handler);
			return f;
		}

	void set_value(const _Ty& _Val)
		{	// store result
		_MyPromise._Get_state_for_set()._Set_value(_Val, false);
		}

	void set_value_at_thread_exit(const _Ty& _Val)
		{	// store result and block until thread exit
		_MyPromise._Get_state_for_set()._Set_value(_Val, true);
		}

	void set_value(_Ty&& _Val)
		{	// store result
		_MyPromise._Get_state_for_set()._Set_value(_STD forward<_Ty>(_Val), false);
		}

	void set_value_at_thread_exit(_Ty&& _Val)
		{	// store result and block until thread exit
		_MyPromise._Get_state_for_set()._Set_value(_STD forward<_Ty>(_Val), true);
		}

	void set_exception(_XSTD exception_ptr _Exc)
		{	// store result
		_MyPromise._Get_state_for_set()._Set_exception(_Exc, false);
		}

	void set_exception_at_thread_exit(_XSTD exception_ptr _Exc)
		{	// store result and block until thread exit
		_MyPromise._Get_state_for_set()._Set_exception(_Exc, true);
		}
		
	//Set a handler to be called, upon future destruction
	void set_future_dtor_handler(std::function<void()> handler)
	{
		_M_future_dtor_handler = handler;
	}

private:
	_Promise<_Ty> _MyPromise;
	std::function<void()> _M_future_dtor_handler;

#if _HAS_FUNCTION_DELETE

#else /* _HAS_FUNCTION_DELETE */
	Promise(const Promise&);	// not defined
	Promise& operator=(const Promise&);	// not defined
#endif /* _HAS_FUNCTION_DELETE */
	};

template<class _Ty>
	class Promise<_Ty&>
	{	// class that defines an asynchronous provider that holds a reference
public:
	Promise()
		: _MyPromise(new _Associated_state<_Ty *>)
		{	// construct
		}

	template<class _Alloc>
		Promise(std::allocator_arg_t, const _Alloc& _Al)
		: _MyPromise(_Make_associated_state<_Ty *>(_Al))
		{	// construct with allocator
		}

#if _HAS_FUNCTION_DELETE
	Promise(const Promise&) = delete;
	Promise& operator=(const Promise&) = delete;
#endif /* _HAS_FUNCTION_DELETE */

	Promise(Promise&& _Other) _NOEXCEPT
		: _MyPromise(_STD forward<_Promise<_Ty *> >(_Other._MyPromise))
		{	// construct from rvalue promise object
		}

	Promise& operator=(Promise&& _Other) _NOEXCEPT
		{	// assign from rvalue promise object
		_MyPromise = _STD forward<_Promise<_Ty *> >(_Other._MyPromise);
		return (*this);
		}

	~Promise() _NOEXCEPT
		{	// destroy
		}

	void swap(Promise& _Other) _NOEXCEPT
		{	// exchange with _Other
		_MyPromise._Swap(_Other._MyPromise);
		}

	Future<_Ty&> get_future()
		{	// return a future object that shares the associated
			// asynchronous state
			//return (Future<_Ty&>(_MyPromise._Get_state_for_future(), std::_Nil_obj));
			Future<_Ty&> f(_MyPromise._Get_state_for_future(), std::_Nil_obj);
			f.set_dtor_handler(_M_future_dtor_handler);
			return f;
		}

	void set_value(_Ty& _Val)
		{	// store result
		_MyPromise._Get_state_for_set()._Set_value(&_Val, false);
		}

	void set_value_at_thread_exit(_Ty& _Val)
		{	// store result and block until thread exit
		_MyPromise._Get_state_for_set()._Set_value(&_Val, true);
		}

	void set_exception(_XSTD exception_ptr _Exc)
		{	// store result
		_MyPromise._Get_state_for_set()._Set_exception(_Exc, false);
		}

	void set_exception_at_thread_exit(_XSTD exception_ptr _Exc)
		{	// store result and block until thread exit
		_MyPromise._Get_state_for_set()._Set_exception(_Exc, true);
		}
		
	//Set a handler to be called, upon future destruction
	void set_future_dtor_handler(std::function<void()> handler)
	{
		_M_future_dtor_handler = handler;
	}

private:
	_Promise<_Ty *> _MyPromise;
	std::function<void()> _M_future_dtor_handler;
	};

template<>
	class Promise<void>
	{	// defines an asynchronous provider that does not hold a value
public:
	Promise()
		: _MyPromise(new _Associated_state<int>)
		{	// construct
		}

	template<class _Alloc>
		Promise(std::allocator_arg_t, const _Alloc& _Al)
		: _MyPromise(_Make_associated_state<int>(_Al))
		{	// construct with allocator
		}

#if _HAS_FUNCTION_DELETE
	Promise(const Promise&) = delete;
	Promise& operator=(const Promise&) = delete;
#endif /* _HAS_FUNCTION_DELETE */

	Promise(Promise&& _Other) _NOEXCEPT
		: _MyPromise(_STD forward<_Promise<int> >(_Other._MyPromise))
		{	// construct from rvalue promise object
		}

	Promise& operator=(Promise&& _Other) _NOEXCEPT
		{	// assign from rvalue promise object
		_MyPromise = _STD forward<_Promise<int> >(_Other._MyPromise);
		return (*this);
		}

	~Promise() _NOEXCEPT
		{	// destroy
		}

	void swap(Promise& _Other) _NOEXCEPT
		{	// exchange with _Other
		_MyPromise._Swap(_Other._MyPromise);
		}

	Future<void> get_future()
		{	// return a future object that shares the associated
			// asynchronous state
			//return (Future<void>(_MyPromise._Get_state_for_future(), std::_Nil_obj));
			Future<void> f(_MyPromise._Get_state_for_future(), std::_Nil_obj);
			f.set_dtor_handler(_M_future_dtor_handler);
			return f;
		}

	void set_value()
		{	// store a (void) result
		_MyPromise._Get_state_for_set()._Set_value(1, false);
		}

	void set_value_at_thread_exit()
		{	// store result and block until thread exit
		_MyPromise._Get_state_for_set()._Set_value(1, true);
		}

	void set_exception(_XSTD exception_ptr _Exc)
		{	// store a result
		_MyPromise._Get_state_for_set()._Set_exception(_Exc, false);
		}

	void set_exception_at_thread_exit(_XSTD exception_ptr _Exc)
		{	// store result and block until thread exit
		_MyPromise._Get_state_for_set()._Set_exception(_Exc, true);
		}
		
	//Set a handler to be called, upon future destruction
	void set_future_dtor_handler(std::function<void()> handler)
	{
		_M_future_dtor_handler = handler;
	}

private:
	_Promise<int> _MyPromise;
	std::function<void()> _M_future_dtor_handler;
	};

template<class _Ty> inline
	void swap(Promise<_Ty>& _Left, Promise<_Ty>& _Right) _NOEXCEPT
	{	// exchange _Left and _Right
	_Left.swap(_Right);
	}

	// TEMPLATE CLASS packaged_task
template <class _Fret>
	struct _P_arg_type
	{	// type for function
	typedef _Fret type;
	};

template <class _Fret>
	struct _P_arg_type<_Fret&>
	{	// type for ref to function
	typedef _Fret *type;
	};

template <>
	struct _P_arg_type<void>
	{	// type for void function
	typedef int type;
	};

#if _HAS_VARIADIC_TEMPLATES
template<class>
	class packaged_task;	// not defined

template<class _Ret,
	class... _ArgTypes>
	class packaged_task<_Ret(_ArgTypes...)>
	{	// class that defines an asynchronous provider that returns the
		// result of a call to a function object
	typedef packaged_task<_Ret(_ArgTypes...)> _Myt;
	typedef typename _P_arg_type<_Ret>::type _Ptype;
	typedef _Promise<_Ptype> _MyPromiseType;
	typedef _State_manager<_Ptype> _MyStateManagerType;
	typedef _Packaged_state<_Ret(_ArgTypes...)> _MyStateType;

public:
	packaged_task() _NOEXCEPT
		: _MyPromise(0)
		{	// construct
		}

	#if _HAS_FUNCTION_DELETE
	packaged_task(packaged_task&) = delete;
	packaged_task& operator=(packaged_task&) = delete;
	#endif /* _HAS_FUNCTION_DELETE */

	template<class _Fty2>
		explicit packaged_task(_Fty2&& _Fnarg)
		: _MyPromise(new _MyStateType(_STD forward<_Fty2>(_Fnarg)))
		{	// construct from rvalue function object
		}

	packaged_task(packaged_task&& _Other) _NOEXCEPT
		: _MyPromise(_STD forward<_Promise<_Ret> >(_Other._MyPromise))
		{	// construct from rvalue packaged_task object
		}

	packaged_task& operator=(packaged_task&& _Other) _NOEXCEPT
		{	// assign from rvalue packaged_task object
		_MyPromise = _STD forward<_MyPromiseType>(_Other._MyPromise);
		return (*this);
		}

	template<class _Fty2,
		class _Alloc>
		explicit packaged_task(std::allocator_arg_t, const _Alloc& _Al,
			_Fty2&& _Fnarg)
		: _MyPromise(_Make_packaged_state<_Ret(_ArgTypes...)>(
			_STD forward<_Fty2>(_Fnarg)), _Al)
		{	// construct from rvalue function object and allocator
		}

	#if __EDG__	/* compiler test */
	template<class _Fty2,
		class _Alloc>
		explicit packaged_task(allocator_arg_t, const _Alloc& _Al,
			_Fty2 _Fnarg)
		: _MyPromise(_Make_packaged_state<_Ret(_ArgTypes...)>(_Fnarg, _Al))
		{	// construct from function object and allocator
		}
	#endif /* __EDG__ */

	~packaged_task() _NOEXCEPT
		{	// destroy
		}

	void swap(packaged_task& _Other) _NOEXCEPT
		{	// exchange with _Other
		_STD swap(_MyPromise, _Other._MyPromise);
		}

	_EXP_OP operator bool() const _NOEXCEPT	// retained
		{	// return status
		return (_MyPromise._Is_valid());
		}

	bool valid() const
		{	// return status
		return (_MyPromise._Is_valid());
		}

	Future<_Ret> get_future()
		{	// return a future object that shares the associated
			// asynchronous state
		return (Future<_Ret>(_MyPromise._Get_state_for_future(), std::_Nil_obj));
		}

	void operator()(_ArgTypes... _Args)
		{	// call the function object
		if (_MyPromise._Is_ready())
			_Throw_future_error(
				make_error_code(future_errc::promise_already_satisfied));
		_MyStateManagerType& _State = _MyPromise._Get_state_for_set();
		_MyStateType *_Ptr = static_cast<_MyStateType *>(_State._Ptr());
		_Ptr->_Call_immediate(_STD forward<_ArgTypes>(_Args)...);
		}

	void make_ready_at_thread_exit(_ArgTypes... _Args)
		{	// call the function object and block until thread exit
		if (_MyPromise._Is_ready())
			_Throw_future_error(
				make_error_code(future_errc::promise_already_satisfied));
		_MyStateManagerType& _State = _MyPromise._Get_state_for_set();
		_MyStateType *_Ptr = static_cast<_MyStateType *>(_State._Ptr());
		_Ptr->_Call_deferred(_STD forward<_ArgTypes>(_Args)...);
		}

	void reset()
		{	// reset to newly constructed state
		_MyStateManagerType& _State = _MyPromise._Get_state();
		_MyStateType *_MyState = static_cast<_MyStateType *>(_State._Ptr());
		std::function<_Ret(_ArgTypes...)> _Fnarg = _MyState->_Get_fn();
		_MyPromiseType _New_promise(new _MyStateType(_Fnarg, true));
		_MyPromise._Swap(_New_promise);
		}

private:
	_MyPromiseType _MyPromise;

	#if _HAS_FUNCTION_DELETE

	#else /* _HAS_FUNCTION_DELETE */
	packaged_task(const packaged_task&);	// not defined
	packaged_task& operator=(const packaged_task&);	// not defined
	#endif /* _HAS_FUNCTION_DELETE */
	};

#else /* _HAS_VARIADIC_TEMPLATES */
template<class>
	class packaged_task;	// not defined

	#if _HAS_FUNCTION_DELETE
#define _PACKAGED_TASK_FUNCTION_DELETES \
	packaged_task(packaged_task&) = delete; \
	packaged_task& operator=(packaged_task&) = delete;

	#else /* _HAS_FUNCTION_DELETE */
#define _PACKAGED_TASK_FUNCTION_DELETES \
private: \
	packaged_task(const packaged_task&);	/* not defined */ \
	packaged_task& operator=(const packaged_task&);	/* not defined */
	#endif /* _HAS_FUNCTION_DELETE */

#define _CLASS_PACKAGED_TASK( \
	TEMPLATE_LIST, PADDING_LIST, LIST, C, X1, X2, X3, X4) \
template<class _Ret _EX(C) LIST(_CLASS_TYPE)> \
	class packaged_task<_Ret(LIST(_TYPE))> \
	{	/* class that defines an asynchronous provider that returns the */ \
		/* result of a call to a function object */ \
	typedef packaged_task<_Ret(LIST(_TYPE))> _Myt; \
	typedef typename _P_arg_type<_Ret>::type _Ptype; \
	typedef _Promise<_Ptype> _MyPromiseType; \
	typedef _State_manager<_Ptype> _MyStateManagerType; \
	typedef _Packaged_state<_Ret(LIST(_TYPE))> _MyStateType; \
public: \
	packaged_task() _NOEXCEPT \
		: _MyPromise(0) \
		{	/* construct */ \
		} \
	template<class _Fty2> \
		explicit packaged_task(_Fty2&& _Fnarg) \
		: _MyPromise(new _MyStateType(_STD forward<_Fty2>(_Fnarg))) \
		{	/* construct from rvalue function object */ \
		} \
	template<class _Fty2> \
		explicit packaged_task(const _Fty2& _Fnarg) \
		: _MyPromise(new _MyStateType(_Fnarg)) \
		{	/* construct from rvalue function object */ \
		} \
	packaged_task(packaged_task&& _Other) _NOEXCEPT \
		: _MyPromise(_STD forward<_Promise<_Ret> >(_Other._MyPromise)) \
		{	/* construct from rvalue packaged_task object */ \
		} \
	packaged_task& operator=(packaged_task&& _Other) _NOEXCEPT \
		{	/* assign from rvalue packaged_task object */ \
		_MyPromise = _STD forward<_MyPromiseType>(_Other._MyPromise); \
		return (*this); \
		} \
	template<class _Fty2, \
		class _Alloc> \
		explicit packaged_task(allocator_arg_t, const _Alloc& _Al, \
			_Fty2&& _Fnarg) \
		: _MyPromise(_Make_packaged_state<_Ret(LIST(_TYPE))>( \
			_STD forward<_Fty2>(_Fnarg)), _Al) \
		{	/* construct from rvalue function object and allocator */ \
		} \
	~packaged_task() _NOEXCEPT \
		{	/* destroy */ \
		} \
	void swap(packaged_task& _Other) _NOEXCEPT \
		{	/* exchange with _Other */ \
		_STD swap(_MyPromise, _Other._MyPromise); \
		} \
	_EXP_OP operator bool() const _NOEXCEPT	/* retained */ \
		{	/* return status */ \
		return (_MyPromise._Is_valid()); \
		} \
	bool valid() const \
		{	/* return status */ \
		return (_MyPromise._Is_valid()); \
		} \
	future<_Ret> get_future() \
		{	/* return a future object that shares the associated */ \
			/* asynchronous state */ \
		return (future<_Ret>(_MyPromise._Get_state_for_future(), std::_Nil_obj)); \
		} \
	void operator()(LIST(_TYPE_ARG)) \
		{	/* call the function object */ \
		if (_MyPromise._Is_ready()) \
			_Throw_future_error( \
				make_error_code(future_errc::promise_already_satisfied)); \
		_MyStateManagerType& _State = _MyPromise._Get_state_for_set(); \
		_MyStateType *_Ptr = static_cast<_MyStateType *>(_State._Ptr()); \
		_Ptr->_Call_immediate(LIST(_FORWARD_ARG)); \
		} \
	void make_ready_at_thread_exit(LIST(_TYPE_ARG)) \
		{	/* call the function object and block until thread exit */ \
		if (_MyPromise._Is_ready()) \
			_Throw_future_error( \
				make_error_code(future_errc::promise_already_satisfied)); \
		_MyStateManagerType& _State = _MyPromise._Get_state_for_set(); \
		_MyStateType *_Ptr = static_cast<_MyStateType *>(_State._Ptr()); \
		_Ptr->_Call_deferred(LIST(_FORWARD_ARG)); \
		} \
	void reset() \
		{	/* reset to newly constructed state */ \
		_MyStateManagerType& _State = _MyPromise._Get_state(); \
		_MyStateType *_MyState = \
			static_cast<_MyStateType *>(_State._Ptr()); \
		std::function<_Ret(LIST(_TYPE))> _Fnarg = _MyState->_Get_fn(); \
		_MyPromiseType _New_promise(new _MyStateType(_Fnarg, true)); \
		_MyPromise._Swap(_New_promise); \
		} \
	_PACKAGED_TASK_FUNCTION_DELETES \
private: \
	_MyPromiseType _MyPromise; \
	};

_VARIADIC_EXPAND_0X(_CLASS_PACKAGED_TASK, , , , )
#undef _PACKAGED_TASK_FUNCTION_DELETES
#undef _CLASS_PACKAGED_TASK
#endif /* _HAS_VARIADIC_TEMPLATES */

template<class _Ty> inline
	void swap(packaged_task<_Ty>& _Left,
		packaged_task<_Ty>& _Right) _NOEXCEPT
	{	// exchange _Left and _Right
	_Left.swap(_Right);
	}

		// HELPERS FOR async
template<class _Fty>
	struct _Is_launch_type
		: std::false_type
	{	// tests for _Launch_type argument
	};

template<>
	struct _Is_launch_type<_Launch_type>
		: std::true_type
	{	// tests for _Launch_type argument
	};

template<class _Ret,
	class _Fty> inline
	_Associated_state<typename _P_arg_type<_Ret>::type>
		*_Get_associated_state(_Launch_type _Psync, _Fty&& _Fnarg)
	{	// construct associated asynchronous state object for the launch type
	switch (_Psync)
		{	// select launch type
	case launch::async:
		return (new _Task_async_state<_Ret, false>(
			_STD forward<_Fty >(_Fnarg)));

	case launch::deferred:
		return (new _Deferred_async_state<_Ret>(
			_STD forward<_Fty >(_Fnarg)));
	default:
		return (new _Task_async_state<_Ret, true>(
			_STD forward<_Fty >(_Fnarg)));
		}
	}

#if _HAS_VARIADIC_TEMPLATES
		// TEMPLATE FUNCTION _Async
template<class _Fty,
	class... _ArgTypes> inline
	Future<typename std::result_of<_Fty(_ArgTypes...)>::type>
	_Async(_Launch_type _Policy, _Fty&& _Fnarg,
		_ArgTypes&&... _Args)
	{	// return a future object whose associated asynchronous state
		// manages a function object
	typedef typename std::result_of<_Fty(_ArgTypes...)>::type _Ret;
	typedef typename _P_arg_type<_Ret>::type _Ptype;
	_Promise<_Ptype> _Pr(_Get_associated_state<_Ret>(_Policy,
		_STD bind(_STD forward<_Fty>(_Fnarg),
			_STD forward<_ArgTypes>(_Args)...)));
	return (Future<_Ret>(_Pr._Get_state_for_future(), std::_Nil_obj));
	}

		// TEMPLATE FUNCTION async
template<class _Fty,
	class... _ArgTypes> inline
	Future<typename std::result_of<
		typename std::enable_if<!_Is_launch_type<
			typename std::decay<_Fty>::type>::value, _Fty>
				::type(_ArgTypes...)>::type>
	async(_Fty&& _Fnarg, _ArgTypes&&... _Args)
	{	// return a future object whose associated asynchronous state
		// manages a function object
	return (_Async(launch::any, _Decay_copy(_STD forward<_Fty>(_Fnarg)),
		_Decay_copy(_STD forward<_ArgTypes>(_Args))...));
	}

template<class _Policy_type,
	class _Fty,
	class... _ArgTypes> inline
	Future<typename std::result_of<
		typename std::enable_if<_Is_launch_type<
			_Policy_type>::value, _Fty>
				::type(_ArgTypes...)>::type>
		async(_Policy_type _Policy, _Fty&& _Fnarg,
		_ArgTypes&&... _Args)
	{	// return a future object whose associated asynchronous state
		// manages a function object
	return (_Async(_Policy, _Decay_copy(_STD forward<_Fty>(_Fnarg)),
		_Decay_copy(_STD forward<_ArgTypes>(_Args))...));
	}

#else /* _HAS_VARIADIC_TEMPLATES */
		// TEMPLATE FUNCTIONS _Async AND async
#define _ASYNC_FUN( \
	TEMPLATE_LIST, PADDING_LIST, LIST, C, X1, X2, X3, X4) \
template<class _Fty _EX(C) LIST(_CLASS_TYPE)> inline \
	future<typename std::result_of<_Fty(LIST(_TYPE))>::type> \
	_Async(_Launch_type _Policy, _Fty&& _Fnarg \
		_EX(C) LIST(_TYPE_REFREF_ARG)) \
	{	/* return a future object that manages a function object */ \
	typedef typename std::result_of<_Fty(LIST(_TYPE))>::type _Ret; \
	typedef typename _P_arg_type<_Ret>::type _Ptype; \
	_Promise<_Ptype> _Pr(_Get_associated_state<_Ret>(_Policy, \
		_STD bind(_STD forward<_Fty>(_Fnarg) \
			_EX(C) LIST(_DECAY_COPY_FORWARD_ARG)))); \
	return (future<_Ret>(_Pr._Get_state_for_future(), std::_Nil_obj)); \
	} \
template<class _Fty _EX(C) LIST(_CLASS_TYPE)> inline \
	future<typename std::result_of< \
		typename enable_if<!_Is_launch_type< \
			typename std::decay<_Fty>::type>::value, _Fty> \
				::type(LIST(_TYPE))>::type> \
	async(_Fty&& _Fnarg _EX(C) LIST(_TYPE_REFREF_ARG)) \
	{	/* return a future object that manages a function object */ \
	return (_Async(launch::any, _Decay_copy(_STD forward<_Fty>(_Fnarg)) \
		_EX(C) LIST(_DECAY_COPY_FORWARD_ARG))); \
	} \
template<class _Policy_type, class _Fty _EX(C) LIST(_CLASS_TYPE)> inline \
	future<typename std::result_of< \
		typename enable_if<_Is_launch_type< \
			_Policy_type>::value, _Fty> \
				::type(LIST(_TYPE))>::type> \
		async(_Policy_type _Policy, _Fty&& _Fnarg \
			_EX(C) LIST(_TYPE_REFREF_ARG)) \
	{	/* return a future object that manages a function object */ \
	return (_Async(_Policy, _Decay_copy(_STD forward<_Fty>(_Fnarg)) \
		_EX(C) LIST(_DECAY_COPY_FORWARD_ARG))); \
	}

_VARIADIC_EXPAND_0X(_ASYNC_FUN, , , , )
#undef _ASYNC_FUN
#endif /* _HAS_VARIADIC_TEMPLATES */
	} // namespace com
} // namespace ara

namespace std {
	// TEMPLATE CLASS SPECIALIZATION is_error_code_enum
	template<>
	struct is_error_code_enum<ara::com::_Future_errc>
		: public true_type
	{	// tests for error_code enumeration
	};
	
template<class _Ty,
	class _Alloc>
	struct uses_allocator<ara::com::Promise<_Ty>, _Alloc>
		: true_type
	{	// asserts that promise<_Ty> can use an allocator
	};

template<class _Ty, class _Alloc>
	struct uses_allocator<ara::com::packaged_task<_Ty>, _Alloc>
		: true_type
	{	// asserts that packaged_task<_Ty> can use an allocator
	};
}	// namespace std

/*
 * Copyright (c) 1992-2013 by P.J. Plauger.  ALL RIGHTS RESERVED.
 * Consult your license regarding permissions and restrictions.
V6.40:1566 */

#if defined(__QNXNTO__) && defined(__USESRCVERSION)
#include <sys/srcversion.h>
__SRCVERSION("$URL$ $Rev$")
#endif

#endif // ARA_COM_FUTURE_H_
