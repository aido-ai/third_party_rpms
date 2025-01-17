#ifndef GOOGLE_RAW_STRING_H__
#define GOOGLE_RAW_STRING_H__

#include <google/protobuf/stubs/common.h>

#include <functional>
#include <google/protobuf/port_def.inc>
#include <memory>
#include <string>

namespace std {

// RawString
//
// A RawString is a container of a shared mem buffer, not like string_view or stringpiece.
// Difference with string_view is RawString use a shared_ptr to maintain buf lifecycle 
// and buf may not pointer to a real string object 
//
// The RawString API provides the following common API operations:
// * Create or assign RawString out of existing string data, memory, or other RawString
// * Swap Rawstring data and compare Cord equality
// * Write out RawString data by constructing a `string`
class PROTOBUF_EXPORT RawString {
 public:
  enum DEVICE_TYPE {
    UNKNOWN = 0,
    CPU = 1,
    GPU = 2,
  };

 public:
  RawString() = default;
  RawString(const RawString& str) = default;
  RawString(RawString&& rstr);

  RawString(const string& str);
  RawString(string&& str);
  RawString(const char* c);
  RawString(const char* c, size_t size);
  RawString(const shared_ptr<const char>& ptr, size_t size);
  
  ~RawString() = default;
  RawString& operator=(const RawString& str) = default;

 public:
  // copy assign
  void assign(const string& str);
  void assign(string&& str);
  void assign(const char* c);
  void assign(const char* c, size_t size);

  // non copy assign
  void assign(const shared_ptr<const char>& ptr, size_t size);
  void assign(const std::RawString& str);
  
  void swap(RawString &rstr); 
  
  void clear();

 public:
  bool empty() const { return own_data_ == nullptr || size_ == 0; }
  size_t size() const { return size_; }
  const char* data() const { return own_data_.get(); }
  shared_ptr<const char> shared_data() const { return own_data_; }
  
  bool operator==(const RawString&) const;

  void set_device(DEVICE_TYPE device) { device_ = device;}
  DEVICE_TYPE device() const { return device_; }

  void set_remote_device(DEVICE_TYPE remote_device) { remote_device_ = remote_device; }
  DEVICE_TYPE remote_device() const { return remote_device_; }

 public:
  string substr(size_t start, size_t length) const;
  string to_string() const;

 private:
  void set_string_data(const string* str);
  
 private:
  shared_ptr<const char> own_data_;
  size_t size_{0};

  DEVICE_TYPE device_{UNKNOWN};
  DEVICE_TYPE remote_device_{UNKNOWN};
};

}  // namespace std

#include <google/protobuf/port_undef.inc>

#endif