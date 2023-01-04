#[derive(Debug)]
pub struct ModSecurity {
  pub m_connector: String,
  pub m_whoami: String,
  // m_logProperties: i32,
  pub m_global_collection: String,
  pub m_ip_collection: String,
  pub m_session_collection: String,
  pub m_user_collection: String,
  pub m_resource_collection: String,
}

impl ModSecurity {
  pub fn new() -> Self {
    Self { 
      m_connector: String::new(),
      m_whoami: String::new(),
      m_global_collection: String::new(),
      m_ip_collection: String::new(),
      m_session_collection: String::new(),
      m_user_collection: String::new(),
      m_resource_collection: String::new(),
    }
  }

  fn setConnectorInformation(&mut self, connector: String) {
    self.m_connector = connector;
  }
}

// struct ModSecurity {
//   public:
//      ModSecurity();
//      ~ModSecurity();
 
//      ModSecurity(const ModSecurity &m) = delete;
//      ModSecurity& operator= (const ModSecurity &m) = delete;
 
//      const std::string& whoAmI();
//      void setConnectorInformation(const std::string &connector);
//      void setServerLogCb(ModSecLogCb cb);
//      /**
//       *
//       * properties   Properties to inform ModSecurity what kind of infornation
//       *              is expected be returned.
//       *
//       */
//      void setServerLogCb(ModSecLogCb cb, int properties);
 
//      void serverLog(void *data, std::shared_ptr<RuleMessage> rm);
 
//      const std::string& getConnectorInformation() const;
 
//      static int processContentOffset(const char *content, size_t len,
//          const char *matchString, std::string *json, const char **err);
 
//      collection::Collection *m_global_collection;
//      collection::Collection *m_resource_collection;
//      collection::Collection *m_ip_collection;
//      collection::Collection *m_session_collection;
//      collection::Collection *m_user_collection;
 
//   private:
//      std::string m_connector;
//      std::string m_whoami;
//      ModSecLogCb m_logCb;
//      int m_logProperties;
//  };