// ======== Lo trinh hoc rust ===========
Một số projects để luyện 
1. Làm các project nhỏ - dạng bài tập. Bên mình có 1 loạt bài pass đầu vào như: 1. Viết lại tail linux command, 2. Sắp xếp n số theo thứ tự tăng sử dụng đa luồng 3. producer and consumer
2. Viết lại các kiểu dữ liệu để cảm nhận được cách viết trong Rust cần Rc, Arc, Mutex,  Lock , Var, Wait, Signal
3. Làm bài tập truyền files giữa 2 máy. Viết code từ socket ko dùng thư viện
4. Project liên quan đến CRUD như trang netflix film: Dựng server dùng rocket, actix, tide...
5. Khám phá tiếp ..

=====================================================
Noi dung nha tuyen dung mong muon:
  - ung vien co kien thuc nen tang: cac chuc nang cau truc cua Rust, framework Rocket, Active-x
  - kha nang tim hieu sau he thong -> tung la du an lien quan den he thong OT (tai lieu detail design, basic design,...)
  - kinh nghiem, kien thuc ve crypto -> lam ve x-wallet: cac dong coin: eth, bsc, bch, avax, tron, xrp, xtz,...

----------------------------------------------------------------
framework rocket, actix, tide


/// ================
// Rust Rocket framework
  - rustup install nightly
  - rustup override set nightly
  - cargo install diesel_cli --no-default-features --features postgres
  - sudo apt install libpq-dev  -> postgres
  - sudo apt install libmysqlclient-dev  -> my sql
  - sudo apt install libsqlite3-dev  -> sqlLite
  - export DATABASE_URL=postgres://rocket:rocket@localhost/rocket

Ubuntu 18.04
  - apt search lmysqlclient
  - sudo apt install libmariadbclient-dev-compat
  - cargo install diesel_cli --no-default-features --features mysql

const phai khai bao kieu
 const viet hoa
 let co the dc gan boi cong thuc
 let ko the khai bao global


** dynamic bien va static bien
Rust:
  - sau khi ra khoi ham thi tu dong xoa bien

** Tai sao ung dung lien quan den tai chinh thi dung rust
  - Toc do

** rust
  - Mọi biến mặc định đều là immutable, và không được mang giá trị null (nên không có lỗi NullReferenceException)
  - Trong Rust, một vùng nhớ luôn có một biến sỡ hữu nó.
  - Gán một biến vào một biến khác, tức là bạn chuyển quyền sở hữu vùng nhớ của biến đó cho biến mới. Biến cũ không được quyền truy xuất tới đó nữa.
  - Trong một thời điểm, chỉ có một trong hai trường hợp có thể xảy ra, đó là:
    + Nhiều biến khác có thể mượn vùng nhớ đó chỉ để đọc (read only).
    + Chỉ có một biến có quyền mượn để ghi giá trị lên đó.
  - Một biến chỉ có thể tồn tại bên trong scope của nó, ra khỏi scope đó nó sẽ bị giải phóng ngay lập tức.

** rust va lap trinh web
https://github.com/huytd/codedaily-v3
https://topdev.vn/blog/rust-va-lap-trinh-web/#2-dung-rust-viet-restful-api


** Q&A rust:
   - Dynamic typing
   - Implement bài toán duyệt cây nhị phân với Rust
   - rust telegram viet
   - rust la static type hay dynamic type. 2 loai nay thi giup gi
    + static type:(rust) Kieu du lieu cua bien duoc quy dinh khi compile, runtime thi ko the doi (co the khai bao hoac ko can khai bao kieu). Trong rust muon doi kieu thi dung shadowing khai bao lai bien trung ten.
    rust ko the ep kieu trong qua trinh chay
    dam bao tinh chac che. compile nhanh hown
    + dynamic type:(javascript) kieu du lieu co the thay doi trong runtime
  - rust tu xoa bien sau khi ra khoi scope
  - Tai sao lai dung rust???
    + nhanh
    + an toan

  - Tai sao println! co dau !, bo ! co chay ko
  - "chuoi".to_string()
    -> "chuoi" chi la charactor nen phai convert thanh string    
  - Tai sao struct truyen vao function??
    + phai truyen vao tham chieu

