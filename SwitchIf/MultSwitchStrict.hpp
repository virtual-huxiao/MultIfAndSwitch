/**
 * @author :			huxiaoer
 * @create time:	2021-7-25
 * @explain:
 *	Multicomponent swithch with Strict Mode;
 */

#pragma once
#include "Struct.h"
#include <assert.h>
#include <functional>
#include <array>

template <class Type, size_t Size>
class MultSwitchStrict{	//Strict Mode
	using ArrayType = std::array<Type, Size>;
	using CaseFun   = std::function<void()>;

public:
	MultSwitchStrict() :arr_{} {};

	template<class ...Args>
	MultSwitchStrict(Args... args) {
		setExpression(std::forward<Args>(args)...);
	}
	template<class ...Args>
	MultSwitchStrict& setExpression(Args... args)& {	//call this function must be left-value-object
		static_assert(sizeof...(args) == Size, "The number of parameters passed in does not match!");
		arr_ = { args... };
		is_assigned_ = true;
		is_continue_ = true;
		is_defaulted_ = false;
		is_matched_ = false;
		return*this;
	}

	MultSwitchStrict&& casedo
	(const ArrayType& arr, CaseFun fun , bool is_break = CaseMod::NotBreak)	&&
	{
		doFun(arr,fun,is_break);
		return std::move(*this);
	}

	MultSwitchStrict&& casedo
	(const ArrayType& arr, bool is_break = CaseMod::NotBreak)&&
	{
		doFun(arr, nullptr, is_break);
		return std::move(*this);
	}

	MultSwitchStrict& casedo
	(const ArrayType& arr, CaseFun fun, bool is_break = CaseMod::NotBreak) &
	{
		doFun(arr, fun, is_break);
		return *this;
	}

	MultSwitchStrict& casedo
	(const ArrayType& arr, bool is_break = CaseMod::NotBreak)&
	{
		doFun(arr, nullptr, is_break);
		return *this;
	}

	void defaultdo(CaseFun fun){
		if(is_continue_){
			if (fun) fun();
		}
		is_defaulted_ = true;
	}

	void end(){
		is_defaulted_ = true;
	}


private:
	void doFun(const ArrayType& arr,const CaseFun& fun, bool is_break){
		assert(!is_defaulted_);	//called defalutdo then calling casedo without reset(call setExpression)
		assert(is_assigned_); //The judgment condition was not passed in,but you called casedo or defaultdo!
		if(is_continue_){
			if (is_matched_) {
				if (fun) fun();
				is_continue_ = !is_break;
				return;
			}
			if(arr == arr_){
				if (fun) fun();
				is_continue_ = !is_break;
				is_matched_ = true;
			}
		}
	}

private:
	ArrayType arr_;
	bool is_assigned_{ false };
	bool is_continue_{ true };
	bool is_matched_{ false };
	bool is_defaulted_{ false };
};