#[derive(Debug)]
struct Member {
    username: String,
    email: String,
    age: u64,
    active: bool,
    sex: bool
}

#[derive(Debug)]
struct Hinhchunhat {
    dai: u32,
    rong: u32
}

impl Hinhchunhat {
    fn dien_tich(&self)  -> u32 {
        self.dai * self.rong
    }

    fn test_return(&self, _num: i32) -> i32 {
        if _num == 1 {
            println!("num la {}", _num);
            return 1;
        }
        println!("else num la {}", _num);
        2
    }

    fn data(&self, _num: u32) -> Self {
        Self {
            dai: _num,
            rong: self.rong,
        }
    }
}

fn main() {
    let member1: Member = Member {
        email: String::from("aasfdsfs"),
        username: String::from("aasfdsfs"),
        age:28,
        active:true,
        sex:true
    };
    let name: String = member1.username;
    println!("name= {}", name);

    // Tao member2 ke thua tu member 1
    let member2 = Member {
        username: String::from("aasfdsfs"),
        ..member1
    };
    // du lieu array
    // let hinh_chu_nhat: (u32, u32) = (30, 46);
    let hinh_chu_nhat = Hinhchunhat {dai: 30, rong: 50};
    // println!("dien tich {}", dien_tich(hinh_chu_nhat));
    println!("dien tich {}", hinh_chu_nhat.dien_tich());
    println!("dien tich data {:?}", hinh_chu_nhat.data(10));
    println!("return data {}", hinh_chu_nhat.test_return(1));

    // dien tich struct member
    let kichthuocstruct = Hinhchunhat {dai: 30, rong: 40};
    println!("dien tich struct {}", dien_tich_struct(&kichthuocstruct));
}

// fn dien_tich(kichthuoc: (u32, u32)) -> u32 {
//     kichthuoc.0 * kichthuoc.1
// }
fn dien_tich_struct(kichthuoc: &Hinhchunhat) -> u32 {
    kichthuoc.dai * kichthuoc.rong
}

