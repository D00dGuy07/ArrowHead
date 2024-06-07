#pragma once

#include <atomic>
#include <type_traits>

namespace Arrow
{
	class RefCount
	{
	public:
		void IncRefCount() const
		{
			++m_RefCount;
		}
		void DecRefCount() const
		{
			--m_RefCount;
		}

		uint32_t GetRefCount() const { return m_RefCount.load(); }
	private:
		mutable std::atomic<uint32_t> m_RefCount = 0;
	};

	template<typename T>
	class Ref
	{
	public:
		Ref()
			: m_Value(nullptr)
		{
			static_assert(std::is_base_of<RefCount, T>::value, "Ref class can only be used with types that inherit from RefCount!");
		}

		Ref(std::nullptr_t)
			: m_Value(nullptr) {}

		template<typename J>
		Ref(Ref<J>& value)
			: m_Value(dynamic_cast<T*>(value.Raw()))
		{
			IncRef();
		}

		Ref(const Ref<T>& other)
			: m_Value(other.m_Value)
		{
			IncRef();
		}

		Ref(T* value)
			: m_Value(value)
		{
			IncRef();
		}

		~Ref()
		{
			DecRef();
		}

		Ref& operator=(std::nullptr_t)
		{
			DecRef();
			m_Value = nullptr;
			return *this;
		}

		template<typename J>
		Ref& operator=(const Ref<J>& other)
		{
			other.IncRef();
			DecRef();

			m_Value = other.m_Value;
			return *this;
		}

		template<typename J>
		Ref& operator=(Ref<J>&& other)
		{
			DecRef();

			m_Value = other.m_Value;
			other.m_Value = nullptr;
			return *this;
		}

		operator bool() { return m_Value != nullptr; }
		operator bool() const { return m_Value != nullptr; }

		T* operator->() { return m_Value; }
		const T* operator->() const { return m_Value; }

		T& operator*() { return *m_Value; }
		const T& operator*() const { return *m_Value; }

		T* Raw() { return m_Value; }
		const T* Raw() const { return m_Value; }

		bool operator==(const Ref<T>& other) const { return m_Value == other.m_Value; }
		bool operator!=(const Ref<T>& other) const { return m_Value != other.m_Value; }

		template<typename J>
		Ref<J> As() const { return Ref<J>(dynamic_cast<J*>(m_Value)); }

		template<typename... Args>
		static Ref<T> Create(Args&&... args) { return Ref<T>(new T(std::forward<Args>(args)...)); }

	private:
		T* m_Value;

		void IncRef()
		{
			if (m_Value)
				m_Value->IncRefCount();
		}

		void DecRef()
		{
			if (m_Value)
			{
				m_Value->DecRefCount();
				if (m_Value->GetRefCount() == 0)
				{
					delete m_Value;
					m_Value = nullptr;
				}
			}
		}
	};
}