mod collections;

use collections::Collections;
use crate::modSecurity::ModSecurity;
use crate::rules::RulesSet;

pub struct Transaction<'a> {
  m_collections: Option<Collections>,
  m_ms: Option<ModSecurity>,
  m_rules: Option<RulesSet<'a>>,
  m_log_cb_data: String,
  // m_variableTime: String,
  // m_variableTimeDay: String,
  // m_variableTimeEpoch: String,
  // m_variableTimeHour: String,
}

impl Transaction<'_> {
  pub fn new(ms: &ModSecurity, rules: &RulesSet, log_cb_data: String) -> Self {
    let m_collections = Collections::new((*ms.m_global_collection).to_string(), (*ms.m_ip_collection).to_string(), (*ms.m_session_collection).to_string(), (*ms.m_user_collection).to_string(), (*ms.m_resource_collection).to_string());
    let m_ms = ModSecurity {
      m_connector: (*ms.m_connector).to_string(),
      m_whoami: (*ms.m_whoami).to_string(),
      m_global_collection: (*ms.m_global_collection).to_string(),
      m_ip_collection: (*ms.m_ip_collection).to_string(),
      m_session_collection: (*ms.m_session_collection).to_string(),
      m_user_collection: (*ms.m_user_collection).to_string(),
      m_resource_collection: (*ms.m_resource_collection).to_string(),
    };

    let m_rules = RulesSet {
      m_secmarker_skipped: rules.m_secmarker_skipped,
      ..RulesSet::new()
    };

    Self {
      m_ms: Some(m_ms),
      m_rules: Some(m_rules),
      m_log_cb_data: log_cb_data,
      m_collections: Some(m_collections)
    }
  }
}

