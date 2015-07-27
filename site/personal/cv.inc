{% set date 2015 July 28 %}

<h1>Curriculum vitae</h1>

<p>When you don't focus on just one field, your CV becomes a mess and needs more and more tailoring
   for particular offers. This is an attempt to write a generic CV that reflects both programming and
   network administration sides of my career.</p>

<h2>What I can do</h2>

<h3>Programming</h3>

<p>Most of my public code is at <a href="http://github.com">github.com/dmbaturin</a>. VyOS, the biggest project
   I maintain, is at <a href="http://github.com/vyos">github.com/vyos</a>, although most of the code is
   inherited from Vyatta and is not indicative of my personal skills (maybe the size is indicative of my
   ability to work with large code bases).</p>

<h4>General</h4>
<p>Familiar with the basics of algorithmic complexity. At least I can tell O(n) from O(exp N) and know
   how hard are lookups/updates in common datastructures.</p>
<p>Familiar with concurrency primitives and problems they are supposed to solve (although I'm by no means
   a concurrency expert).</p>
<p>What I have little or no experience with: parallel programming (used MPI in Fortran90 toy programs),
   programming for mobile devices, Windows programming.</p>

<h4>Languages</h4>
<p>Wrote useful programs in Python, OCaml, Perl, C, Ada.</p>
<p>Wrote toy programs in Haskell, Scheme, StandardML, Fortran.</p>
<p>JavaScript. I use it for DOM manipulation, but I'm not familiar with any of the
   popular libraries, so it's safe to say that I don't know it.</p>
<p>Ah, yes, Bourne shell. I don't know if it counts, but I wrote a lot of it.</p>
<p>I also wrote in PHP, but I don't want to do it anymore.</p>

<h4>Web programming</h4>
<p>I wrote websites/applications back then, but I haven't done it in a long time,
   and I know things changed. If I get into it again, I'll have a lot to catch up with.</p>

<h4>Relational databases</h4>
<p>Worked with SQLite, MySQL, and PostreSQL.</p>
<p>I can insert/update and select data (and also group and filter it with WHERE, LIKE, HAVING etc.).</p>
<p>I can design simple database schemas and normalize them (I rarely go beyond the second normal form though).</p>
<p>If the database is going to be complex, or big, or both, I'll leave it to someone else.
   I also have very little experience with stored procedures and triggers.</p>

<h4>Tools and processes</h3>
<p>Active Git user. Used Subversion long time ago, also used Mercurial a little.</p>
<p>Understand why unit tests can be a good idea, used Python unittest module, OUnit, check,
   and some other unit test frameworks.</p>
<p>Made DEB and RPM packages.</p>
<p>Familiar with high level build tools such at GNU autotools and Oasis.</p>
<p>Used Jenkins for continuous integration.</p>
<p>Used Bugzilla, Mantis, and JIRA issue trackers.</p>
<p>Ran open source projects with more than one contributor.</p>

<h4>Courses</h4>

<p>Introduction to functional programming by Delft on edX. <a href="https://verify.edx.org/cert/efcc8689b4d8486484009b2fe7e8483f">Verify.</a></p>
<p>Programming Languages by the Univerisity of Washington on Coursera. <a href="https://www.coursera.org/account/accomplishments/records/P5M4pQu66mJUK8jg">Verify.</a>

<h3>Networking</h3>

<h4>General networking</h4>
<p>Can design and assess small and medium size networks and addressing plans (choose physical media, VPN protocols,
   routing protocols etc. to match the requirements).</p>
<p>Can diagnose network problems, use standard tools (traceroute, nmap, netcat, whois etc.), make and interpret
   traffic dumps with tcpdump/tshark.</p>
<p>Used IXIA Networks equipment for performance testing.</p>
<p>Have experience with IPv6 and dual stack setups, including router advertisment, DHCPv6, IPv6 BGP, DNS records and so on.</p>

<h4>Certifications</h4>

<ul>
  <li> Hurricane Electric <a href="https://ipv6.he.net/certification/scoresheet.php?pass_name=dmbaturin">IPv6 Sage</a> </li>
  <li> <a href="https://www.certmetrics.com/brocade/public/verification.aspx?code=PKFQ5FECCMV428P2">Brocade Certified vRouter Engineer 2013</a>. </li>
  <li> CCNA R&S 419440597336JSXM. <a href="http://www.ciscocertificates.com/verify.cfm">Verify.</a>. </li>
</ul>

<h4>Switched networks</h4>
<p>Worked with Cisco (Catalyst and Nexus), ProCurve, Dell PowerEdge, Force10, Brocade ethernet switches.</p>
<p>I can setup 802.1q VLANs, QinQ, link aggregation. I can setup spanning tree protocols when I have to
   (I prefer link aggregation whenever the topology allows).</p>
<p>Familiar with L2 security features such as private VLAN and MAC address policies.</p>
<p>What I have very little or no experience with: MPLS, optical network infrastructure (DWDM etc.),
   TDM and SDH networks, wireless networks (of any kind).</p>
<p>My favorite switch product line is Catalyst.</p>

<h4>Routed networks</h4>
<p>Worked with Vyatta/EdgeOS/VyOS, Cisco IOS, VMware vShield, OpenWRT, Brocade, and other routers/L3 switches.</p>
<p>Used OSPF, BGP, RIPv2 (a little). Wrote routing policies (route-maps in Cisco terms) to avoid route leaks
   and influence routing decisions (AS path prepend and so on).</p>
