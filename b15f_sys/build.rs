use std::env;
use std::path::PathBuf;

fn main() {
	// Tell cargo to invalidate the built crate whenever the wrapper changes
	println!("cargo:rerun-if-changed=./src/wrapper.hpp");
	println!("cargo:rerun-if-changed=./src/wrapper.cpp");

	// Compile and link static C++ wrapper
	cc::Build::new()
		.cpp(true)
		.files(["./src/wrapper.hpp", "./src/wrapper.cpp"])
		.compile("wrapper");
	println!("cargo:rustc-link-search=static=libwrapper.a");

	// Dynamically link libb15drv.so
	println!("cargo:rustc-link-search=/usr/lib/");
	println!("cargo:rustc-link-lib=b15fdrv");

	let bindings = bindgen::Builder::default()
		.header("./src/wrapper.hpp")
		.clang_args(["-x", "c++", "-std=c++14"])
		.enable_cxx_namespaces()
		.allowlist_type("B15F")
		.blocklist_function("B15F_exec")
		.blocklist_function("B15F_delay_.*")
		.opaque_type("USART")
		.blocklist_function("USART.*")
		.opaque_type("std::.*")
		.allowlist_function("tryGetInstance")
		// Tell cargo to invalidate the built crate whenever any of the
		// included header files changed.
		.parse_callbacks(Box::new(bindgen::CargoCallbacks::new()))
		.generate()
		.expect("Unable to generate bindings");

	// Write the bindings to the $OUT_DIR/bindings.rs file.
	let out_path = PathBuf::from(env::var("OUT_DIR").unwrap());
	bindings
		.write_to_file(out_path.join("bindings.rs"))
		.expect("Couldn't write bindings!");
}
