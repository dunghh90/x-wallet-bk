fn add(a: i32, b: i32) -> i32 {
    a + b
}

fn main() {
    // ========== hello world ============
    println!("Hello, world123!");

    // ========== call function ============
    let result = add(2,3);
    println!("result = {}", result);

    // ========= flow if =========
    let mut a = 100;
    a = a + 1;
    println!("a = {}", a);
    if a > 99 {
        println!("lon hon 99");
    } else if a < 99 {
        println!("be hon 99");
    } else {
        println!("bang");
    }

    // ========= flow loop =========
    let mut b = 0;
    loop {
        if b == 5 {
            break;
        }
        println!("b = {}", b);
        b +=1;
    }

    // ========= while =========
    let mut a3 = 0;
    while a3 != 5 {
        println!("{}", a3);
        a3 += 1;
    }

    // ========= tuple =========
    let tup = ("hello", 100_000);
    // println!("tup = {:?}",tup)
    println!("_interger = {}", tup.0);
    let (_int, _str) = tup;
    println!("int = {}", _int);

    // ========= hashing =========
    // let _hashing = [0; 32];
    // // println!("hashing = {:?}", _hashing)
    // for i: &i32 in _hashing.iter() {
    //     print!("{}", i)
    // }

    // =========== Ownership ========
    let _s1 = String::from("hello");
    let _s2 = _s1; // de ko tro vao 1 bo nho thi dung ham clone
    // let _s2 = _s1.clone(); // copy ra bo nho moi
    // println!("{}", _s1); ==> loi vi s1 la bi muon

    // ============= Ownership error 2 ======
    let s3 = String::from("hello");
    takes_ownership(s3);
    // println!("{}", _s3); ==> loi vi s1 la bi muon boi takes_ownership
    // fix error2_1
    let _s4 = gives_ownership();
    println!("{}", _s4);

    // fix error2_2
    let _s4 = gives_ownership();
    let _s5 = String::from("hahahahah");
    let _s6 = takes_ownership(_s5);
    // println!("{}, {}", _s4, s6);

    let _s7 = String::from("hahahahah");
    let (_s8, len) = calculate_length(_s7);

    // ========== tham chieu & =====
    // let len = calculate_length_thamchieu(&_s7);

    // ========== tham chieu &mut =====
    let mut _s9 = String::from("hahahahah");

    let len2 = calculate_length_thamchieu_mut(&mut _s9);

}

fn gives_ownership() -> String {
    let some_string = String::from("hello");
    some_string
}

fn takes_ownership(some_string: String) -> String {
    some_string
}

fn calculate_length(some_string: String) -> (String, usize) {
    let length = some_string.len();
    (some_string, length)
}
fn calculate_length_thamchieu(some_string: &String) -> usize {
    let length = some_string.len();
    length
}

// Neu mut tham chieu
fn calculate_length_thamchieu_mut(some_string: &mut String) -> usize {
    some_string.push_str("world");
    let length = some_string.len();
    length
}

