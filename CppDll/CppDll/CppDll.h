#pragma once

#include <iostream>

#pragma pack(4)

extern "C" {

enum CppResult {
  CppResult_Success,
  CppResult_UnknownError = 0x80000000,
  CppResult_InvalidParameter,
  CppResult_NotImplemented,
};

enum DataType {
  DataType_1D,
  DataType_2D,
};

struct InitParam {
  size_t size;
  int32_t int_32;
  int16_t int_16;
  int8_t int_8;
  uint32_t uint_32;
  uint16_t uint_16;
  uint8_t uint_8;
  float float_32;
  double float_64;
  char string[64];
};

struct DataBase {
  size_t size;
  DataType type;
};

struct Data1D {
  size_t size;
  DataType type;
  int32_t x;
  uint8_t* data;
};

struct Data2D {
  size_t size;
  DataType type;
  int32_t x;
  int32_t y;
  uint8_t* data;
};

typedef void (*OnDataTypeChanged)(const DataType prev, const DataType next);

void HelloWorld();
CppResult Initialize(const InitParam* param);
CppResult SetDataType(const DataType type);
CppResult GetData(DataBase* data);
CppResult GetDataArray(DataBase*** data, int32_t* size);
CppResult PrintData(const DataBase* data);
CppResult RegisterCallback(OnDataTypeChanged callback);
CppResult UnRegisterCallback(OnDataTypeChanged callback);

} // extern "C"
#pragma pack()
