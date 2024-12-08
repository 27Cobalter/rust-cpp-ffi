#include <numeric>
#include <print>
#include <ranges>

#include <CppDll.h>

void Callback(const DataType prev, const DataType next){
  std::println(">>> DataType Changed! {} -> {}", static_cast<int32_t>(prev), static_cast<int32_t>(next));
}

auto main() -> int {
  InitParam init{.size     = sizeof(InitParam),
                 .int_32   = std::numeric_limits<int32_t>::min() / 2,
                 .int_16   = std::numeric_limits<int16_t>::min() / 2,
                 .int_8    = std::numeric_limits<int8_t>::min() / 2,
                 .uint_32  = std::numeric_limits<uint32_t>::max() / 2,
                 .uint_16  = std::numeric_limits<uint16_t>::max() / 2,
                 .uint_8   = std::numeric_limits<uint8_t>::max() / 2,
                 .float_32 = std::numeric_limits<float>::max() / 2,
                 .float_64 = std::numeric_limits<double>::max() / 2,
                 .string   = "StringText Max64"};

  HelloWorld();
  std::println("Initialize");
  auto ret  = Initialize(&init);
  RegisterCallback(Callback);
  ret       = SetDataType(DataType::DataType_1D);
  std::println("");
  std::println("GetData");
  Data1D d1 = {.size = sizeof(Data1D), .type = DataType::DataType_1D};
  ret       = GetData(reinterpret_cast<DataBase*>(&d1));
  ret       = PrintData(reinterpret_cast<DataBase*>(&d1));
  std::println("");
  std::println("GetDataArray");
  DataBase** d_array = nullptr;
  int32_t size = 0;
  ret = GetDataArray(&d_array, &size);
  std::println("");
  std::println("size : {}", size);
  for(auto i : std::views::iota(0, size))
  {
    std::println("");
    std::println(">> {}", i);
    PrintData(d_array[i]);
  }

  ret       = SetDataType(DataType::DataType_2D);
  std::println("");
  std::println("GetData");
  Data2D d2 = {.size = sizeof(Data2D), .type = DataType::DataType_2D};
  ret       = GetData(reinterpret_cast<DataBase*>(&d2));
  ret       = PrintData(reinterpret_cast<DataBase*>(&d2));
  std::println("");
  std::println("GetDataArray");
  d_array = nullptr;
  size = 0;
  ret = GetDataArray(&d_array, &size);
  for(auto i : std::views::iota(0, size))
  {
  std::println("");
    std::println(">> {}", i);
    PrintData(d_array[i]);
  }
}
