#pragma once

#include<map>
#include<string>
#include <stdint.h>

class Property
{
public:

	Property operator()(const std::string& key, bool value)
	{
		prop_int_map_[key] = (uint64_t)value;
		return *this;
	}
	Property operator()(const std::string& key, int32_t value)
	{
		prop_int_map_[key] = (uint64_t)value;
		return *this;
	}
	Property operator()(const std::string& key, uint32_t value)
	{
		prop_int_map_[key] = (uint64_t)value;
		return *this;
	}
	Property operator()(const std::string& key, int64_t value)
	{
		prop_int_map_[key] = (uint64_t)value;
		return *this;
	}
	Property operator()(const std::string& key, uint64_t value)
	{
		prop_int_map_[key] = value;
		return *this;
	}

	Property operator()(const std::string& key, const std::string& value)
	{
		prop_string_map_[key] = value;
		return *this;
	}
	Property operator()(const std::string& key, const char* value)
	{
		prop_string_map_[key] = value;
		return *this;
	}
	Property operator()(const std::string& key, std::nullptr_t pointer)
	{
		prop_pointer_map_[key] = pointer;
		return *this;
	}

	template<typename pointer_type>
	Property operator()(const std::string& key, pointer_type* const point)
	{
		prop_pointer_map_[key] = (void)point;
		return *this;
	}

	bool GetValue(const std::string& key, bool default_value) const
	{
		auto iter = prop_int_map_.find(key);
		if (iter != prop_int_map_.end())
		{
			return (bool)(iter->second);
		}
		return default_value;
	}

	int32_t GetValue(const std::string& key, int32_t default_value) const
	{
		auto iter = prop_int_map_.find(key);
		if (iter != prop_int_map_.end())
		{
			return (int32_t)(iter->second);
		}
		return default_value;
	}
	uint32_t GetValue(const std::string& key, uint32_t default_value) const
	{
		auto iter = prop_int_map_.find(key);
		if (iter != prop_int_map_.end())
		{
			return (uint32_t)(iter->second);
		}
		return default_value;
	}

	int64_t GetValue(const std::string& key, int64_t default_value) const
	{
		auto iter = prop_int_map_.find(key);
		if (iter != prop_int_map_.end())
		{
			return (int64_t)(iter->second);
		}
		return default_value;
	}
	uint64_t GetValue(const std::string& key, uint64_t default_value) const
	{
		auto iter = prop_int_map_.find(key);
		if (iter != prop_int_map_.end())
		{
			return (uint64_t)(iter->second);
		}
		return default_value;
	}
	std::string GetValue(const std::string& key, const std::string& default_value) const
	{
		auto iter = prop_string_map_.find(key);
		if (iter != prop_string_map_.end())
		{
			return  iter->second;
		}
		return default_value;
	}

	template<typename pointer_type>
	pointer_type GetPointer(const std::string& key) const
	{
		auto iter = prop_pointer_map_ find(key);
		if (iter != prop_pointer_map_.end())
		{
			return reinterpret_cast<pointer_type(iter->second);
		}
		return nullptr;
	}

	void Clear()
	{
		prop_int_map_.clear();
		prop_string_map_.clear();
		prop_pointer_map_.clear();
	}

private:
	std::map<std::string, uint64_t> prop_int_map_;
	std::map<std::string, std::string> prop_string_map_;
	std::map<std::string, void*> prop_pointer_map_;
};
