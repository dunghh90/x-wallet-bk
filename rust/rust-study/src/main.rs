// mod api;
// use api::test_str::{Foo};
// mod api;
// use api::Foo1;
mod test_struct;
use crate::test_struct::Foo;
use crate::test_struct::DienTich;

mod struct_mod;
use struct_mod::ModSecurity;
// use api::task::{
//     get_task,
//     submit_task,
//     start_task,
//     // fail_task,
//     // complete_task,
//     // pause_task,
// };
mod enum_option;
fn main() {
    // let mut x: i32 = 1;
    // {
    //     let y = &mut x;
    //     let z = x;
    //     println!("y = {:?}", y);
    //     // println!("z = {:?}", z);
    // }
    // x = 2;
    // test_struct::test_struct();

    let mut test_foo = Foo {
        a: 50,
        b: 60,
    };
    let mut test_dt = DienTich {
        a: 40,
        b: 50,
    };
    // println!("test_foo = {:?}", test_foo);
    // println!("test_dt = {:?}", test_dt);

    // let cst = ModSecurity::new();

    // println!("cst = {:?}", cst);

    let mut str = "hahahaha".to_string();

    // let test_arg = ModSecurity::test_error(&mut str);
    // println!("test_arg = {}", test_arg);
    println!("str = {}", str);

    // let test_arg1 = cst.test_error(&mut str);
    // println!("test_arg1 = {}", test_arg1);

    // enum_option::enum_option_study();

    let mut agr1 = ModSecurity::new();

    let varable1 = struct_arg_pointer(&mut agr1);
    println!("varable1 = {:?}", varable1);
}


fn struct_arg_pointer(arg: &mut ModSecurity) -> ModSecurity {
    // let result = arg.setConnectorInformation(String::from("testststt"));
    // let result = arg.new();
    let result = ModSecurity::new();
    result
}