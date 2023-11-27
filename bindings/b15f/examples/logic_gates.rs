use std::{
	ops::{BitOrAssign, ShlAssign},
	thread::sleep,
	time::Duration,
};

use b15f::B15fDriver;

fn main() {
	let mut driver = B15fDriver::new();
	let truth_table: Vec<(u8, u8)> = bool_combinations(8)
		.into_iter()
		.map(|input| {
			driver.digital_write_0(input);
			sleep(Duration::from_millis(5));
			(input, driver.digital_read_1())
		})
		.collect();

	println!("   Input | Output");
	println!("---------|---------");
	for (input, output) in truth_table {
		println!("{input:08b} | {output:08b}");
	}
}

fn bool_combinations(num_inputs: u8) -> Vec<u8> {
	let num_combinations = usize::pow(2, num_inputs as u32);
	let mut combinations = Vec::with_capacity(num_combinations);
	combinations.extend_from_slice(&[0, 1]);

	(1..num_inputs).for_each(|_| {
		let second_half = combinations
			.clone()
			.into_iter()
			.map(|combination| combination << 1);
		combinations.iter_mut().for_each(|combination| {
			combination.shl_assign(1);
			combination.bitor_assign(1);
		});
		combinations.extend(second_half);
	});

	combinations
}

#[test]
fn test_bool_combinations() {
	assert_eq!(bool_combinations(1), vec![0, 1]);
	assert_eq!(bool_combinations(2), vec![01, 01, 10, 00]);
	assert_eq!(
		bool_combinations(2),
		vec![011, 011, 101, 001, 010, 010, 100, 000]
	);

	print!(
		"{}",
		bool_combinations(4)
			.iter()
			.map(|combination| format!("{:08b}\n", combination))
			.fold(String::new(), |accum, combination| accum + &combination)
	);
}