** iter() trong vector
  - luu trong Heap

** tuple type:
  - la kieu compound type. (array)
  - tuple co do dai co dinh sau khi khai bao
  - moi phan tu co the co kieu khac nhau
  - khai bao kieu du lieu cho tung phan tu
  - toi da 12 phan tu

** enum type:
  - thanh phan co the co kieu khac nhau
  - function trong enum

** overload
  - rust ko co overload.(ten function giong, param thi khac)

** ownership
  - Giup rust dam bao memory an toan ko can phai giai phong bo nho, quan ly memory su dung trong luc thuc thi
  - memory được quản lý bởi một ownership system, rust bat loi luc compile
  - Moi 1 gia tri chi co 1 bien lam owner, chi co 1 owner tai 1 thoi diem, khi owner ra khoi scope thi value se bi drop
  - let x = 10 : nam tren stack
  - tuple struct -> cung chi la struct

** Borrow checker: muon dia chi
  - Khi truyền một variable (thay vì reference tới variable) cho một function khác, ta sẽ mất quyền ownership. Function đó sẽ là owner của variable này và bạn không thể sử dụng lại được nữa ở context cũ.


** consencus

** array
  - Du lieu phai cung kieu
  - let array_x = [7; 11] -> in ra 11 so 7
  - 
** struct
  - co 3 loai struct:
    + Unit struct: mot struct ma ko co gi ca
    + Tuple struct: chi can dinh nghi kieu du lieu, ko can dinh nghia ten field
    + Named struct: kieu pho bien, phai dinh nghia ten field
  - implement function struct
  ex:
    pub struct Human { 
      pub name: String,
      pub sex: String, 
    }
    impl Human {
      pub fn print_detail(&self) {
        println!("{}", self.name);
      }
    }

** String
  - Luu trong Head
  - String rong:
    let str = String::new();
  - function support: .is_empty(), .split_whitespace, .contains("abc), .push_str(), .split("/)
  - Get length string: capacity, len
    + capacity: dung with_capcity de khai bao do dai string. Neu thieu se tu dong nhan doi len
  

** Noi dung quan trong
  - managing rust chia module muc 7
  - smart point
  - multi thread

** Trais, Drive
  - tuong tu class interface co cac ham abstract
  - De print object thi dung Drive(Debug)
  - Tao giup ho tro chuyen doi lap trinh huong doi tuong

** Closure:  |bien| -> {}
  - Khac voi function binh thuong kieu du lieu dau vao va kieu du lieu dau ra ko bat buoc
  - Vua khai bao vua thuc thi nhung 

** use
  - Chức năng chính của use là bind lại full path của element vào một tên mới, để chúng ta không cần phải lặp lại một tên dài mỗi lần sử dụng.

** regular_exception
  - phai co regex

** http request

** web program muc 19

** smart pointer

** print pointer
  println!("{:?} {:p}", pointer, &pointer);

** Head/stack
  - Stack: Nhung gi biet duoc size thi se duoc luu trong stack (vd: so nguyen, float)
    + function cung la kieu co ban va luu tren stack
  - Head: luu gia tri chua biet size. size chua biet trong thoi diem compile hoac thay doi khi runtime. 
    + thang alocate memory se di tim vung nho va tra lai dia chi vung trong va dia chi nay luu trong stack


** command line dieu khien rust

*** Smart pointer ***
** thread/multi thread
** channel


** Box, change object: tinh toan tren Heap thi danh doi phan nho ve toc do va bo nho runtime, con tinh toan luc compile time thi nhanh hon
  - Trais object va box:
    + Box: dung Box::new() de chuyen bien tu stack sang heap (dynamic dispatch)
    + Trais: la 1 object interface trong lap trinh huong doi tuong (dynamic dispatch luu o Heap)
    + Trais -> la 1 object giup cho moi element co the hoat dong voi chuc nang, kieu du lieu khac nhau
    + Trais giup ich cho cac project phat trien cong cu moi chuc nang moi
  Loi ich la gi? Tai sao lai su dung no?
