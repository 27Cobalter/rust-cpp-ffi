use std::{env, fs, path::PathBuf};

enum TargetOs {
    Windows,
    Linux,
}

fn main() {
    // Check target os
    let target_os: TargetOs;
    if cfg!(target_os = "windows") {
        target_os = TargetOs::Windows;
    } else if cfg!(target_os = "linux") {
        target_os = TargetOs::Linux;
    } else {
        unimplemented!();
    }

    // Get Directory
    let profile = env::var("PROFILE").unwrap();
    let project_dir = env::var("CARGO_MANIFEST_DIR").unwrap();
    let cmake_dir = PathBuf::from(&project_dir)
        .join("..")
        .join("build")
        .join(format!("x64-{}", &profile));

    let link_lib = "CppDll";

    // Link library
    println!(
        "cargo:rustc-link-search=native={}/lib/",
        cmake_dir.to_str().unwrap()
    );
    println!("cargo:rustc-link-lib=dylib={}", &link_lib);

    // Copy Dll to Rust Binary Dir
    let (lib_pref, lib_suff) = match target_os {
        TargetOs::Windows => ("", "dll"),
        TargetOs::Linux => ("lib", "so"),
    };

    let dll_name = format!("{}{}.{}", &lib_pref, &link_lib, &lib_suff);
    let dll_src = PathBuf::from(&cmake_dir).join("bin").join(&dll_name);
    let dll_dst = PathBuf::from("target").join(&profile).join(&dll_name);
    println!(
        "cargo:warning=Copy Dll {} -> {}",
        dll_src.to_str().unwrap(),
        dll_dst.to_str().unwrap()
    );
    fs::copy(dll_src, dll_dst).expect("Failed to copy dll.");
}
