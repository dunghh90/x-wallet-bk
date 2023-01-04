pub fn enum_option_study() {
  let _x = Some(123);
  let _y = plus_one(_x);
  println!("x: {:?}", _x);
  println!("y: {:?}", _y);

  let _z = plus_two(6);
  println!("z: {:?}", _z);

}

fn plus_one(x: Option<i32>) -> Option<i32> {
  match x {
    Some(x) => Some(x + 1),
    _ => None
  } 
}


fn plus_two(y: i32) -> i32 {
  match y {
    5 => 5,
    _ => 0
  } 
}
