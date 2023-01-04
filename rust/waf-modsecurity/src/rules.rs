use std::fs;

const NUMBER_OF_PHASES: i32 = 7;

#[derive(Debug, Clone)]
struct Rule {
  m_fileName: String,
  m_lineNumber: i32,
  // FIXME: phase may not be neede to SecMarker.
  m_phase: i32,
  m_ruleId: i64,
}

#[derive(Debug, Clone)]
pub struct Rules {
  pub m_rules: Vec<Option<Rule>>,
}

impl Rules {

  // pub fn new() -> Self {
  //   Self {
  //     m_rules: None
  //   }
  // }

  pub fn size(&self) -> i32 { self.m_rules.len().try_into().unwrap() }

  pub fn at(&self, index: i32) -> &Option<Rule> {
    let option_rule = &self.m_rules[index as usize];

    option_rule
  }

  pub fn append(&mut self) -> i32 {1}
  // pub fn append(&mut self, from: &mut Rules, ids: &mut Vec<i64>, err: &String) -> i32 {
    
  //   // for id in ids.iter() {
  //   // for m_rulesAtPhase_item in &self.m_rulesAtPhase {
  //   let mut j = 0;
  //   loop {
  //   // for (let j: i32 = 0; j < from.size(); j++) {
  //     if j == from.size() {
  //       println!("OK, that's enough");

  //       // Exit this loop
  //       break;
  //     }

  //     let rule = from.at(j); //TODO: RuleWithOperator

  //     let exist_rule = match rule {
  //       Some(_rule) => true,
  //       None => false
  //     };

  //     let exist_ruleId = match ids.binary_search(&(rule.as_ref().unwrap()).m_ruleId) {
  //       Ok(_v) => true,
  //       Err(_e) => false,
  //     };

  //     if exist_rule && exist_ruleId {
  //       if err != "" {
  //           println!("Rule id: {}  is duplicated", rule.as_ref().unwrap().m_ruleId);
  //       }
  //       return -1;
  //     }

  //     j += 1;
  //   }
  //   // self.m_rules.insert(m_rules.end(), from.m_rules.begin(), from.m_rules.end());
  //   // self.m_rules.extend(from.m_rules);
  //   for val in &from.m_rules {
  //     self.m_rules.push((*val).clone());
  //   }
  //   return j;
  // }

}


pub struct RulesSetPhases<'a> {
  m_rulesAtPhase: &'a [Option<Rules>;8]
  // m_rulesAtPhase: Vec<Option<Rules>>
}

impl<'a> RulesSetPhases<'a> {
  pub fn dump() {
    // for (let mut j = 0; j < m_rules.size(); j++) {
    // for j in 0..Rules::m_rules.size() {
    //   println!("    Rule ID: {}", Rules::m_rules.at(j).getReference());
    //   println!("--{}", Rules::m_rules.at(j));
    // }
  }

  pub fn append(&self, from: &RulesSetPhases, err: &str) -> i32 {
    let mut amount_of_rules: i32 = 0;
    let mut v: Vec<i64> = Vec::new();

    for m_rulesAtPhase_item in self.m_rulesAtPhase {
    // for i in 0..NUMBER_OF_PHASES {
        // v = Vec::with_capacity(self.m_rulesAtPhase(i));
        for z in 0..m_rulesAtPhase_item.as_ref().unwrap().size() {
            let rule_ckc = m_rulesAtPhase_item.as_ref().unwrap().at(z.try_into().unwrap()); // TODO: RuleWithOperator
            // if !rule_ckc {  
            //   continue;
            // }
            v.push(rule_ckc.as_ref().unwrap().m_ruleId);
        }
    }
    // std::sort (v.begin(), v.end());

    let mut phase = 0;
    for m_rulesAtPhase_item in self.m_rulesAtPhase {
    // for phase in 0..NUMBER_OF_PHASES {
        let res = &m_rulesAtPhase_item.as_ref().unwrap().append();
        // let res = m_rulesAtPhase_item.as_ref().unwrap().append(
        //                                             &mut from.at((phase).try_into().unwrap()), 
        //                                             &mut v, 
        //                                             &err.to_owned());
        if *res < 0 {
            return *res;
        }
        amount_of_rules = amount_of_rules + res;
        phase = phase + 1;
    }

    amount_of_rules
  }

  // TODO: 
  fn at(&self, index: i32) -> Rules { 
    Rules {
      m_rules: vec![Some(Rule{
        m_fileName: String::new(),
        m_lineNumber: 0,
        // FIXME: phase may not be neede to SecMarker.
        m_phase: 0,
        m_ruleId: 0,
      })]
    }
    // self.m_rulesAtPhase[index as usize].unwrap() 
  }
}

pub struct RulesSetProperties {
  m_requestBodyLimitAction: String,
  m_responseBodyLimitAction: String,

  m_secRequestBodyAccess: String,
  m_secResponseBodyAccess: String,
  m_secXMLExternalEntity: String,
  m_tmpSaveUploadedFiles: String,
  m_uploadKeepFiles: String,
  
  m_httpblKey: String,
  m_uploadDirectory: String,
  m_uploadTmpDirectory: String,
  m_secArgumentSeparator: String,
  m_secWebAppId: String,
}

