use b15f::B15fDriver;

fn main() {
	let mut driver = B15fDriver::new();

	loop {
		let dip_switch = driver.read_dip_switch();
		driver.digital_write_0(dip_switch);
	}
}
