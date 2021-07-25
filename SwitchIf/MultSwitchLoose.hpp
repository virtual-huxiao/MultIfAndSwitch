/**
 * @author :			huxiaoer
 * @create time:	2021-7-25
 * @explain:
 *	Multicomponent swithch with Loose Mode;
 */

#pragma once
#include "Struct.h"
#include <assert.h>
#include <functional>
#include <array>
#include <initializer_list>
#include <vector>

template <class Type, size_t Size>
class MultSwitchLoose {	//Strict Mode
	using ArrayType = std::array<Type, Size>;
	using ArgsList = std::initializer_list<Type>;
	using CaseFun = std::function<void()>;

public:
	MultSwitchLoose() :arr_{} { vec_.reserve(Size); };

	template<class ...Args>
	MultSwitchLoose(Args... args) 
		:MultSwitchLoose{}
	{
		setExpression(std::forward<Args>(args)...);
	}
	template<class ...Args>
	MultSwitchLoose& setExpression(Args... args)& {	//call this function must be left-value-object
		static_assert(sizeof...(args) == Size, "The number of parameters passed in does not match!");
		arr_ = { args... };
		is_assigned_  = true;
		is_continue_  = true;
		is_defaulted_ = false;
		is_matched_   = false;
		return*this;
	}

	MultSwitchLoose&& casedo
	(const ArgsList& arr, CaseFun fun, bool is_break = CaseMod::NotBreak)&&
	{
		doFun(arr, fun, is_break);
		return std::move(*this);
	}
	MultSwitchLoose&& casedo
	(const ArgsList& arr,bool is_break = CaseMod::NotBreak)&&
	{
		doFun(arr, nullptr, is_break);
		return std::move(*this);
	}

	MultSwitchLoose& casedo
	(const ArgsList& arr, CaseFun fun, bool is_break = CaseMod::NotBreak)&
	{
		doFun(arr, fun, is_break);
		return *this;
	}
	MultSwitchLoose& casedo
	(const ArgsList& arr, bool is_break = CaseMod::NotBreak)&
	{
		doFun(arr, nullptr, is_break);
		return *this;
	}

	void defaultdo(CaseFun fun) {
		if (is_continue_) {
			if (fun) fun();
		}
		is_defaulted_ = true;
	}

	void end(){
		is_defaulted_ = true;
	}


private:
	void doFun(const ArgsList& arr, const CaseFun& fun, bool is_break) {
		assert(!is_defaulted_);	//called defalutdo then calling casedo without reset(call setExpression)
		assert(arr.size() <= Size);	//Too many parameters!
		assert(is_assigned_); //The judgment condition was not passed in,but you called casedo or defaultdo!
		if (is_continue_) {

			if(is_matched_){
				if (fun) fun();
				is_continue_ = !is_break;
				return;
			}

			is_loose_same_  = true;
			vec_.clear();
			vec_.insert(vec_.end(), arr.begin(), arr.end());
			for(size_t i = 0; i<vec_.size(); ++i){
				if(vec_[i] != arr_[i]){
					is_loose_same_ = false;
					break;
				}
			}
			if (is_loose_same_) {
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
	bool is_loose_same_{ true };
	bool is_matched_{ false };
	bool is_defaulted_{ false };
	std::vector<Type> vec_;
};