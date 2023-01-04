#[allow(unused_imports)]
use crate::modSecurity::ModSecurity;

pub struct Collections {
  // m_global_collection_key: String,
  // m_ip_collection_key: String,
  // m_session_collection_key: String,
  // m_user_collection_key: String,
  // m_resource_collection_key: String,

  m_global_collection: String,
  m_ip_collection: String,
  m_session_collection: String,
  m_user_collection: String,
  m_resource_collection: String,
  // m_tx_collection: String,
}

impl Collections {
  // pub fn new(ms: &ModSecurity) -> Self {
  pub fn new(m_global_collection: String, m_ip_collection: String, m_session_collection: String, m_user_collection: String, m_resource_collection: String) -> Self {
    Self {
      m_global_collection: m_global_collection, 
      m_ip_collection: m_ip_collection,
      m_session_collection: m_session_collection,
      m_user_collection: m_user_collection,
      m_resource_collection: m_resource_collection
    }
  }
}

