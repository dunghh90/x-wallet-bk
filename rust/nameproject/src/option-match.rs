#[derive(Debug)]
// enum IpAddressKind {
//     V4(String),
//     V6(String)
// }

enum IpAddressKind {
    V4(u8, u8, u8, u8),
    V6(String)
}

#[derive(Debug)]
struct _IpAdress {
    kind: IpAddressKind,
    address: String,
}

// ========== enum =========
// fn main() {

//     // let localhost = IpAddressKind::V4(String::from("127.0.0.2"));
//     let localhost = IpAddressKind::V4(127, 0, 0, 3);
//     println!("localhost: {:#?}", localhost);
// }

// ========== option enum =========
// fn main() {
//     let x: i32 = 5;
//     let y = Some(10);
//     let z: Option<i32> = None;

//     let sum: i32 = x + z.unwrap_or(0);
//     println!("{}", sum);
// }


// ========== option match =========
fn main() {

}
