use ftm::io
use rand::{RngCore, Rng, SeedableRng};

mod back_house {
    pub struct Breakfast {
        pub toast: String,
        pub fruit: String,
    }

    pub enum Salar {
        Soup,
        Salar
    }

    impl Breakfast {
        pub fn monday(toast: &str) -> Breakfast {
            Breakfast {
                toast: String::from(toast),
                fruit: String::from("banana")
            }
        }
    }
}

mod front_house;
fn eat_res() {
    front_house::hosting::add_to_waitlist();
}

fn eat_at_restanrant() {
    let mut order = back_house::Breakfast::monday("Fish");
    order.toast = String::from("Chicken");

    let order2 = back_house::Breakfast {
        toast: String::from("Wheat"),
        fruit: String::from("apple"),
    };

    let order3 = back_house::Salar::Soup;
}