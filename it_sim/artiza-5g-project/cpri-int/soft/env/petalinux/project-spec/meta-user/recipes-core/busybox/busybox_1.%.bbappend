do_configure_append() {
 sed -e 's/CONFIG_FEATURE_IFUPDOWN_EXTERNAL_DHCP=y/# CONFIG_FEATURE_IFUPDOWN_EXTERNAL_DHCP is not set/' -i .config
 sed -e 's/CONFIG_UDHCPC=y/# CONFIG_UDHCPC is not set/' -i .config
 sed -e 's/CONFIG_UDHCPC6=y/# CONFIG_UDHCPC6 is not set/' -i .config
 sed -e 's/CONFIG_UDHCPD=y/# CONFIG_UDHCPD is not set/' -i .config
 sed -e 's/CONFIG_DHCPD_LEASES_FILE/# CONFIG_DHCPD_LEASES_FILE is not set/' -i .config
 sed -e 's/CONFIG_FEATURE_UDHCPC_ARPING=y/# CONFIG_FEATURE_UDHCPC_ARPING is not set/' -i .config
 sed -e 's/CONFIG_FEATURE_UDHCPC_SANITIZEOPT=y/# CONFIG_FEATURE_UDHCPC_SANITIZEOPT is not set/' -i .config
 sed -e 's/CONFIG_UDHCP_DEBUG=0/# CONFIG_UDHCP_DEBUG is not set/' -i .config
 sed -e 's/CONFIG_UDHCPC_DEFAULT_SCRIPT/# CONFIG_UDHCPC_DEFAULT_SCRIPT is not set/' -i .config
 sed -e 's/CONFIG_UDHCPC_SLACK_FOR_BUGGY_SERVERS=80/# CONFIG_UDHCPC_SLACK_FOR_BUGGY_SERVERS is not set/' -i .config
 sed -e 's/CONFIG_IFUPDOWN_UDHCPC_CMD_OPTIONS/# CONFIG_IFUPDOWN_UDHCPC_CMD_OPTIONS is not set/' -i .config

# sed -e 's/CONFIG_SYSLOGD=y/# CONFIG_SYSLOGD is not set/' -i .config
# sed -e 's/CONFIG_FEATURE_ROTATE_LOGFILE=y/# CONFIG_FEATURE_ROTATE_LOGFILE is not set/' -i .config
# sed -e 's/CONFIG_FEATURE_REMOTE_LOG=y/# CONFIG_FEATURE_REMOTE_LOG is not set/' -i .config
# sed -e 's/CONFIG_FEATURE_SYSLOGD_DUP=y/# CONFIG_FEATURE_SYSLOGD_DUP is not set/' -i .config
# sed -e 's/CONFIG_FEATURE_SYSLOGD_CFG=y/# CONFIG_FEATURE_SYSLOGD_CFG is not set/' -i .config
# sed -e 's/CONFIG_FEATURE_IPC_SYSLOG=y/# CONFIG_FEATURE_IPC_SYSLOG is not set/' -i .config
# sed -e 's/CONFIG_LOGREAD=y/# CONFIG_LOGREAD is not set/' -i .config
# sed -e 's/CONFIG_FEATURE_LOGREAD_REDUCED_LOCKING=y/# CONFIG_FEATURE_LOGREAD_REDUCED_LOCKING is not set/' -i .config
# sed -e 's/CONFIG_FEATURE_KMSG_SYSLOG=y/# CONFIG_FEATURE_KMSG_SYSLOG is not set/' -i .config
}
