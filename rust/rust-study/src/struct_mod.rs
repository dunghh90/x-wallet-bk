#[derive(Debug)]
pub struct ModSecurity {
  pub m_connector: String,
  pub m_whoami: String,
  // m_logProperties: i32,
}

impl ModSecurity {
  pub fn new() -> Self {
    Self { 
      m_connector: String::new(),
      m_whoami: String::new(),
    }
  }

  pub fn setConnectorInformation(&mut self, connector: String) {
    self.m_connector = connector;
  }

  fn test_error(err: &mut String) -> i32 {
    *err = String::from("test");
    1
  }
}

// fn msc_rules_add_file(rules: &RulesSet, file: String, error: &mut String) -> i32 {
//   let ret = rules.loadFromUri(file);
//   if (ret < 0) {
//       // *error = strdup(rules->getParserError().c_str());
//       error = "error connect file";
//   }
//   ret
// }