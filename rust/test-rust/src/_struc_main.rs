#[derive(Debug)]
struct TestStruct {
    var_a : String,
    var_b : String,
}

fn main() {
    let _a = TestStruct {
        var_a : String::from("var_a data"),
        var_b : "bbbb".to_owned(),
    };
    println!("Hello, world! _a = {:?}", _a);
    println!("Hello, world! var_a = {}", _a.var_a);
    println!("Hello, world! var_b = {}", _a.var_b);
}

