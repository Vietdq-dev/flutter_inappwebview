#ifndef FLUTTER_INAPPWEBVIEW_PLUGIN_FLUTTER_UTIL_H_
#define FLUTTER_INAPPWEBVIEW_PLUGIN_FLUTTER_UTIL_H_

#include <flutter/encodable_value.h>

#include "util.h"

namespace flutter_inappwebview_plugin
{
  static inline flutter::EncodableValue make_fl_value()
  {
    return flutter::EncodableValue();
  }

  template<typename T>
  static inline flutter::EncodableValue make_fl_value(const T& val)
  {
    return flutter::EncodableValue(val);
  }

  template<typename T>
  static inline flutter::EncodableValue make_fl_value(const T* val)
  {
    return val == nullptr ? make_fl_value() : flutter::EncodableValue(val);
  }

  template<typename T>
  static inline flutter::EncodableValue make_fl_value(const std::vector<T>& vec)
  {
    auto encodableList = flutter::EncodableList{};
    for (auto const& val : vec) {
      encodableList.push_back(make_fl_value(val));
    }
    return encodableList;
  }

  template<typename K, typename T>
  static inline flutter::EncodableValue make_fl_value(const std::map<K, T>& map)
  {
    auto encodableMap = flutter::EncodableMap{};
    for (auto const& [key, val] : map) {
      encodableMap.insert({ make_fl_value(key), make_fl_value(val) });
    }
    return encodableMap;
  }

  template<typename T>
  static inline flutter::EncodableValue make_fl_value(const std::optional<T>& optional)
  {
    return optional.has_value() ? make_fl_value(optional.value()) : make_fl_value();
  }

  template<typename T>
  static inline flutter::EncodableValue make_fl_value(const std::optional<std::vector<T>>& optional)
  {
    if (!optional.has_value()) {
      return make_fl_value();
    }
    auto& vecValue = optional.value();
    auto encodableList = flutter::EncodableList{};
    for (auto const& val : vecValue) {
      encodableList.push_back(make_fl_value(val));
    }
    return encodableList;
  }

  template<typename K, typename T>
  static inline flutter::EncodableValue make_fl_value(const std::optional<std::map<K, T>>& optional)
  {
    if (!optional.has_value()) {
      return make_fl_value();
    }
    auto& mapValue = optional.value();
    auto encodableMap = flutter::EncodableMap{};
    for (auto const& [key, val] : mapValue) {
      encodableMap.insert({ make_fl_value(key), make_fl_value(val) });
    }
    return encodableMap;
  }

  template<typename T>
  static inline T get_fl_map_value(const flutter::EncodableMap& map, const char* string)
  {
    return std::get<T>(map.at(make_fl_value(string)));
  }

  template<typename T, typename std::enable_if<((!is_mappish<T>::value && !is_vector<T>::value) ||
    std::is_same<T, flutter::EncodableMap>::value || std::is_same<T, flutter::EncodableList>::value), int>::type* = nullptr>
  static inline std::optional<T> get_optional_fl_map_value(const flutter::EncodableMap& map, const char* string)
  {
    return make_pointer_optional<T>(std::get_if<T>(&map.at(make_fl_value(string))));
  }

  template<typename T>
  static inline T get_fl_map_value(const flutter::EncodableMap& map, const char* string, const T& defaultValue)
  {
    auto optional = get_optional_fl_map_value<T>(map, string);
    return !optional.has_value() ? defaultValue : optional.value();
  }

  template<typename T, typename std::enable_if<(is_mappish<T>::value && !std::is_same<T, flutter::EncodableMap>::value)>::type* = nullptr>
  static inline std::optional<T> get_optional_fl_map_value(const flutter::EncodableMap& map, const char* string)
  {
    using K = typename T::key_type;
    using V = typename T::mapped_type;

    auto flMap = std::get_if<flutter::EncodableMap>(&map.at(make_fl_value(string)));
    if (flMap) {
      T mapValue = {};
      for (auto itr = flMap->begin(); itr != flMap->end(); itr++) {
        mapValue.insert({ std::get<K>(itr->first),  std::get<V>(itr->second) });
      }
      return make_pointer_optional<T>(&mapValue);
    }
    return std::nullopt;
  }

  template<typename K, typename T>
  static inline std::map<K, T> get_fl_map_value(const flutter::EncodableMap& map, const char* string, const std::map<K, T>& defaultValue)
  {
    auto optional = get_optional_fl_map_value<std::map<K, T>>(map, string);
    return !optional.has_value() ? defaultValue : optional.value();
  }

  template<typename T, typename std::enable_if<(is_vector<T>::value && !std::is_same<T, flutter::EncodableList>::value), bool>::type* = nullptr>
  static inline std::optional<T> get_optional_fl_map_value(const flutter::EncodableMap& map, const char* string)
  {
    using V = typename T::value_type;

    auto flList = std::get_if<flutter::EncodableList>(&map.at(make_fl_value(string)));
    if (flList) {
      T vecValue(flList->size());
      for (auto itr = flList->begin(); itr != flList->end(); itr++) {
        vecValue.push_back(std::get<V>(*itr));
      }
      return make_pointer_optional<T>(&vecValue);
    }
    return std::nullopt;
  }

  template<typename T>
  static inline std::vector<T> get_fl_map_value(const flutter::EncodableMap& map, const char* string, const std::vector<T>& defaultValue)
  {
    auto optional = get_optional_fl_map_value<std::vector<T>>(map, string);
    return !optional.has_value() ? defaultValue : optional.value();
  }
}

#endif //FLUTTER_INAPPWEBVIEW_PLUGIN_FLUTTER_UTIL_H_