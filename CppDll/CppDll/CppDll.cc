#include "CppDll.h"

#include <cassert>
#include <memory>
#include <print>
#include <ranges>
#include <vector>

DataType data_type = DataType_1D;
std::vector<std::unique_ptr<uint8_t[]>> data_buffer;
std::vector<DataBase*> base_buffer;
std::vector<Data1D> data1d_buffer;
std::vector<Data2D> data2d_buffer;

void HelloWorld() {
  std::println("HelloWorld");
}

CppResult Initialize(const InitParam* param) {
  if (param->size != sizeof(InitParam)) {
    assert(false);
    return CppResult::CppResult_InvalidParameter;
  }

  std::println("{:16s}: {}", "size", param->size);
  std::println("{:16s}: {}", "int32_t", param->int_32);
  std::println("{:16s}: {}", "int16_t", param->int_16);
  std::println("{:16s}: {}", "int8_t", param->int_8);
  std::println("{:16s}: {}", "uint32_t", param->uint_32);
  std::println("{:16s}: {}", "uint16_t", param->uint_16);
  std::println("{:16s}: {}", "uint8_t", param->uint_8);
  std::println("{:16s}: {}", "float", param->float_32);
  std::println("{:16s}: {}", "double", param->float_64);
  std::println("{:16s}: {}", "char[]", param->string);

  return CppResult::CppResult_Success;
}

CppResult SetDataType(const DataType type) {
  data_type = type;

  return CppResult::CppResult_Success;
}

CppResult GetData(DataBase* data) {
  data_buffer.clear();

  constexpr int32_t data_size = 1024;

  if (data->type == DataType_1D) {
    if (data->size != sizeof(Data1D)) {
      assert(false);
      return CppResult::CppResult_InvalidParameter;
    }

    Data1D* data1 = reinterpret_cast<Data1D*>(data);
    auto buffer   = std::make_unique<uint8_t[]>(data_size);
    for (auto i : std::views::iota(0, data_size)) {
      buffer[i] = i;
    }

    data1->size = sizeof(Data1D);
    data1->type = DataType::DataType_1D;
    data1->x    = data_size;
    data1->data = buffer.get();
    data_buffer.emplace_back(std::move(buffer));
  } else if (data->type == DataType_2D) {
    if (data->size != sizeof(Data2D)) {
      assert(false);
      return CppResult::CppResult_InvalidParameter;
    }

    Data2D* data2 = reinterpret_cast<Data2D*>(data);
    auto buffer   = std::make_unique<uint8_t[]>(data_size * data_size);
    for (auto i : std::views::iota(0, data_size * data_size)) {
      buffer[i] = i;
    }

    data2->size = sizeof(Data2D);
    data2->type = DataType::DataType_2D;
    data2->x    = data_size;
    data2->y    = data_size;
    data2->data = buffer.get();
    data_buffer.emplace_back(std::move(buffer));
  }

  return CppResult::CppResult_Success;
}

CppResult GetDataArray(DataBase*** data, int32_t* size) {
  data_buffer.clear();
  base_buffer.clear();
  data1d_buffer.clear();
  data2d_buffer.clear();
  *size = 0;

  constexpr int32_t array_size = 4;
  constexpr int32_t data_size  = 512;

  if (data_type == DataType_1D) {
    data1d_buffer.resize(array_size);
    base_buffer.resize(array_size);

    for (auto iter : std::views::iota(0, array_size)) {
      auto buffer = std::make_unique<uint8_t[]>(data_size);
      for (auto i : std::views::iota(0, data_size)) {
        buffer[i] = i + iter;
      }

      data1d_buffer[iter].size = sizeof(Data1D);
      data1d_buffer[iter].type = DataType::DataType_1D;
      data1d_buffer[iter].x    = data_size;
      data1d_buffer[iter].data = buffer.get();
      data_buffer.emplace_back(std::move(buffer));

      base_buffer[iter] = reinterpret_cast<DataBase*>(&data1d_buffer[iter]);
    }
  } else if (data_type == DataType_2D) {
    data2d_buffer.resize(array_size);
    base_buffer.resize(array_size);

    for (auto iter : std::views::iota(0, array_size)) {
      auto buffer = std::make_unique<uint8_t[]>(data_size * data_size);
      for (auto i : std::views::iota(0, data_size * data_size)) {
        buffer[i] = i + iter;
      }

      data2d_buffer[iter].size = sizeof(Data2D);
      data2d_buffer[iter].type = DataType::DataType_2D;
      data2d_buffer[iter].x    = data_size;
      data2d_buffer[iter].y    = data_size;
      data2d_buffer[iter].data = buffer.get();
      data_buffer.emplace_back(std::move(buffer));

      base_buffer[iter] = reinterpret_cast<DataBase*>(&data2d_buffer[iter]);
    }
  }

  *size = array_size;
  *data = base_buffer.data();
  return CppResult::CppResult_Success;
}

CppResult PrintData(const DataBase* data) {
  if (data->size == sizeof(DataBase)) {
    std::println("{:16s}: {}", "size", data->size);
    std::println("{:16s}: {}", "type", static_cast<int32_t>(data->type));
  } else if (data->size == sizeof(Data1D)) {
    auto data1 = reinterpret_cast<const Data1D*>(data);
    std::println("{:16s}: {}", "size", data1->size);
    std::println("{:16s}: {}", "type", static_cast<int32_t>(data1->type));
    std::println("{:16s}: {}", "x", data1->x);
    std::println("{:16s}: [", "data");
    for (auto i : std::views::iota(0, 8)) {
      std::print("{:5d} ", data1->data[i]);
    }
    std::println("...");
    std::println("]");
  } else if (data->size == sizeof(Data2D)) {
    auto data2 = reinterpret_cast<const Data2D*>(data);
    std::println("{:16s}: {}", "size", data2->size);
    std::println("{:16s}: {}", "type", static_cast<int32_t>(data2->type));
    std::println("{:16s}: {}", "x", data2->x);
    std::println("{:16s}: {}", "y", data2->y);
    std::println("{:16s}: [", "data");
    for (auto j : std::views::iota(0, 8)) {
      for (auto i : std::views::iota(0, 8)) {
        std::print("{:5d} ", data2->data[i] + (j * data2->x));
      }
      std::println("...");
    }
    std::println("]");
  }

  return CppResult::CppResult_Success;
}
