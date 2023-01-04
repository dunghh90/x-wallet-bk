mod front_house {
    pub mod hosting {
        pub fn add_to_waitlist() {}
        pub fn seat_at_table() {}
    }

    pub mod serving {
        pub fn take_order() {}
        pub fn take_payment() {}
    }
}

fn call_order() {}

mod back_house {
    fn cook_order() {}
    fn fix_order() {
        super::call_order();
        cook_order();
    }
}

fn eat_at_restanrant() {
    crate::front_house::hosting::add_to_waitlist();

}