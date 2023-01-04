// https://www.youtube.com/watch?v=JxA2Y0G80sg&list=PLFnEYduGTiXE2ejxmzTIraP2feI-pmeuw&index=11

// Authentication
// Authorization
struct Employee {
    position: Position,
    status: Status,
}

enum Position {
    CEO,
    CTO,
    Manager,
}

enum Status {
    Active,
    Denied
}
fn try_access(employee: &Employee) -> Result<(), String> {
    match employee.status {
        Status::Denied => return Err("Access denied".to_owned()),
        _ => (),
    };
    match employee.position {
        Position::CEO => Ok(()),
        // Position::CTO => Ok(()),
        Position::Manager => Ok(()),
        _ => Err("invalid position".to_owned()),
    }
}

fn print_access(employee: &Employee) -> Result<(), String> {
    let access = try_access(employee)?;
    println!("access");
    Ok(())
}

fn main() {
    // let manager = Position::Manager;
    let manager = Employee {
        position: Position::Manager,
        status: Status::Active,
    };

    let ceo = Employee {
        position: Position::CEO,
        status: Status::Denied,
    };

    let cto = Employee {
        position: Position::CTO,
        status: Status::Active,
    };

    print_access(&cto);
    

    // let access = try_access(&manager1);
    // if access.is_ok() {
    //     println!("access");
    // } else {
    //     println!("denied");
    // }

}