Collection co 3 loai: vector, string, hash map -> moi element thi phai giong nhau


** Arc, Rc, RefCell, Mutex
  - Rc (referency counted): tang bo dem owner, 1 chu so huu co the share du lieu cho nguoi khac su dung.
  - smart pointer cho phep chia se du lieu cho nhieu owner. Khi drop du lieu ban dau thi du lieu do van con su dung
  - Arc (atomic referency count): dung an toan khi multi thread
  - RefCell: Rc chi chia se owner chu ko thay doi duoc data, giup tham chieu va chinh sua tuong tu nhu dung mut cho bien,
  co the thay doi duoc du lieu struct, thuong la clone, copy ra du lieu, nen ko dung cho du lieu lon.
  refcell giup vay muon du lieu
  - Refcell chay trong qua trinh runtime, ko an toan tren multi thread
  - So sanh mut va refcell
    + refcell thay doi duoc kieu du lieu duoc share thuong dung ket hop voi Rc, refcell tao ra vung nho va thay doi moi thu trong struct, cell luon move hoac copy du lieu vi vay refcell thi tuong tu nhung la tham chieu nen phai dam bao rule ownership, ko an toan khi multi thread
    + mut: thuong gap van de khi compile va kho giai quyet ownership

  - Mutex: giup kiem soat viec doc va ghi du lieu tai 1 thoi diem, 


======================================================
** Option Enum & Match
  - enum la dinh nghia kieu du lieu moi, co the co function cua kieu
  - struct la dinh nghia class/object cua doi tuong gom thuoc tinh va function
  - convert kieu Option thanh kieu binh thuong
    + y.unwrap_or(default)
    + Option dung de check gia tri nhap vao la kieu so hay ko phai la kieu so (tiet kiem duoc buoc kiem tra phai la kieu so hay ko)
** Packages, Crates, Modules
  - crate : thu vien giong npm
  - src/bin: noi luu cac file main khac
  - cargo new --lib libfile: tao ra lib cargo, se co mod de code ra nhung chuc nang hay su dung
  - module o level 2 luon luon la private, tuong tu function trong module cung la private
  Nha hang gom nhieu module:  
    + front_house:
      - hosting: fn add to waitlist, fn seat at table
      - serving: fn take order, fn take payment
    Cach goi function bang cach:
      fn eat_at_restaurent() {
        crate::front_house::hosting::add_to_waitlist();
      }
    dung super: lui lai 1 level
    + back_house: 
      - fn cook_order
      - fn fix_cook

  - use: dung khi goi cac lib dependency crate
  - mod: co the tao file co cung ten voi module hoac ten folder cung ten roi mod la ten file


** Generic Types   kieu T
  - giup giam thieu luong code bi trung lap trong qua trinh code, code nhieu lan dung di dung lai
  - Chinh la Option<T> kieu T de tranh bi trung lap
  - La kieu du lieu chung cho phep nhap bat ky kieu du lieu nao

** Bubble sort (giai thuat)
  - https://visualgo.net/en/sorting
  - CS50
  - Thuat toan sap xep
** Data construct (cau truc du lieu)- Merge Sort (chia de tri)
  - 

** Su khac nhau giua cac ngon ngu:
  - bo thu gom rac (javascrip, python): viet code nhanh, rac nhieu, file lon -> toc do bien dich chay chuong trinh lau
  - quan ly thu cong: tu quan ly bo nho, code lau, de xay ra loi trong qtrinh bien dich, toc do nhanh
  - ownership model: tuong tu nhu qly thu cong tu quan ly bo nho, khac phuc viec xay ra loi trong qua trinh chay vi co chuc nang quan ly bo nho tot, file nho toc do nhanh

** stack & heap
  - Bo tri bien, function nam o dau
  - Stack bo nho co kich thuoc co dinh ko thay doi trong qua trinh chay
  - Head thi thay doi duoc dung luong trong qua trinh chay