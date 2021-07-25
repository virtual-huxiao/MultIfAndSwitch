/**
 * @author :			huxiaoer
 * @create time:	2021-7-25
 * @explain:
 *	Multicomponent if;
 */

#pragma once
#include <array>
#include <functional>
#include <cassert>

template <class Type, size_t Size>
class MultIf{
	using ArrayType = std::array<Type, Size>;
	using CallFun   = std::function<void()>;

public:
	MultIf() :arr_{} {};

	template<class ...Args>
	MultIf(Args... args) {
		setExpression(std::forward<Args>(args)...);
	}

	template<class ...Args>
	MultIf& setExpression(Args... args) &{	//call this function must be left-value-object
		static_assert(sizeof...(args) == Size, "The number of parameters passed in does not match!");
		arr_ = { args... };
		is_assigned_ = true;
		is_continue_ = true;
		is_elsed_    = false;
		return* this;
	}


	MultIf&& ifdo(const ArrayType& arr, CallFun fun) &&{
		doFun(arr, fun);
		return std::move(*this);
	}

	MultIf& ifdo(const ArrayType& arr, CallFun fun) &{
		doFun(arr, fun);
		return *this;
	}
	
	void elsedo(CallFun fun){
		assert(is_assigned_);	//The judgment condition was not passed in,but you called ifdo or elsedo!
		if (is_continue_) {
			if (fun) fun();
		}
		is_elsed_ = true;
	}

	void end() {
		is_elsed_ = true;
	}
	

private:
	void doFun(const ArrayType& arr,const CallFun& fun){
		assert(!is_elsed_);	//called elsedo then calling ifdo without reset(call setExpression)
		assert(is_assigned_);	//The judgment condition was not passed in,but you called ifdo or elsedo!
		if (is_continue_) {
			if (arr == arr_) {
				if (fun) fun();
				is_continue_ = false;
			}
		}
	}


private:
	ArrayType arr_;
	bool is_assigned_{ false };
	bool is_continue_{ true };
	bool is_elsed_{ false };
};