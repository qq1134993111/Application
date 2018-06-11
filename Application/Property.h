#pragma once

#include<unordered_map>
#include<string>
#include<stdint.h>

#include<boost/any.hpp>
#include<boost/optional.hpp>

namespace general
{

	template<typename T>
	class PropertyT
	{
	public:

		template<typename U>
		PropertyT operator()(const T& key, const U& value)
		{
			prop_any_umap_[key] = value;
			return *this;
		}

		template<typename U>
		PropertyT SetValue(const T& key, const U& value)
		{
			prop_any_umap_[key] = value;
			return *this;
		}

		template<typename U>
		U GetValue(const T& key, const U& default_value) const
		{
			try
			{
				auto it = prop_any_umap_.find(key);
				if (it != prop_any_umap_.end())
				{
					return boost::any_cast<U>(it->second);
				}

			}
			catch (...)
			{

			}

			return default_value;
		}

		template<typename U>
		boost::optional<U> GetValue(const T& key) const
		{
			try
			{
				auto it = prop_any_umap_.find(key);
				if (it != prop_any_umap_.end())
				{
					return boost::any_cast<U>(it->second);
				}

			}
			catch (...)
			{

			}

			return {};
		}

		void Clear()
		{
			prop_any_umap_.clear();
		}

	private:
		std::unordered_map<T, boost::any> prop_any_umap_;
	};


	class Property
	{
	public:
		Property() {}
		~Property() {}

		template<typename U>
		Property operator()(uint32_t key, const U& value)
		{
			prop_i_.SetValue(key, value);
			return *this;
		}

		template<typename U>
		Property operator()(const std::string& key, const U& value)
		{
			prop_s_.SetValue(key, value);
			return *this;
		}

		template<typename U>
		Property SetValue(uint32_t key, const U& value)
		{
			prop_i_.SetValue(key, value);
			return *this;
		}

		template<typename U>
		Property SetValue(const std::string& key, const U& value)
		{
			prop_s_.SetValue(key, value);
			return *this;
		}

		template<typename U>
		U GetValue(uint32_t key, const U& default_value) const
		{
			return prop_i_.GetValue(key, default_value);
		}

		template<typename U>
		U GetValue(const std::string& key, const U& default_value) const
		{
			return prop_s_.GetValue(key, default_value);
		}

		template<typename U>
		boost::optional<U> GetValue(uint32_t key) const
		{
			return prop_i_.GetValue(key);
		}

		template<typename U>
		boost::optional<U> GetValue(const std::string& key) const
		{
			return prop_s_.GetValue(key);
		}

		void Clear()
		{
			prop_i_.Clear();
			prop_s_.Clear();
		}
	private:
		PropertyT<uint32_t> prop_i_;
		PropertyT<std::string> prop_s_;
	};


	//class Property
	//{
	//public:
	//
	//	Property operator()(const std::string& key, bool value)
	//	{
	//		prop_int_map_[key] = (uint64_t)value;
	//		return *this;
	//	}
	//	Property operator()(const std::string& key, int32_t value)
	//	{
	//		prop_int_map_[key] = (uint64_t)value;
	//		return *this;
	//	}
	//	Property operator()(const std::string& key, uint32_t value)
	//	{
	//		prop_int_map_[key] = (uint64_t)value;
	//		return *this;
	//	}
	//	Property operator()(const std::string& key, int64_t value)
	//	{
	//		prop_int_map_[key] = (uint64_t)value;
	//		return *this;
	//	}
	//	Property operator()(const std::string& key, uint64_t value)
	//	{
	//		prop_int_map_[key] = value;
	//		return *this;
	//	}
	//
	//	Property operator()(const std::string& key, const std::string& value)
	//	{
	//		prop_string_map_[key] = value;
	//		return *this;
	//	}
	//	Property operator()(const std::string& key, const char* value)
	//	{
	//		prop_string_map_[key] = value;
	//		return *this;
	//	}
	//	Property operator()(const std::string& key, std::nullptr_t pointer)
	//	{
	//		prop_pointer_map_[key] = pointer;
	//		return *this;
	//	}
	//
	//	template<typename pointer_type>
	//	Property operator()(const std::string& key, pointer_type* const point)
	//	{
	//		prop_pointer_map_[key] = (void)point;
	//		return *this;
	//	}
	//
	//	bool GetValue(const std::string& key, bool default_value) const
	//	{
	//		auto iter = prop_int_map_.find(key);
	//		if (iter != prop_int_map_.end())
	//		{
	//			return (bool)(iter->second);
	//		}
	//		return default_value;
	//	}
	//
	//	int32_t GetValue(const std::string& key, int32_t default_value) const
	//	{
	//		auto iter = prop_int_map_.find(key);
	//		if (iter != prop_int_map_.end())
	//		{
	//			return (int32_t)(iter->second);
	//		}
	//		return default_value;
	//	}
	//	uint32_t GetValue(const std::string& key, uint32_t default_value) const
	//	{
	//		auto iter = prop_int_map_.find(key);
	//		if (iter != prop_int_map_.end())
	//		{
	//			return (uint32_t)(iter->second);
	//		}
	//		return default_value;
	//	}
	//
	//	int64_t GetValue(const std::string& key, int64_t default_value) const
	//	{
	//		auto iter = prop_int_map_.find(key);
	//		if (iter != prop_int_map_.end())
	//		{
	//			return (int64_t)(iter->second);
	//		}
	//		return default_value;
	//	}
	//	uint64_t GetValue(const std::string& key, uint64_t default_value) const
	//	{
	//		auto iter = prop_int_map_.find(key);
	//		if (iter != prop_int_map_.end())
	//		{
	//			return (uint64_t)(iter->second);
	//		}
	//		return default_value;
	//	}
	//	std::string GetValue(const std::string& key, const std::string& default_value) const
	//	{
	//		auto iter = prop_string_map_.find(key);
	//		if (iter != prop_string_map_.end())
	//		{
	//			return  iter->second;
	//		}
	//		return default_value;
	//	}
	//
	//	template<typename pointer_type>
	//	pointer_type GetPointer(const std::string& key) const
	//	{
	//		auto iter = prop_pointer_map_ find(key);
	//		if (iter != prop_pointer_map_.end())
	//		{
	//			return reinterpret_cast<pointer_type(iter->second);
	//		}
	//		return nullptr;
	//	}
	//
	//	void Clear()
	//	{
	//		prop_int_map_.clear();
	//		prop_string_map_.clear();
	//		prop_pointer_map_.clear();
	//	}
	//
	//private:
	//	std::unordered_map<std::string, uint64_t> prop_int_map_;
	//	std::unordered_map<std::string, std::string> prop_string_map_;
	//	std::unordered_map<std::string, void*> prop_pointer_map_;
	//};


}