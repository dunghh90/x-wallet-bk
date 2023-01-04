
// Trait: dung de tao chuc nang mac dinh cua san pham
struct Data {
    num1: i32,
    num2: i32,
    str1: String,
    optional: Option<i32,
}
impl Data {
    fn new() -> self {
        Data {
            num1: 15,
            num2: 25,
            str1: "some string2".to_string(),
            optional: None,
        }
    }
}

trait Transform {
    fn revert(&self) -> String;
}

impl Transform for Data {
    fn revert(&self) -> String {
        self.str1.chars().rev().collect::<String>()
    }
}
fn main() {
    let a = Data::new();
    println!("{}", a.revert)
}
