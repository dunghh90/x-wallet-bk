// https://www.youtube.com/watch?v=_HBVbhwrODM&list=PLFnEYduGTiXE2ejxmzTIraP2feI-pmeuw&index=10
// LifeTime la bien return ve tham chieu trong pham vi ham
// suy luan xem co nam chung 1 cho cung lifetime voi nhau khong
fn main() {
    // let a = Data::new();
    // println!("{}", a.revert)
    let num1 = 10;
    let num2 = 30;

}

fn get_ref<'a, 'b: 'a>(param_1: &'a i32, param_2: &'b i32) -> &'a i32 {
    if param_1 > param_2 {
        param_1
    } else {
        param_2
    }
}
