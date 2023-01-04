// dung generic type khi can giam thieu viec trung lap code
struct Point<T, U> {
    x: T,
    y: U,
}

impl<T, U> Point<T,U> {
    fn mixed<V, W>(self, orther: Point<V, W>) -> Point<T, W> {
        Point {
            x: self.x,
            y: other.y,
        }
    }
}
fn main() {
    let number_list = vec![202, 1354, 1243, 6345, 12];
    let mut largest = get_lastgest(number_list);
    // let mut largest = number_list[0];
    // for number in number_list {
    //     if number > largest {
    //         largest = number;
    //     }
    // }

    println!("{}", largest);

    let p1 = Point {x: 5, y: 10};
    let p2 = Point {x: 10.5, y: 20.5};
    println!("{}", p2.x);
}



fn get_lastgest<T: PartialOrd + Copy>(number_list: Vec<T>) -> T {
    let mut largest = number_list[0];
    for number in number_list {
        if number > largest {
            largest = number;
        }
    }
    largest
}
