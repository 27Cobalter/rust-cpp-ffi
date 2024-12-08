#![allow(non_snake_case)]
#![allow(non_camel_case_types)]

use std::ffi;

#[repr(u32)]
pub enum CppResult {
    CppResult_Success,
    CppResult_UnknownError = 0x80000000,
    CppResult_InvalidParameter,
    CppResult_NotImplemented,
}

#[repr(u32)]
pub enum DataType {
    DataType_1D,
    DataType_2D,
}

#[repr(C, packed(4))]
pub struct InitParam {
    pub size: usize,
    pub int_32: i32,
    pub int_16: i16,
    pub int_8: i8,
    pub uint_32: u32,
    pub uint_16: u16,
    pub uint_8: u8,
    pub float_32: f32,
    pub float_64: f64,
    pub string: [ffi::c_uchar; 64],
}

#[repr(C, packed(4))]
pub struct DataBase {
    pub size: usize,
    pub data_type: DataType,
}

#[repr(C, packed(4))]
pub struct Data1D {
    pub size: usize,
    pub data_type: DataType,
    pub x: i32,
    pub data: *const u8,
}

#[repr(C, packed(4))]
pub struct Data2D {
    pub size: usize,
    pub data_type: DataType,
    pub x: i32,
    pub y: i32,
    pub data: *const u8,
}

pub type OnDataTypeChanged = extern "C" fn(prev: DataType, next: DataType);

extern "C" {
    pub fn HelloWorld();
    pub fn Initialize(param: *const InitParam) -> CppResult;
    pub fn SetDataType(data_type: DataType) -> CppResult;
    pub fn GetData(data: *mut DataBase) -> CppResult;
    pub fn GetDataArray(data: *mut *mut *const DataBase, size: *mut i32) -> CppResult;
    pub fn PrintData(data: *const DataBase) -> CppResult;
    pub fn RegisterCallback(callback: OnDataTypeChanged);
    pub fn UnRegisterCallback(callback: OnDataTypeChanged);
} // extern "C"
