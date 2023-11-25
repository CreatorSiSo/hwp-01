use b15f_sys::root as binding;
use std::sync::RwLock;

pub trait Driver {
	fn new() -> Self;

	fn read_dip_switch(&self) -> u8;
	fn digital_read_0(&self) -> u8;
	fn digital_read_1(&self) -> u8;
	fn digital_write_0(&mut self, value: u8);
	fn digital_write_1(&mut self, value: u8);
}

pub struct B15fDriver {
	inner: RwLock<*mut binding::B15F>,
}

impl Driver for B15fDriver {
	/// ## Exeptions
	/// Might crash because of exeptions from the C++ side.
	fn new() -> Self {
		Self {
			// TODO Handle exeptions, prob needs C++ wrapper
			// SAFETY: Inside RwLock, ...
			inner: RwLock::new(unsafe { binding::B15F_getInstance() }),
		}
	}

	fn read_dip_switch(&self) -> u8 {
		unsafe { binding::B15F_readDipSwitch(*self.inner.read().unwrap()) }
	}

	fn digital_read_0(&self) -> u8 {
		unsafe { binding::B15F_digitalRead0(*self.inner.read().unwrap()) }
	}
	fn digital_read_1(&self) -> u8 {
		unsafe { binding::B15F_digitalRead1(*self.inner.read().unwrap()) }
	}
	fn digital_write_0(&mut self, value: u8) {
		unsafe { binding::B15F_digitalWrite0(*self.inner.read().unwrap(), value) }
	}
	fn digital_write_1(&mut self, value: u8) {
		unsafe { binding::B15F_digitalWrite1(*self.inner.read().unwrap(), value) }
	}
}

pub struct B15fStud {
	dip_switch: u8,
	in0: u8,
	in1: u8,
	out0: u8,
	out1: u8,
}

impl Driver for B15fStud {
	fn new() -> Self {
		Self {
			dip_switch: 1,
			in0: 0,
			in1: 0,
			out0: 0,
			out1: 0,
		}
	}

	fn read_dip_switch(&self) -> u8 {
		self.dip_switch
	}

	fn digital_read_0(&self) -> u8 {
		self.in0
	}
	fn digital_read_1(&self) -> u8 {
		self.in1
	}
	fn digital_write_0(&mut self, value: u8) {
		println!("{value:08b}");
		self.out0 = value;
	}
	fn digital_write_1(&mut self, value: u8) {
		println!("{value:08b}");
		self.out1 = value;
	}
}
