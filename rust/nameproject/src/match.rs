
// ========== option match =========
fn main() {
    decimals(Coin::Bitcoin(Balance::Shark));
}

#[derive(Debug)]
enum Balance {
    Small,
    Shark,
}

enum Coin {
    Solana,
    Bitcoin(Balance),
    Near,
}

fn decimals(coin: Coin) -> u8 {
    match coin {
        Coin::Solana => {
            println!("Solana Match");
            1
        }
        Coin::Near => 10,
        Coin::Bitcoin(bala) => {
            println!("I am {:?}", bala);
            20
        }
    }
}
