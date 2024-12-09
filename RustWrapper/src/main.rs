pub mod cpp_dll_wrapper;

use core::slice;
use std::ptr;

use cpp_dll_wrapper::cpp_dll_api::*;

extern "C" fn call_back(prev: DataType, next: DataType) {
    println!(">>> DataType Changed! {} -> {}", prev as i32, next as i32);
}

fn main() {
    let mut init = InitParam {
        size: size_of::<InitParam>(),
        int_32: i32::MIN / 2,
        int_16: i16::MIN / 2,
        int_8: i8::MIN / 2,
        uint_32: u32::MAX / 2,
        uint_16: u16::MAX / 2,
        uint_8: u8::MAX / 2,
        float_32: f32::MAX / 2.0,
        float_64: f64::MAX / 2.0,
        string: [0; 64],
    };
    let str = ("StringText Max64").as_bytes();
    let len = str.len().min(init.string.len());
    init.string[..len].copy_from_slice(&str[..len]);

    unsafe {
        HelloWorld();
        println!("Initialize");
        Initialize(&init);
        RegisterCallback(call_back);
        SetDataType(DataType::DataType_1D);
        println!();
        println!("GetData");
        let mut d1 = Data1D {
            size: size_of::<Data1D>(),
            data_type: DataType::DataType_1D,
            x: 0,
            data: ptr::null_mut(),
        };
        GetData(&mut d1 as *mut _ as *mut DataBase);
        PrintData(&mut d1 as *mut _ as *mut DataBase);
        println!();
        println!("GetDataArray");
        let mut d_array: *mut *const DataBase = ptr::null_mut();
        let mut size: i32 = 0;
        GetDataArray(&mut d_array, &mut size);
        let size = size as usize;
        println!();
        println!("size : {}", size);
        let d_array: &[*const DataBase] = slice::from_raw_parts(d_array, size);

        for (i, &d) in d_array.iter().enumerate() {
            println!();
            println!(">> {}", i);
            PrintData(d);
        }

        SetDataType(DataType::DataType_2D);
        println!();
        println!("GetData");
        let mut d2 = Data2D {
            size: size_of::<Data2D>(),
            data_type: DataType::DataType_2D,
            x: 0,
            y: 0,
            data: ptr::null_mut(),
        };
        GetData(&mut d2 as *mut _ as *mut DataBase);
        PrintData(&mut d2 as *mut _ as *mut DataBase);
        println!();
        println!("GetDataArray");
        let mut d_array: *mut *const DataBase = ptr::null_mut();
        let mut size: i32 = 0;
        GetDataArray(&mut d_array, &mut size);
        let size = size as usize;
        println!();
        println!("size : {}", size);
        let d_array: &[*const DataBase] = slice::from_raw_parts(d_array, size);
        for (i, &d) in d_array.iter().enumerate() {
            println!();
            println!(">> {}", i);
            PrintData(d);
        }

        println!("Read Unaligned");

        for &d in d_array.iter() {
            // unsafe cast
            let aa = (d as *const Data1D).read_unaligned();
            let s = aa.size;
            let t = aa.data_type as u32;
            let x = aa.x;
            // let y = aa.y;
            let data = aa.data;
            println!("size={}", s);
            println!("type={}", t);
            println!("x={}", x);
            // println!("{}", y);
            println!("data={:?}", data);
        }
    }
}
