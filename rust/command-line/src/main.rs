use std::process::Command;

fn main() {
    let mut cmd = Command::new("python");
    cmd.arg("dcode.py");
    match cmd.output {
        Ok(o) => {
            unsafe {
                println!("Output: {}", String::from_utf8_unchecked(o.stdout));
            }
        },

        Err(e) => println!("Error: {}", e)
    }
}
