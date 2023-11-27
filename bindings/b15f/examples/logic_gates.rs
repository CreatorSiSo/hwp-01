use b15f::{B15fDriver, B15fStud, Driver};

fn main() {
	let mut driver = B15fDriver::new();
	// let mut driver = B15fStud::new();

    loop {
        let dip_switch = driver.read_dip_switch();
        driver.digital_write_0(dip_switch);
    }
}
