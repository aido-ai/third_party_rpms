#include <google/protobuf/rawstring.h>
#include <google/protobuf/testing/googletest.h>
#include <gtest/gtest.h>
#include <string.h>

namespace std {

void CHECK_EMPTY(RawString &rstr) {
  EXPECT_EQ(rstr.size(), 0);
  EXPECT_TRUE(rstr.data() == nullptr);
  EXPECT_TRUE(rstr.shared_data() == nullptr);
  EXPECT_EQ(RawString::UNKNOWN, rstr.device());
  EXPECT_EQ(RawString::UNKNOWN, rstr.remote_device());
}

TEST(RawStringTest, ConstructTest_default) {
  RawString rstr;
  CHECK_EMPTY(rstr);
}

TEST(RawStringTest, ConstructTest_string) {
  string str("hello");
  RawString rstr(str);
  EXPECT_EQ(5, rstr.size());
  EXPECT_TRUE(rstr.data() != nullptr);
  EXPECT_TRUE(rstr.shared_data() != nullptr);
  EXPECT_EQ(0, strncmp(rstr.data(), str.c_str(), rstr.size()));
  EXPECT_EQ(RawString::UNKNOWN, rstr.device());
  EXPECT_EQ(RawString::UNKNOWN, rstr.remote_device());
}

TEST(RawStringTest, ConstructTest_stringMove) {
  string str("hello");
  string str2 = str;
  RawString rstr(std::move(str2));
  EXPECT_TRUE(str2.empty());
  EXPECT_EQ(5, rstr.size());
  EXPECT_TRUE(rstr.data() != nullptr);
  EXPECT_TRUE(rstr.shared_data() != nullptr);
  EXPECT_EQ(0, strncmp(rstr.data(), str.c_str(), rstr.size()));
  EXPECT_EQ(RawString::UNKNOWN, rstr.device());
  EXPECT_EQ(RawString::UNKNOWN, rstr.remote_device());
}

TEST(RawStringTest, ConstructTest_char) {
  string str("hello");
  RawString rstr(str.c_str());
  EXPECT_EQ(5, rstr.size());
  EXPECT_TRUE(rstr.data() != nullptr);
  EXPECT_TRUE(rstr.shared_data() != nullptr);
  EXPECT_EQ(0, strncmp(rstr.data(), str.c_str(), rstr.size()));
  EXPECT_EQ(RawString::UNKNOWN, rstr.device());
  EXPECT_EQ(RawString::UNKNOWN, rstr.remote_device());
}

TEST(RawStringTest, ConstructTest_charSize) {
  string str("hello");
  RawString rstr(str.c_str(), str.size());
  EXPECT_EQ(5, rstr.size());
  EXPECT_TRUE(rstr.data() != nullptr);
  EXPECT_TRUE(rstr.shared_data() != nullptr);
  EXPECT_EQ(0, strncmp(rstr.data(), str.c_str(), rstr.size()));
  EXPECT_EQ(RawString::UNKNOWN, rstr.device());
  EXPECT_EQ(RawString::UNKNOWN, rstr.remote_device());
}

TEST(RawStringTest, ConstructTest_sharedData) {
  string str("hello");
  shared_ptr<const char> shared_str(str.c_str(), [](const char *) {});

  RawString rstr(shared_str, str.size());
  EXPECT_EQ(5, rstr.size());
  EXPECT_EQ(str.c_str(), rstr.data());
  EXPECT_EQ(shared_str, rstr.shared_data());
  EXPECT_EQ(RawString::UNKNOWN, rstr.device());
  EXPECT_EQ(RawString::UNKNOWN, rstr.remote_device());
}

TEST(RawStringTest, CopyConstructTest_empty) {
  RawString rstr1;
  CHECK_EMPTY(rstr1);

  RawString rstr2(rstr1);
  CHECK_EMPTY(rstr2);
}

TEST(RawStringTest, CopyConstructTest_default) {
  string str("hello");
  shared_ptr<const char> shared_str(str.c_str(), [](const char *) {});

  RawString rstr1(shared_str, str.size());
  rstr1.set_device(RawString::CPU);
  rstr1.set_remote_device(RawString::GPU);

  EXPECT_EQ(5, rstr1.size());
  EXPECT_EQ(str.c_str(), rstr1.data());
  EXPECT_EQ(shared_str, rstr1.shared_data());
  EXPECT_EQ(RawString::CPU, rstr1.device());
  EXPECT_EQ(RawString::GPU, rstr1.remote_device());

  RawString rstr2(rstr1);

  EXPECT_EQ(5, rstr1.size());
  EXPECT_EQ(str.c_str(), rstr1.data());
  EXPECT_EQ(shared_str, rstr1.shared_data());
  EXPECT_EQ(RawString::CPU, rstr1.device());
  EXPECT_EQ(RawString::GPU, rstr1.remote_device());

  EXPECT_EQ(5, rstr2.size());
  EXPECT_EQ(str.c_str(), rstr2.data());
  EXPECT_EQ(shared_str, rstr2.shared_data());
  EXPECT_EQ(RawString::CPU, rstr2.device());
  EXPECT_EQ(RawString::GPU, rstr2.remote_device());
}

TEST(RawStringTest, MoveConstructTest_empty) {
  RawString rstr1;
  CHECK_EMPTY(rstr1);

  RawString rstr2(std::move(rstr1));
  CHECK_EMPTY(rstr2);
  CHECK_EMPTY(rstr1);
}

TEST(RawStringTest, MoveConstructTest_default) {
  string str("hello");
  shared_ptr<const char> shared_str(str.c_str(), [](const char *) {});

  RawString rstr1(shared_str, str.size());
  rstr1.set_device(RawString::CPU);
  rstr1.set_remote_device(RawString::GPU);

  EXPECT_EQ(5, rstr1.size());
  EXPECT_EQ(str.c_str(), rstr1.data());
  EXPECT_EQ(shared_str, rstr1.shared_data());
  EXPECT_EQ(RawString::CPU, rstr1.device());
  EXPECT_EQ(RawString::GPU, rstr1.remote_device());

  RawString rstr2(std::move(rstr1));

  CHECK_EMPTY(rstr1);

  EXPECT_EQ(5, rstr2.size());
  EXPECT_EQ(str.c_str(), rstr2.data());
  EXPECT_EQ(shared_str, rstr2.shared_data());
  EXPECT_EQ(RawString::CPU, rstr2.device());
  EXPECT_EQ(RawString::GPU, rstr2.remote_device());
}

TEST(RawStringTest, OperatorSetTest_empty) {
  RawString rstr1;
  CHECK_EMPTY(rstr1);
  
  string str("hello");
  shared_ptr<const char> shared_str(str.c_str(), [](const char *) {});

  RawString rstr2(shared_str, str.size());
  rstr2.set_device(RawString::CPU);
  rstr2.set_remote_device(RawString::GPU);

  EXPECT_EQ(5, rstr2.size());
  EXPECT_EQ(str.c_str(), rstr2.data());
  EXPECT_EQ(shared_str, rstr2.shared_data());
  EXPECT_EQ(RawString::CPU, rstr2.device());
  EXPECT_EQ(RawString::GPU, rstr2.remote_device());

  rstr2 = rstr1;
  CHECK_EMPTY(rstr1);
  CHECK_EMPTY(rstr2);
}

TEST(RawStringTest, OperatorSetTest_default) {
  string str1("hello");
  shared_ptr<const char> shared_str1(str1.c_str(), [](const char*) {});
  RawString rstr1(shared_str1, str1.size());
  rstr1.set_device(RawString::CPU);
  rstr1.set_remote_device(RawString::GPU);

  EXPECT_EQ(rstr1.size(), 5);
  EXPECT_EQ(str1.c_str(), rstr1.data());
  EXPECT_EQ(shared_str1, rstr1.shared_data());
  EXPECT_EQ(RawString::CPU, rstr1.device());
  EXPECT_EQ(RawString::GPU, rstr1.remote_device());
  
  string str2("world");
  shared_ptr<const char> shared_str2(str2.c_str(), [](const char*) {});
  RawString rstr2(shared_str2, str2.size());
  rstr2.set_device(RawString::GPU);
  rstr2.set_remote_device(RawString::CPU);

  EXPECT_EQ(rstr2.size(), 5);
  EXPECT_EQ(str2.c_str(), rstr2.data());
  EXPECT_EQ(shared_str2, rstr2.shared_data());
  EXPECT_EQ(RawString::GPU, rstr2.device());
  EXPECT_EQ(RawString::CPU, rstr2.remote_device());

  rstr1 = rstr2;

  EXPECT_EQ(rstr1.size(), 5);
  EXPECT_EQ(str2.c_str(), rstr1.data());
  EXPECT_EQ(shared_str2, rstr1.shared_data());
  EXPECT_EQ(RawString::GPU, rstr1.device());
  EXPECT_EQ(RawString::CPU, rstr1.remote_device());

  EXPECT_EQ(rstr2.size(), 5);
  EXPECT_EQ(str2.c_str(), rstr2.data());
  EXPECT_EQ(shared_str2, rstr2.shared_data());
  EXPECT_EQ(RawString::GPU, rstr2.device());
  EXPECT_EQ(RawString::CPU, rstr2.remote_device());
}

TEST(RawStringTest, AssignTest_string) {
  RawString rstr;
  CHECK_EMPTY(rstr);

  rstr.set_device(RawString::CPU);
  rstr.set_remote_device(RawString::GPU);
  EXPECT_EQ(RawString::CPU, rstr.device());
  EXPECT_EQ(RawString::GPU, rstr.remote_device()); 

  string str("hello");
  rstr.assign(str);
  EXPECT_EQ(5, rstr.size());
  EXPECT_TRUE(rstr.data() != nullptr);
  EXPECT_TRUE(rstr.shared_data() != nullptr);
  EXPECT_EQ(0, strncmp(rstr.data(), str.c_str(), rstr.size()));
  EXPECT_EQ(RawString::UNKNOWN, rstr.device());
  EXPECT_EQ(RawString::UNKNOWN, rstr.remote_device());
}

TEST(RawStringTest, AssignTest_leftValueString) {
  RawString rstr;
  CHECK_EMPTY(rstr);

  rstr.set_device(RawString::CPU);
  rstr.set_remote_device(RawString::GPU);
  EXPECT_EQ(RawString::CPU, rstr.device());
  EXPECT_EQ(RawString::GPU, rstr.remote_device()); 

  string str("hello");
  string str2 = str;
  rstr.assign(move(str));
  EXPECT_TRUE(str.empty());
  str = str2;
  EXPECT_EQ(5, rstr.size());
  EXPECT_TRUE(rstr.data() != nullptr);
  EXPECT_TRUE(rstr.shared_data() != nullptr);
  EXPECT_EQ(0, strncmp(rstr.data(), str.c_str(), rstr.size()));
  EXPECT_EQ(RawString::UNKNOWN, rstr.device());
  EXPECT_EQ(RawString::UNKNOWN, rstr.remote_device());
}

TEST(RawStringTest, AssignTest_constCharPointer) {
  RawString rstr;
  CHECK_EMPTY(rstr);

  rstr.set_device(RawString::CPU);
  rstr.set_remote_device(RawString::GPU);
  EXPECT_EQ(RawString::CPU, rstr.device());
  EXPECT_EQ(RawString::GPU, rstr.remote_device()); 

  string str("hello");
  rstr.assign(str.c_str());
  EXPECT_EQ(5, rstr.size());
  EXPECT_TRUE(rstr.data() != nullptr);
  EXPECT_TRUE(rstr.shared_data() != nullptr);
  EXPECT_EQ(0, strncmp(rstr.data(), str.c_str(), rstr.size()));
  EXPECT_EQ(RawString::UNKNOWN, rstr.device());
  EXPECT_EQ(RawString::UNKNOWN, rstr.remote_device());
}

TEST(RawStringTest, AssignTest_constCharPointerAndSize) {
  RawString rstr;
  CHECK_EMPTY(rstr);

  rstr.set_device(RawString::CPU);
  rstr.set_remote_device(RawString::GPU);
  EXPECT_EQ(RawString::CPU, rstr.device());
  EXPECT_EQ(RawString::GPU, rstr.remote_device()); 

  string str("hello");
  rstr.assign(str.c_str(), str.size());
  EXPECT_EQ(5, rstr.size());
  EXPECT_TRUE(rstr.data() != nullptr);
  EXPECT_TRUE(rstr.shared_data() != nullptr);
  EXPECT_EQ(0, strncmp(rstr.data(), str.c_str(), rstr.size()));
  EXPECT_EQ(RawString::UNKNOWN, rstr.device());
  EXPECT_EQ(RawString::UNKNOWN, rstr.remote_device());
}

TEST(RawStringTest, AssignTest_sharedPtr) {
  RawString rstr;
  CHECK_EMPTY(rstr);

  rstr.set_device(RawString::CPU);
  rstr.set_remote_device(RawString::GPU);
  EXPECT_EQ(RawString::CPU, rstr.device());
  EXPECT_EQ(RawString::GPU, rstr.remote_device()); 

  string str("hello");
  shared_ptr<const char> shared_str(str.c_str(), [](const char *) {});

  rstr.assign(shared_str, str.size());
  EXPECT_EQ(5, rstr.size());
  EXPECT_TRUE(rstr.data() != nullptr);
  EXPECT_TRUE(rstr.shared_data() != nullptr);
  EXPECT_EQ(0, strncmp(rstr.data(), str.c_str(), rstr.size()));
  EXPECT_EQ(RawString::UNKNOWN, rstr.device());
  EXPECT_EQ(RawString::UNKNOWN, rstr.remote_device());
}

TEST(RawStringTest, AssignTest_rawString) {
  string str("hello");
  shared_ptr<const char> shared_str(str.c_str(), [](const char *) {});

  RawString rstr1(shared_str, str.size());
  rstr1.set_device(RawString::CPU);
  rstr1.set_remote_device(RawString::GPU);

  EXPECT_EQ(5, rstr1.size());
  EXPECT_EQ(str.c_str(), rstr1.data());
  EXPECT_EQ(shared_str, rstr1.shared_data());
  EXPECT_EQ(RawString::CPU, rstr1.device());
  EXPECT_EQ(RawString::GPU, rstr1.remote_device());

  RawString rstr2;
  CHECK_EMPTY(rstr2);

  rstr2.assign(rstr1);
  EXPECT_EQ(5, rstr1.size());
  EXPECT_EQ(str.c_str(), rstr1.data());
  EXPECT_EQ(shared_str, rstr1.shared_data());
  EXPECT_EQ(RawString::CPU, rstr1.device());
  EXPECT_EQ(RawString::GPU, rstr1.remote_device());

  EXPECT_EQ(5, rstr2.size());
  EXPECT_EQ(str.c_str(), rstr2.data());
  EXPECT_EQ(shared_str, rstr2.shared_data());
  EXPECT_EQ(RawString::CPU, rstr2.device());
  EXPECT_EQ(RawString::GPU, rstr2.remote_device());
}

TEST(RawStringTest, SwapTest_empty2Empty) {
  RawString rstr1;
  CHECK_EMPTY(rstr1);
  
  RawString rstr2;
  CHECK_EMPTY(rstr2);

  rstr1.swap(rstr2);

  CHECK_EMPTY(rstr1);
  CHECK_EMPTY(rstr2);
}

TEST(RawStringTest, SwapTest_empty2NoEmpty) {
  string str1("hello");
  shared_ptr<const char> shared_str1(str1.c_str(), [](const char*) {});
  RawString rstr1(shared_str1, str1.size());
  rstr1.set_device(RawString::CPU);
  rstr1.set_remote_device(RawString::GPU);

  EXPECT_EQ(rstr1.size(), 5);
  EXPECT_EQ(str1.c_str(), rstr1.data());
  EXPECT_EQ(shared_str1, rstr1.shared_data());
  EXPECT_EQ(RawString::CPU, rstr1.device());
  EXPECT_EQ(RawString::GPU, rstr1.remote_device());
  
  RawString rstr2;
  CHECK_EMPTY(rstr2);

  rstr1.swap(rstr2);

  CHECK_EMPTY(rstr1);

  EXPECT_EQ(rstr2.size(), 5);
  EXPECT_EQ(str1.c_str(), rstr2.data());
  EXPECT_EQ(shared_str1, rstr2.shared_data());
  EXPECT_EQ(RawString::CPU, rstr2.device());
  EXPECT_EQ(RawString::GPU, rstr2.remote_device());
}

TEST(RawStringTest, SwapTest_noEmpty2NoEmpty) {
  string str1("hello");
  shared_ptr<const char> shared_str1(str1.c_str(), [](const char*) {});
  RawString rstr1(shared_str1, str1.size());
  rstr1.set_device(RawString::CPU);
  rstr1.set_remote_device(RawString::GPU);

  EXPECT_EQ(rstr1.size(), 5);
  EXPECT_EQ(str1.c_str(), rstr1.data());
  EXPECT_EQ(shared_str1, rstr1.shared_data());
  EXPECT_EQ(RawString::CPU, rstr1.device());
  EXPECT_EQ(RawString::GPU, rstr1.remote_device());
  
  string str2("world");
  shared_ptr<const char> shared_str2(str2.c_str(), [](const char*) {});
  RawString rstr2(shared_str2, str2.size());
  rstr2.set_device(RawString::GPU);
  rstr2.set_remote_device(RawString::CPU);

  EXPECT_EQ(rstr2.size(), 5);
  EXPECT_EQ(str2.c_str(), rstr2.data());
  EXPECT_EQ(shared_str2, rstr2.shared_data());
  EXPECT_EQ(RawString::GPU, rstr2.device());
  EXPECT_EQ(RawString::CPU, rstr2.remote_device());

  rstr1.swap(rstr2);

  EXPECT_EQ(rstr1.size(), 5);
  EXPECT_EQ(str2.c_str(), rstr1.data());
  EXPECT_EQ(shared_str2, rstr1.shared_data());
  EXPECT_EQ(RawString::GPU, rstr1.device());
  EXPECT_EQ(RawString::CPU, rstr1.remote_device());

  EXPECT_EQ(rstr2.size(), 5);
  EXPECT_EQ(str1.c_str(), rstr2.data());
  EXPECT_EQ(shared_str1, rstr2.shared_data());
  EXPECT_EQ(RawString::CPU, rstr2.device());
  EXPECT_EQ(RawString::GPU, rstr2.remote_device());
}

TEST(RawStringTest, ClearTest) {
  string str("hello");
  shared_ptr<const char> shared_str(str.c_str(), [](const char*) {});
  RawString rstr(shared_str, str.size());
  rstr.set_device(RawString::CPU);
  rstr.set_remote_device(RawString::GPU);

  EXPECT_EQ(rstr.size(), 5);
  EXPECT_EQ(str.c_str(), rstr.data());
  EXPECT_EQ(shared_str, rstr.shared_data());
  EXPECT_EQ(RawString::CPU, rstr.device());
  EXPECT_EQ(RawString::GPU, rstr.remote_device());

  rstr.clear();
  CHECK_EMPTY(rstr);
}

TEST(RawStringTest, OperatorEqualTest_defaultEqual) {
  RawString rstr1;
  RawString rstr2;
  EXPECT_EQ(rstr1, rstr2);
}

TEST(RawStringTest, OperatorEqualTest_valueEqual) {
  string str("hello");
  shared_ptr<const char> shared_str(str.c_str(), [](const char*) {});
  
  RawString rstr1(shared_str, str.size());
  rstr1.set_device(RawString::CPU);
  rstr1.set_remote_device(RawString::GPU);

  RawString rstr2(shared_str, str.size());
  rstr2.set_device(RawString::CPU);
  rstr2.set_remote_device(RawString::GPU);

  EXPECT_TRUE(rstr1 == rstr2);
}

TEST(RawStringTest, OperatorEqualTest_dataNotEq) {
  string str1("hello");
  shared_ptr<const char> shared_str1(str1.c_str(), [](const char*) {});

  string str2("world");
  shared_ptr<const char> shared_str2(str2.c_str(), [](const char*) {});
  
  RawString rstr1(shared_str1, str1.size());
  rstr1.set_device(RawString::CPU);
  rstr1.set_remote_device(RawString::GPU);

  RawString rstr2(shared_str2, str2.size());
  rstr2.set_device(RawString::CPU);
  rstr2.set_remote_device(RawString::GPU);

  EXPECT_FALSE(rstr1 == rstr2);
}

TEST(RawStringTest, OperatorEqualTest_sizeNotEq) {
  string str("hello");
  shared_ptr<const char> shared_str(str.c_str(), [](const char*) {});
  
  RawString rstr1(shared_str, str.size());
  rstr1.set_device(RawString::CPU);
  rstr1.set_remote_device(RawString::GPU);

  RawString rstr2(shared_str, str.size() - 1);
  rstr2.set_device(RawString::CPU);
  rstr2.set_remote_device(RawString::GPU);

  EXPECT_FALSE(rstr1 == rstr2);
}

TEST(RawStringTest, OperatorEqualTest_deviceNotEq) {
  string str("hello");
  shared_ptr<const char> shared_str(str.c_str(), [](const char*) {});
  
  RawString rstr1(shared_str, str.size());
  rstr1.set_device(RawString::CPU);
  rstr1.set_remote_device(RawString::GPU);

  RawString rstr2(shared_str, str.size());
  rstr2.set_device(RawString::GPU);
  rstr2.set_remote_device(RawString::GPU);

  EXPECT_FALSE(rstr1 == rstr2);
}

TEST(RawStringTest, OperatorEqualTest_remoteDeviceNotEq) {
  string str("hello");
  shared_ptr<const char> shared_str(str.c_str(), [](const char*) {});
  
  RawString rstr1(shared_str, str.size());
  rstr1.set_device(RawString::CPU);
  rstr1.set_remote_device(RawString::GPU);

  RawString rstr2(shared_str, str.size());
  rstr2.set_device(RawString::CPU);
  rstr2.set_remote_device(RawString::CPU);

  EXPECT_FALSE(rstr1 == rstr2);
}

TEST(RawStringTest, DeviceOpsTest) {
  RawString rstr;
  EXPECT_EQ(RawString::UNKNOWN, rstr.device());
  EXPECT_EQ(RawString::UNKNOWN, rstr.remote_device());

  rstr.set_device(RawString::CPU);
  EXPECT_EQ(RawString::CPU, rstr.device());
  EXPECT_EQ(RawString::UNKNOWN, rstr.remote_device());

  rstr.set_remote_device(RawString::CPU);
  EXPECT_EQ(RawString::CPU, rstr.device());
  EXPECT_EQ(RawString::CPU, rstr.remote_device());
}

TEST(RawStringTest, SubstrTest_empty) {
  RawString rstr;
  CHECK_EMPTY(rstr);

  EXPECT_EQ(std::string(), rstr.substr(0, 0));
  EXPECT_EQ(std::string(), rstr.substr(0, 1));
  EXPECT_EQ(std::string(), rstr.substr(1, 2));
}

TEST(RawStringTest, SubstrTest_default) {
  RawString rstr("hello world");

  EXPECT_EQ(std::string(""), rstr.substr(0, 0));
  EXPECT_EQ(std::string("hello world"), rstr.substr(0, 11));
  EXPECT_EQ(std::string(""), rstr.substr(0, 12));
  EXPECT_EQ(std::string("hello"), rstr.substr(0, 5));
  EXPECT_EQ(std::string("world"), rstr.substr(6, 5));
  EXPECT_EQ(std::string(""), rstr.substr(6, 6));
}

TEST(RawStringTest, ToStringTest_empty) {
  RawString rstr;

  EXPECT_EQ(std::string(""), rstr.to_string());

  rstr.assign("hello");
  EXPECT_EQ(std::string("hello"), rstr.to_string());
}

}  // namespace std
