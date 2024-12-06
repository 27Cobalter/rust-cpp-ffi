extern "C" {
    fn HelloWorld();
}

fn main() {
    unsafe {
        HelloWorld();
    }
}
