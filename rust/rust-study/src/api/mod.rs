use std::mem::size_of;
use std::rc::Rc;
use std::sync::Arc;

#[derive(Debug)]
pub struct Foo1 {
    pub a: i32,
    // b: f64,
    pub b: i32,
}

// pub fn test_struct() {
//     // let xab: i32 = 10;
//     // println!("xab: {}", size_of::<xab>());
//     println!("Foo: {}", size_of::<Foo>());
//     println!("Option<Foo>: {}", size_of::<Option<Foo>>());
//     println!("Rc<Foo>: {}", size_of::<Rc<Foo>>());
//     println!("Option<Rc<Foo>>: {}", size_of::<Option<Rc<Foo>>>());
//     println!("Arc<Foo>: {}", size_of::<Arc<Foo>>());
//     println!("Option<Arc<Foo>>: {}", size_of::<Option<Arc<Foo>>>());
// }