<p>Set up small autonomous systems from scratch (from RIPE application to configuring peering/transit sessions).</p>
<p>Used multiple VPN protocols and scenarios including IPsec, GRE/IPsec, OpenVPN in site-to-site and client-server modes,
   L2TP/IPsec and PPTP for remote users.</p>

<h4>Security</h4>
<p>Used multiple VPN protocols and scenarios including IPsec, GRE/IPsec, OpenVPN in site-to-site and client-server modes,
   L2TP/IPsec and PPTP for remote users. Set up IPsec between different vendors' equipment.</p>
<p>Configured firewall and NAT on multiple platforms.</p>
<p>What I have little or no experience with: DDoS mitigation (other than blackholing the client),
   application layer firewalls.</p>

<h3>Systems administration</h3>

<h4>UNIX</h4>
<p>Worked with RHEL/CentOS, Debian, FreeBSD, Solaris (a little), SCO (a little).</p>
<p>Can perform the usual tasks: install packages, configure services, add users and so on.</p>
<p>Write shell scripts routinely.</p>
<p>What I have little or no experience with: mandatory access control, unattended installation
   (I have some idea how to setup cobbler/koan, but not much more than that).</p>

<h4>Virtualization</h4>
<p>Used VMware, KVM, Xen (a little). Made OVFs for VMware and VirtualBox, used VMware
   OVF fields for appliance autoconfiguration.</p>
<p>Worked with Amazon Web Services (mostly EC2) a little.</p>

<h4>DNS</h4>
<p>Maintained authoritative and caching name serves based on BIND, both masters and slaves.
   Used split-horizon DNS. Have some idea how to setup autogenerated reverse zones.</p>
<p>What I have little or no experience with: DNSSEC.</p>

<h4>VoIP</h4>
<p>Configured FreeSWITCH for class4 and class5 deployments. Wrote FreeSWITCH scripts.</p>
<p>Familiar with typical voice provider operations (although I have little experience with 
   softswitch and billing systems integration).</p>
<p>Can interpret SIP traffic dumps.</p>

<h2>Where I worked</h2>

<h3>AVyS Telecom</h3>
<p><em>From December 2013</em></p>
<p>A VoIP transit provider.</p>
<p>Redesigned the company network. Set up a VPN between points of presence with BGP
   for internal routing (a mix of VyOS and EdgeOS routers).</p>
<p>Participated in deploying new points of presence, configured FreeSWITCH class4 setups.</p>
<p>Developed VoIP-related in-house software, mostly in Python.</p>

<h3>Vyatta inc., later acquired by Brocade, USA (remotely)</h3>
<p><em>December 2010 to September 2013</em></p>
<p>Vyatta is a Linux distro for routers/firewalls where all network functions are controlled from
   a single CLI in the same fashion as in hardware routers.</p>
<p>First time I was a full time programmer without systems/network admin responsibilities.</p>
<p>When I discovered Vyatta in 2008, I instantly became a huge fan and contributed to the community
   a lot, so eventually they offered me to work for them as a programmer.</p>
<p>Participated in NAT CLI redesign, added IPv6 support to IPsec, and made other improvements to the system.</p>

<h3>IT consultant</h3>
<p><em>From March 2009 to December 2010</em></p>
<p>Helped a few local companies to improve their systems and cut costs by migrating to free software.</p>

<h3>Soyuz-T, Tomsk, Russia</h3>
<p><em>From September 2008 to March 2009</em></p>
<p>A furniture store chain.</p>
<p>I turned out to be the sole Linux and VoIP admin there (even though I had no VoIP experience
   before and had to learn it myself), and one of the two network admins.</p>
<p>Configured Active Directory integration for Linux machines, class5 FreeSWITCH installation
   and VoIP phones, SIP to PSTN gateways. Managed a Cisco-based DMVPN setup that connected stores in multiple cities
   with the headquarters.</p>
<p>Developed company's web store and websites for some of its subsidiaries in PHP (based on Drupal).
   Managed to fix some bugs in obfuscated PHP code (the company who made their original website
   used their own proprietary CMS, and when they went defunct, there was no one to fix it).</p>
<p>Occasionally provided support to users, fixed cabling and, performed hardware installations/repairs.</p>

<h3>Promstroyneftegazproyekt, Tomsk. Russia</h3>
<p><em>From May 2008 to August 2008</em></p>
<p>A civil engineering company.</p>
<p>Temporary contract. A small office with about thirty workstations and a couple of servers.</p>
<p>Didn't break anything, kept the network running, provided support to users.</p>
<p>My main accomplishment there was made long after I quit. My successor quit after a couple of months and didn't tell
   his successor the password for the encrypted partition with sensitive data, so after server reboot access to it was lost.
   The successor couldn't contact him, but he contacted me and somehow I managed to remember that password
   months after I last used it.</p>

<h3>Tomsk Polytechnic University, Center for Technological Innovations, Tomsk, Russia</h3>
<p><em>From Septermber 2007 to May 2008</em></p>
<p>A department responsible for communication between researchers and industry representatives
   interested in commercializing their work.</p>
<p>A small office with just five workstations and one server/router. Reconfigured the network
   to use a public subnet provided by the university rather than a NATed private subnet.
   Configured a web proxy (Squid) to conserve bandwidth and block NSFW websites. Kept the network and the workstations running,
   provided technical support to users.</p>