pub struct Driver<'a> {
  pub buffer: String,
  pub trace_scanning: bool,
  pub trace_parsing: bool,
  pub m_lastRule: bool,   // TODO
  // start From RulesSetProperties
  pub m_requestBodyLimitAction: String,
  pub m_responseBodyLimitAction: String,
  pub m_secRequestBodyAccess: String,
  pub m_secResponseBodyAccess: String,
  pub m_secXMLExternalEntity: String,
  pub m_tmpSaveUploadedFiles: String,
  pub m_uploadKeepFiles: String,

  pub m_remoteRulesActionOnFailed: String,
  pub m_secRuleEngine: String,
  // end From RulesSetProperties

  pub m_rulesSetPhases: Option<RulesSetPhases<'a>>,
}

impl Driver<'_> {
  pub fn new() -> Self {
    // let mut test_vec = Vec::with_capacity(8);
    // for _ in 0..8 {
    //   test_vec.push(Rules::new());
    // }
    Self {
      buffer: String::from(""),
      trace_scanning: false,
      trace_parsing: false,
      m_lastRule: false,
      // start RulesSetProperties(Some(DebugLog))
      m_requestBodyLimitAction: String::from("PropertyNotSetBodyLimitAction"),
      m_responseBodyLimitAction: String::from("PropertyNotSetBodyLimitAction"),
      m_secRequestBodyAccess: String::from("PropertyNotSetConfigBoolean"),
      m_secResponseBodyAccess: String::from("PropertyNotSetConfigBoolean"),
      m_secXMLExternalEntity: String::from("PropertyNotSetConfigBoolean"),
      m_tmpSaveUploadedFiles: String::from("PropertyNotSetConfigBoolean"),
      m_uploadKeepFiles: String::from("PropertyNotSetConfigBoolean"),
      
      m_remoteRulesActionOnFailed: String::from("PropertyNotSetRemoteRulesAction"),
      m_secRuleEngine: String::from("PropertyNotSetRuleEngine"),
      // end RulesSetProperties(Some(DebugLog))
      m_rulesSetPhases: None,
    }
  }

  pub fn parseFile(&mut self, uri: &str) -> i32 {
    // std::ifstream t(f);
    // std::string str;

    // if (utils::isFile(f) == false) {
    //     m_parserError << "Failed to open the file: " << f << std::endl;
    //     return false;
    // }

    // t.seekg(0, std::ios::end);
    // str.reserve(t.tellg());
    // t.seekg(0, std::ios::beg);

    // str.assign((std::istreambuf_iterator<char>(t)),
    //     std::istreambuf_iterator<char>());

    // return parse(str, f);

    let mut _str: String;

    let contents = fs::read_to_string("src/".to_owned() + uri)
        .expect("Should have been able to read the file");
    
    self.buffer = contents;
    // println!("With text:\n{_str}");
    1
    
  }
}

pub struct RulesSet<'a> {
  pub m_secmarker_skipped: i32,

  // start From RulesSetProperties
  pub m_requestBodyLimitAction: String,
  pub m_responseBodyLimitAction: String,
  pub m_secRequestBodyAccess: String,
  pub m_secResponseBodyAccess: String,
  pub m_secXMLExternalEntity: String,
  pub m_tmpSaveUploadedFiles: String,
  pub m_uploadKeepFiles: String,

  pub m_remoteRulesActionOnFailed: String,
  pub m_secRuleEngine: String,

  // end From RulesSetProperties
  pub m_rulesSetPhases: Option<RulesSetPhases<'a>>,

}

impl RulesSet<'_> {

  pub fn new() -> Self {
    // let mut test_vec = Vec::with_capacity(8);
    // for _ in 0..8 {
    //   test_vec.push(Rules::new());
    // }
    Self { 

      // start RulesSetProperties(Some(DebugLog))
      m_requestBodyLimitAction: String::from("PropertyNotSetBodyLimitAction"),
      m_responseBodyLimitAction: String::from("PropertyNotSetBodyLimitAction"),
      m_secRequestBodyAccess: String::from("PropertyNotSetConfigBoolean"),
      m_secResponseBodyAccess: String::from("PropertyNotSetConfigBoolean"),
      m_secXMLExternalEntity: String::from("PropertyNotSetConfigBoolean"),
      m_tmpSaveUploadedFiles: String::from("PropertyNotSetConfigBoolean"),
      m_uploadKeepFiles: String::from("PropertyNotSetConfigBoolean"),
      
      m_remoteRulesActionOnFailed: String::from("PropertyNotSetRemoteRulesAction"),
      m_secRuleEngine: String::from("PropertyNotSetRuleEngine"),
      // end RulesSetProperties(Some(DebugLog))

      m_secmarker_skipped: 0,
      m_rulesSetPhases: None,
    }
  }

  pub fn loadFromUri(&self, uri: String) -> i32 {
    println!("======== {}", uri);

    let mut driver = Driver::new();
  
    if driver.parseFile(&uri) == 0 {}
    //     // m_parserError << driver->m_parserError.str();
    //     drop driver;
    //     return -1;
    // }
  
    // let rules: i32 = self.merge(driver);
    // drop driver;
  
    // rules
    1

  }

  fn merge(&self, from: &Driver) -> i32 {
    let mut amount_of_rules: i32 = 0;
    let m_parserError = String::new();

    let m_rulesSetPhases_from = from.m_rulesSetPhases.as_ref().unwrap();

    match &self.m_rulesSetPhases {
      Some(m_rulesSetPhases) => { amount_of_rules = m_rulesSetPhases.append(m_rulesSetPhases_from, &m_parserError); }
      None => { println!("data null");}
    }
    // amount_of_rules = self.m_rulesSetPhases.append(from.m_rulesSetPhases, &m_parserError);
    // mergeProperties(&from, &self, &m_parserError);

    amount_of_rules
  }

  fn dump() {
    // RulesSet::m_rulesSetPhases.dump();
  }
}

