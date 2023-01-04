#[allow(unused_variables)]
/*
 * ModSecurity, http://www.modsecurity.org/
 * Copyright (c) 2015 - 2021 Trustwave Holdings, Inc. (http://www.trustwave.com/)
 *
 * You may not use this file except in compliance with
 * the License.  You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * If any of the files related to licensing are missing or if you have any
 * other questions related to licensing please contact Trustwave Holdings, Inc.
 * directly using the email address security@modsecurity.org.
 *
 */
mod modSecurity;
mod rules;
mod transaction;

use modSecurity::ModSecurity;
use rules::RulesSet;
use transaction::Transaction;
 
// let main_rule_uri = vec!["basic_rules.conf"];
const main_rule_uri: &str = "basic_rules.conf";

// println!("basic rule: {}", main_rule_uri);

fn msc_init() -> ModSecurity {
    ModSecurity::new()
}

fn msc_set_connector_info(msc: &mut ModSecurity, connector: String) {
    msc.m_connector = connector;
}

fn msc_create_rules_set() -> RulesSet<'static> {
    RulesSet::new()
}

fn msc_rules_add_file(rules: &RulesSet, file: &str, error: &mut String) -> i32 {
    let ret = rules.loadFromUri(file.to_string());
    if ret < 0 {
        // *error = strdup(rules->getParserError().c_str());
        *error = String::from("error connect file");
    }
    ret
}

#[allow(unused_variables)]
fn msc_rules_cleanup(_rules: &RulesSet) {
    // drop rules;
}

fn msc_cleanup(_modsec: &ModSecurity) {
    // drop modsec;
}

fn msc_rules_dump(_rules: &RulesSet) {
    // rules.dump();
}

fn msc_rules_add_remote(_rules: &RulesSet, _key: String, _uri: String, _error: &String) -> i32 {
    // let ret = rules.loadRemote(key, uri);
    // if ret < 0 {
    //     // *error = strdup(rules->getParserError().c_str());
    //     *error = "Error add remote file";
    // }
    // ret
    1
}

fn msc_new_transaction<'a>(ms: &'a ModSecurity, rules: &'a RulesSet<'a>, _logCbData: String) -> Transaction<'a> {
    Transaction::new(ms, rules, _logCbData)
}

fn msc_process_connection(_transaction: &Transaction, client: String, cPort: i32, server: String, sPort: i32) -> i32 {
    // transaction.processConnection(client, cPort, server, sPort)
    1
}

fn msc_process_uri(_transaction: &Transaction, uri: String, protocol: String, http_version: String) -> i32 {
    // transaction.processURI(uri, protocol, http_version)
    1
}

fn msc_process_request_headers(_transaction: &Transaction) -> i32 {
    // transaction.processRequestHeaders()
    1
}

fn msc_process_request_body(_transaction: &Transaction) -> i32 {
    // transaction.processRequestBody()
    1
}

fn msc_process_response_headers(_transaction: &Transaction, code: i32, protocol: String) -> i32 {
    // transaction.processResponseHeaders(code, protocol);
    1
}

fn msc_process_response_body(_transaction: &Transaction) -> i32 {
    // transaction.processResponseBody()
    1
}

fn msc_process_logging(_transaction: &Transaction) -> i32 {
    // transaction.processLogging()
    1
}

//  int main (int argc, char **argv)
 fn main() {
    let mut ret: i32;
    let mut error = String::new();
    let mut modsec: ModSecurity;
    let transaction: Transaction;
    let rules: RulesSet;

    // let binding = msc_init();
    modsec = msc_init();

    // Set info connect
    msc_set_connector_info(&mut modsec, "ModSecurity-test v0.0.1-alpha (Simple example on how to use ModSecurity API".to_string());

    rules = msc_create_rules_set();
    
    ret = msc_rules_add_file(&rules, &main_rule_uri, &mut error);
    if ret < 0 {
        println!("Problems loading the rules -- {}", error);

        msc_rules_cleanup(&rules);
        msc_cleanup(&modsec);
        return;
    }
    msc_rules_dump(&rules);
 
    ret = msc_rules_add_remote(&rules, "test".to_string(),
        "https://www.modsecurity.org/modsecurity-regression-test-secremoterules.txt".to_string(),
        &error);
    if ret < 0 {
        println!("Problems loading the rules -- {}", error);
        
        msc_rules_cleanup(&rules);
        msc_cleanup(&modsec);
        return;
    }
    msc_rules_dump(&rules);

    transaction = msc_new_transaction(&modsec, &rules, "NULL".to_string());

    msc_process_connection(&transaction, "127.0.0.1".to_string(), 12345, "127.0.0.1".to_string(), 80);
    msc_process_uri(&transaction,
        "http://www.modsecurity.org/test?key1=value1&key2=value2&key3=value3".to_string(),
        "GET".to_string(), "1.1".to_string());
    msc_process_request_headers(&transaction);
    msc_process_request_body(&transaction);
    msc_process_response_headers(&transaction, 200, "HTTP 1.3".to_string());
    msc_process_response_body(&transaction);
    msc_process_logging(&transaction);
}
 
 
 