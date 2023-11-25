use b15f::{B15fDriver, B15fStud, Driver};
use std::time::Duration;

fn main() {
	// let driver = B15fDriver::new();
	let mut driver = B15fStud::new();

	let mut knight_rider = (0..).map(|i: usize| {
		const NUM_LEDS: usize = 8;
		const RIGHT: u8 = 0b00000001;
		const LEFT: u8 = 0b10000000;

		let shift = (i % NUM_LEDS) as u8;
		if ((i / NUM_LEDS) % 2) == 0 {
			RIGHT << shift
		} else {
			LEFT >> shift
		}
	});

	loop {
		std::thread::sleep(Duration::from_millis(50));

		let dip_switch = driver.read_dip_switch();

		match Mode::from_dip_switch(dip_switch) {
			Mode::Inverse => println!("Test"),
			Mode::KnightRider => {
				if let Some(leds) = knight_rider.next() {
					driver.digital_write_0(leds);
					driver.digital_write_1(!leds);
				}
			}
		}
	}
}

#[derive(Debug, PartialEq, Eq)]
enum Mode {
	Inverse,
	KnightRider,
}

impl Mode {
	fn from_dip_switch(dip_switch: u8) -> Self {
		match dip_switch {
			0 => Mode::Inverse,
			_ => Mode::KnightRider,
		}
	}
}
