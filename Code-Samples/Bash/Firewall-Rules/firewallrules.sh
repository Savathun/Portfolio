#!/bin/sh

sudo systemctl start ssh

iptables -F   

sudo iptables -A INPUT -i eth0 -p tcp --dport 8008 -m state --state NEW,ESTABLISHED -j ACCEPT
iptables -A OUTPUT -o eth0 -p tcp --sport 8008 -m state --state ESTABLISHED -j ACCEPT

iptables -A INPUT -p tcp --dport 22 -m conntrack --ctstate NEW,ESTABLISHED -j ACCEPT
iptables -A OUTPUT -p tcp --sport 22 -m conntrack --ctstate ESTABLISHED -j ACCEPT

iptables -A INPUT -i eth0 -p udp --sport 53 -m state --state ESTABLISHED -j ACCEPT
iptables -A OUTPUT -o eth0 -p udp --dport 53 -m state --state NEW,ESTABLISHED -j ACCEPT
iptables -A INPUT -i eth0 -p tcp --sport 53 -m state --state ESTABLISHED -j ACCEPT
iptables -A OUTPUT -o eth0 -p tcp --dport 53 -m state --state NEW,ESTABLISHED -j ACCEPT


iptables -A OUTPUT -j DROP

iptables -A INPUT -p icmp --icmp-type echo-request -j ACCEPT
iptables -A OUTPUT -p icmp --icmp-type echo-reply -j ACCEPT
iptables -A INPUT -p icmp -j DROP
iptables -A OUTPUT -p udp -o eth0 --dport 53 -j ACCEPT
iptables -A INPUT -p udp -i eth0 --sport 53 -j ACCEPT
iptables -A INPUT -p udp -j DROP
iptables -A INPUT -p tcp -j DROP


iptables -A INPUT -s 10.0.0.0/8 -j DROP  
iptables -A FORWARD -s 10.0.0.0/8 -j DROP
iptables -A OUTPUT -s 10.0.0.0/8 -j DROP

iptables -A INPUT -s 0.0.0.0 -j DROP  
iptables -A FORWARD -s 0.0.0.0 -j DROP
iptables -A OUTPUT -s 0.0.0.0 -j DROP

iptables -A INPUT -s localhost -j DROP  
iptables -A FORWARD -s localhost -j DROP
iptables -A OUTPUT -s localhost -j DROP


iptables -A INPUT -s 255.255.255.255 -j DROP  
iptables -A FORWARD -s 255.255.255.255 -j DROP
iptables -A OUTPUT -s 255.255.255.255 -j DROP

iptables -A INPUT -j DROP
iptables -A FORWARD -j DROP
iptables -A OUTPUT -j DROP


MACHINEIP=$(hostname -I) && grep -rl 'HOME_NET any' /etc/snort/snort.conf | xargs sed -i "s%HOME_NET any%HOME_NET $MACHINEIP%g"


/etc/snort/snort.conf | sed -i 's%HOME_NET any%HOME_NET $(hostname -I)%g'

echo 'alert tcp $EXTERNAL_NET any -> $HOME_NET $HTTP_PORTS (msg:"Volex - Possible CVE-2014-6271 bash Vulnerability Requested (header) "; flow:established,to_server; content:"() {"; http_header; sid:31975;)

alert tcp $EXTERNAL_NET any -> $HOME_NET $HTTP_PORTS (msg:"OS-OTHER Bash CGI environment variable injection attempt"; flow:to_server,established; content:"%3D%28%29+%7B"; fast_pattern:only; metadata:policy balanced-ips drop, policy security-ips drop, ruleset community, service http; reference:cve,2014-6271; reference:cve,2014-7169; classtype:web-application-activity; sid:31975; rev:3;)

alert tcp $EXTERNAL_NET any -> $HOME_NET $HTTP_PORTS (msg:"OS-OTHER Bash CGI environment variable injection attempt"; flow:to_server,established; content:"() {"; fast_pattern:only; http_client_body; metadata:policy balanced-ips drop, policy security-ips drop, ruleset community, service http; reference:cve,2014-6271; reference:cve,2014-7169; classtype:web-application-activity; sid:31976; rev:3;)

alert tcp $EXTERNAL_NET any -> $HOME_NET $HTTP_PORTS (msg:"OS-OTHER Bash CGI environment variable injection attempt"; flow:to_server,established; content:"() {"; fast_pattern:only; http_uri; metadata:policy balanced-ips drop, policy security-ips drop, ruleset community, service http; reference:cve,2014-6271; reference:cve,2014-7169; classtype:web-application-activity; sid:31977; rev:3;)

alert tcp $EXTERNAL_NET any -> $HOME_NET $HTTP_PORTS (msg:"OS-OTHER Bash CGI environment variable injection attempt"; flow:to_server,established; content:"() {"; fast_pattern:only; http_header; metadata:policy balanced-ips drop, policy security-ips drop, ruleset community, service http; reference:cve,2014-6271; reference:cve,2014-7169; classtype:web-application-activity; sid:31978; rev:3;)

alert udp $HOME_NET 67 -> $HOME_NET 68 (msg:"OS-OTHER Malicious DHCP server bash environment variable injection attempt"; flow:stateless; content:"() {"; fast_pattern:only; content:"|02 01 06 00|"; depth:4; metadata:policy balanced-ips drop, policy security-ips drop, ruleset community, service dhcp; reference:cve,2014-6271; reference:cve,2014-7169; classtype:attempted-admin; sid:31985; rev:3;)

alert tcp $EXTERNAL_NET any -> $HTTP_SERVERS $HTTP_PORTS (msg:"WEB-CGI HyperSeek hsx.cgi directory traversal attempt"; flow:to_server,established; uricontent:"/hsx.cgi"; content:"../../"; content:"%00"; distance:1; reference:bugtraq,2314; reference:cve,2001-0253; reference:nessus,10602; classtype:web-application-attack; sid:803; rev:11;) 

alert tcp any any -> any any (msg:"Drectory Traversal, Any Any"; flow:to_server,established; content:"../../"; sid:803; rev:1;)

alert tcp any any -> $HOME_NET any  (msg:"Nmap XMAS Tree Scan"; flags:FPU,12; sid:1000006; rev:1; )

alert tcp any any -> $HOME_NET any (msg:"Nmap FIN Scan"; flags:F,12; sid:1000008; rev:1;)

alert icmp any any -> $HOME_NET any (msg: "NMAP ping sweep Scan"; dsize:0; sid:10000004; rev: 1;)

alert udp any any -> $HOME_NET any (msg: "NMAP UDP Scan"; sid:10000010; rev:1; )

alert tcp any any -> any any (msg:"SCAN NULL"; flow:stateless; ack:0; flags:0; seq:0; reference:arachnids,4; classtype:attempted-recon; sid:623; rev:6;)

alert tcp any any -> any 8008 (msg:"Brute Force Attempt"; flow:established,to_server; content:"Login or password incorrect"; http_client_body; nocase; sid:1000001; rev:1;)

alert tcp any any -> any any (content:"PASS"; isdataat:50,relative; content:!"|0a|"; within:50; sid:1000005;)

alert tcp any any -> any any (msg:"XSS raw content"; flow:to_server,established; content:"<script>"; sid:1000007;)

alert tcp any any -> any any (msg:"NII Cross-Site Scripting attempt";flow:to_server,established; pcre:"/((\%3C) <)((\%2F) \/)*[a-z0-9\%]+((\%3E) >)/i"; classtype:Web-application-attack; sid:9000; rev:5;)' | cat >> /etc/snort/rules/custom.rules
