use std::env;
use std::path::PathBuf;

fn main() {
	println!("cargo:rustc-link-search=/usr/lib/");

	println!("cargo:rustc-link-lib=b15fdrv");
	// Tell cargo to invalidate the built crate whenever the wrapper changes
	println!("cargo:rerun-if-changed=wrapper.h");

	let bindings = bindgen::Builder::default()
		.header("wrapper.hpp")
		.clang_arg("-x") // -x c++
		.clang_arg("c++")
		.clang_arg("-std=c++14")
		.enable_cxx_namespaces()
		.allowlist_type("B15F")
		.blocklist_file("usart.h")
		.opaque_type("std::.*")
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
