use unicode_segmentation::UnicodeSegmentation;
use std::collections::HashMap;
fn main() {
    // vector
    let a: [i32;3] = [2, 3, 5]; // array
    let mut v: Vec<i32> = vec![1, 2, 3];
    let mut v2 = Vec::new();
    v2.push(3);
    v2.push(4);
    v2.push(6);
    v2.push(7);

    println!("{:?}", v2);
    println!("{}", &v[2]);

    match v.get(4) {
        Some(four) => println!("This is {}", four),
        None => println!("None",)
    }

    // in ra tung element
    for i in &mut v {
        *i += 10;
    }
    println!("{:?}", v);

    // su dung vector voi enum

    enum SheetCell {
        Int(i32),
        Float(f64),
        Text(String),
    }

    let row = vec![
        SheetCell::Int(5),
        SheetCell::Float(10.12),
        SheetCell::Text(String::from("Blue")),
    ];

    match &row[1] {
        &SheetCell::Float(i) => println!("gia tri i = {}",i),
        _ => println!("this is not float"),
    }


    // String ho tro UTF-8
    let s1 = String::from("string 1");
    let s2 = String::new();
    let s3 = "A String";
    let s4 = s3.to_string();

    let s5 = s1 + " thực hiện test " + &s4;
    println!("{}", s5);

    // in ra chuoi byte
    for i in s5.bytes() {
        println!("{}", i);
    }

    // in chuoi theo ky tu (Scalar values)
    for i in s5.chars() {
        print!("{}", i);
    }
    println!("");

    // HaskMap
    // let mu = String::from("MU");

    // let scores = HashMap::new();

    // for (key , value) in &scores {
    //     println!("{} {}", key, value);
    // }

    // Grapheme Cluster  ky tun in unicode de in ra chu hoan chinh
    for i in s5.graphemes(true) {
        print!("{}", i);
    }
    println!("");

    // Ung dung dem tu
    let text = "hello world this is my childrent of my parent";

    let mut map = HashMap::new();

    for i in text.split_whitespace() {
        let count = map.entry(i).or_insert(0);
        *count += 1;
    }
    println!("{:?}", map);
    
}
