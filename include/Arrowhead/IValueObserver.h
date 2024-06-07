#pragma once

#include <ostream>

template<typename T>
class IValueObserver
{
public:
	template<typename... Args>
	IValueObserver(Args&&... args)
		: m_Value(T(std::forward<Args>(args)...)) {}

	// Simple Unary Operators
	auto operator+() { return +m_Value; }
	auto operator-() { return -m_Value; }
	auto operator*() { return *m_Value; }
	auto operator&() { return &m_Value; }
	auto operator!() { return !m_Value; }
	auto operator~() { return ~m_Value; }

	// Increment/Decrement operators
	auto operator++()
	{
		OnValueModified();
		return ++m_Value;
	}

	auto operator++(int)
	{
		OnValueModified();
		return m_Value++;
	}

	auto operator--()
	{
		OnValueModified();
		return --m_Value;
	}

	auto operator--(int)
	{
		OnValueModified();
		return m_Value--;
	}

	// Multiplicative Operators
	template<typename T2>
	auto operator*(const T2& other) const { return m_Value * other; }

	template<typename T2>
	auto operator/(const T2& other) const { return m_Value / other; }

	template<typename T2>
	auto operator%(const T2& other) const { return m_Value % other; }

	template<typename T2>
	auto operator*(const T2&& other) const { return m_Value * other; }

	template<typename T2>
	auto operator/(const T2&& other) const { return m_Value / other; }

	template<typename T2>
	auto operator%(const T2&& other) const { return m_Value % other; }

	// Additive Operators
	template<typename T2>
	auto operator+(const T2& other) const { return m_Value + other; }

	template<typename T2>
	auto operator-(const T2& other) const { return m_Value - other; }

	template<typename T2>
	auto operator+(const T2&& other) const { return m_Value + other; }

	template<typename T2>
	auto operator-(const T2&& other) const { return m_Value - other; }

	// Shift Operators
	template<typename T2>
	auto operator>>(const T2& other) const { return m_Value >> other; }

	template<typename T2>
	auto operator<<(const T2& other) const { return m_Value << other; }

	template<typename T2>
	auto operator>>(const T2&& other) const { return m_Value >> other; }

	template<typename T2>
	auto operator<<(const T2&& other) const { return m_Value << other; }

	// Relational and Equality Operators
	template<typename T2>
	auto operator<(const T2& other) const { return m_Value < other; }

	template<typename T2>
	auto operator>(const T2& other) const { return m_Value > other; }

	template<typename T2>
	auto operator<=(const T2& other) const { return m_Value <= other; }

	template<typename T2>
	auto operator>=(const T2& other) const { return m_Value >= other; }

	template<typename T2>
	auto operator==(const T2& other) const { return m_Value == other; }

	template<typename T2>
	auto operator!=(const T2& other) const { return m_Value != other; }

	template<typename T2>
	auto operator<(const T2&& other) const { return m_Value < other; }

	template<typename T2>
	auto operator>(const T2&& other) const { return m_Value > other; }

	template<typename T2>
	auto operator<=(const T2&& other) const { return m_Value <= other; }

	template<typename T2>
	auto operator>=(const T2&& other) const { return m_Value >= other; }

	template<typename T2>
	auto operator==(const T2&& other) const { return m_Value == other; }

	template<typename T2>
	auto operator!=(const T2&& other) { return m_Value != other; }

	// Bitwise Operators
	template<typename T2>
	auto operator&(const T2& other) const { return m_Value & other; }

	template<typename T2>
	auto operator^(const T2& other) const { return m_Value ^ other; }

	template<typename T2>
	auto operator|(const T2& other) const { return m_Value | other; }

	template<typename T2>
	auto operator&(const T2&& other) const { return m_Value & other; }

	template<typename T2>
	auto operator^(const T2&& other) const { return m_Value ^ other; }

	template<typename T2>
	auto operator|(const T2&& other) const { return m_Value | other; }

