#pragma once

#include<unordered_map>
#include<string>
#include<stdint.h>

#include<boost/any.hpp>
#include<boost/optional.hpp>
#include <unordered_set>

namespace general
{

	template<typename T>
	class PropertyT
	{
	public:

		template<typename U>
		PropertyT& operator()(const T& key, const U& value)
		{
			return SetValue(key, value);
		}

		template<typename U>
		typename std::enable_if<
			!(std::is_same<bool, U>::value ||
				std::is_same<char, U>::value ||
				std::is_same<unsigned char, U>::value ||
				std::is_same<int8_t, U>::value ||
				std::is_same<uint8_t, U>::value ||
				std::is_same<int16_t, U>::value ||
				std::is_same<uint16_t, U>::value ||
				std::is_same<int32_t, U>::value ||
				std::is_same<uint32_t, U>::value ||
				std::is_same<int64_t, U>::value ||
				std::is_same<uint64_t, U>::value ||
				std::is_same<float, U>::value ||
				std::is_same<double, U>::value),
			PropertyT&>::type
			SetValue(const T& key, const U& value)
		{
			prop_any_umap_[key] = value;
			return *this;
		}

		template<typename U>
		typename std::enable_if<
			std::is_same<bool, U>::value ||
			std::is_same<char, U>::value ||
			std::is_same<unsigned char, U>::value ||
			std::is_same<int8_t, U>::value ||
			std::is_same<uint8_t, U>::value ||
			std::is_same<int16_t, U>::value ||
			std::is_same<uint16_t, U>::value ||
			std::is_same<int32_t, U>::value ||
			std::is_same<uint32_t, U>::value ||
			std::is_same<int64_t, U>::value ||
			std::is_same<uint64_t, U>::value,
			PropertyT>::type
			SetValue(const T& key, const U& value)
		{
			prop_any_umap_[key] = (uint64_t)value;
			return *this;
		}

		template<typename U>
		typename std::enable_if<
			std::is_same<float, U>::value ||
			std::is_same<double, U>::value,
			PropertyT&>::type
			SetValue(const T& key, const U& value)
		{
			prop_any_umap_[key] = (double)value;
			return *this;
		}

		template<typename U>
		U GetValue(const T& key, const U& default_value) const
		{

			boost::optional<U> value = GetValue<U>(key);
			return value.value_or(default_value);
		}


		template<typename U>
		typename std::enable_if<
			!(std::is_same<bool, U>::value ||
				std::is_same<char, U>::value ||
				std::is_same<unsigned char, U>::value ||
				std::is_same<int8_t, U>::value ||
				std::is_same<uint8_t, U>::value ||
				std::is_same<int16_t, U>::value ||
				std::is_same<uint16_t, U>::value ||
				std::is_same<int32_t, U>::value ||
				std::is_same<uint32_t, U>::value ||
				std::is_same<int64_t, U>::value ||
				std::is_same<uint64_t, U>::value ||
				std::is_same<float, U>::value ||
				std::is_same<double, U>::value),
			boost::optional<U>>
			::type
			GetValue(const T& key) const
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

		template<typename U>
		typename std::enable_if<
			std::is_same<bool, U>::value ||
			std::is_same<char, U>::value ||
			std::is_same<unsigned char, U>::value ||
			std::is_same<int8_t, U>::value ||
			std::is_same<uint8_t, U>::value ||
			std::is_same<int16_t, U>::value ||
			std::is_same<uint16_t, U>::value ||
			std::is_same<int32_t, U>::value ||
			std::is_same<uint32_t, U>::value ||
			std::is_same<int64_t, U>::value ||
			std::is_same<uint64_t, U>::value,
			boost::optional<U>>
			::type
			GetValue(const T& key) const
		{
			try
			{
				auto it = prop_any_umap_.find(key);
				if (it != prop_any_umap_.end())
				{
					return static_cast<U>(boost::any_cast<uint64_t>(it->second));
				}

			}
			catch (...)
			{

			}

			return {};
		}

		template<typename U>
		typename std::enable_if<
			std::is_same<float, U>::value ||
			std::is_same<float, U>::value,
			boost::optional<U>>::type
			GetValue(const T& key) const
		{
			try
			{
				auto it = prop_any_umap_.find(key);
				if (it != prop_any_umap_.end())
				{
					return boost::any_cast<double>(it->second);
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
		Property& operator()(uint32_t key, const U& value)
		{
			prop_i_.SetValue(key, value);
			return *this;
		}

		template<typename U>
		Property& operator()(const std::string& key, const U& value)
		{
			prop_s_.SetValue(key, value);
			return *this;
		}

		template<typename U>
		Property& SetValue(uint32_t key, const U& value)
		{
			prop_i_.SetValue(key, value);
			return *this;
		}

		template<typename U>
		Property& SetValue(const std::string& key, const U& value)
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



}