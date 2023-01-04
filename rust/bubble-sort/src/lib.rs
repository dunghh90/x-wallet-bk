use std::fmt::Debug;

pub fn bubble_sort<T: PartialOrd + Debug> (v: &mut [T]) {
    for _ in 0..v.len() {
        println!("==v: {:?}", v);
        let mut sorted = true;
        for i in 0..v.len() - 1 {
            if v[i] > v[i + 1] {
                v.swap(i, i + 1);
                sorted = false;
            }
        }
        if sorted { 
            return;
        }
    }
}

#[cfg(test)]
mod tests {
    use super::*;
    #[test]
    fn it_works() {
        let mut v = vec![21, 12, 35, 14];
        bubble_sort(&mut v);
        // let result = 2 + 2;
        assert_eq!(v, vec![12,14,21,31]);
    }
}