	// Logical Operators
	template<typename T2>
	auto operator&&(const T2& other) const { return m_Value && other; }

	template<typename T2>
	auto operator||(const T2& other) const { return m_Value || other; }

	template<typename T2>
	auto operator&&(const T2&& other) const { return m_Value && other; }

	template<typename T2>
	auto operator||(const T2&& other) const { return m_Value || other; }

	// Assignment Operators

	template<typename T2>
	auto operator=(const T2& other)
	{
		OnValueModified();
		return m_Value = other;
	}

	template<typename T2>
	auto operator=(const T2&& other)
	{
		OnValueModified();
		return m_Value = other;
	}

	template<typename T2>
	auto operator+=(const T2& other)
	{
		OnValueModified();
		return m_Value += other;
	}

	template<typename T2>
	auto operator+=(const T2&& other)
	{
		OnValueModified();
		return m_Value += other;
	}

	template<typename T2>
	auto operator-=(const T2& other)
	{
		OnValueModified();
		return m_Value -= other;
	}

	template<typename T2>
	auto operator-=(const T2&& other)
	{
		OnValueModified();
		return m_Value -= other;
	}

	template<typename T2>
	auto operator*=(const T2& other)
	{
		OnValueModified();
		return m_Value *= other;
	}

	template<typename T2>
	auto operator*=(const T2&& other)
	{
		OnValueModified();
		return m_Value *= other;
	}

	template<typename T2>
	auto operator/=(const T2& other)
	{
		OnValueModified();
		return m_Value /= other;
	}

	template<typename T2>
	auto operator/=(const T2&& other)
	{
		OnValueModified();
		return m_Value /= other;
	}

	template<typename T2>
	auto operator%=(const T2& other)
	{
		OnValueModified();
		return m_Value %= other;
	}

	template<typename T2>
	auto operator%=(const T2&& other)
	{
		OnValueModified();
		return m_Value %= other;
	}

	template<typename T2>
	auto operator<<=(const T2& other)
	{
		OnValueModified();
		return m_Value <<= other;
	}

	template<typename T2>
	auto operator<<=(const T2&& other)
	{
		OnValueModified();
		return m_Value <<= other;
	}

	template<typename T2>
	auto operator>>=(const T2& other)
	{
		OnValueModified();
		return m_Value >>= other;
	}

	template<typename T2>
	auto operator>>=(const T2&& other)
	{
		OnValueModified();
		return m_Value >>= other;
	}

	template<typename T2>
	auto operator&=(const T2& other)
	{
		OnValueModified();
		return m_Value &= other;
	}

	template<typename T2>
	auto operator&=(const T2&& other)
	{
		OnValueModified();
		return m_Value &= other;
	}

	template<typename T2>
	auto operator^=(const T2& other)
	{
		OnValueModified();
		return m_Value ^= other;
	}

	template<typename T2>
	auto operator^=(const T2&& other)
	{
		OnValueModified();
		return m_Value ^= other;
	}

	template<typename T2>
	auto operator|=(const T2& other)
	{
		OnValueModified();
		return m_Value |= other;
	}

	template<typename T2>
	auto operator|=(const T2&& other)
	{
		OnValueModified();
		return m_Value |= other;
	}

	template<typename... Args>
	auto operator()(Args&& ...args) { return m_Value(std::forward<Args>(args)...); }

	template<typename T2>
	auto operator[](const T2& other) { return m_Value[other]; }

	T* operator->() { return &m_Value; }
	const T* operator->() const { return &m_Value; }

	T& Value() { return m_Value; }
	operator T& () { return Value(); }

	const T& Value() const { return m_Value; }
	operator const T& () const { return Value(); }

	// For when a value is changed without an operator, like a function call
	void ManualTrigger() { OnValueModified(); }

private:
	T m_Value;

	// Called right before it is modified
	virtual void OnValueModified() = 0;

	friend std::ostream& operator<<(std::ostream& os, const IValueObserver<T>& observer)
	{
		os << observer.m_Value;
		return os;
	}
};