{{% set date 2013 Aug 11 %}}

<h1>Undocumented EdgeOS commands</h1>

<p>There are some hidden utilities in Ubiquiti EdgeOS that are often used in its scripts but aren't documented anywhere.
   Here's a non-exhaustive (and probably even inaccurate) list of the hidden commands.</p>
<p>There is probably no reason to use them.</p>
<p>This list was compiled by reading the scripts, I don't have access to the source code of these binaries.</p>

<h2>/usr/sbin/ubnt-hal</h2>
<table>
  <tr>
    <th> <strong>Option</strong> </th>
    <th> <strong>Arguments</strong></th>
    <th> <strong>Description</strong></th>
  </tr>
  <tr>
    <td>getBoardName</td>
    <td>None</td>
    <td>Prints symbolic board name (e.g. "EdgeRouter Lite 3-Port")</td>
  </tr>
  <tr>
    <td>show-version</td>
    <td>None</td>
    <td>Prints system version information ("show version" does exactly that)</td>
  </tr>
  <tr>
    <td>getBoardId</td>
    <td>None</td>
    <td>Prints board identifier (e.g. "e102" for ERL)</td>
  </tr>
  <tr>
    <td>hasPoe</td>
    <td>None</td>
    <td>Exits with 0 if platform support PoE, exists with 1 otherwise (additionally prints 0 if PoE supported, 1 otherwise)</td>
  </tr>
  <tr>
    <td>getPortCount</td>
    <td>None</td>
    <td>Prints ethernet port count</td>
  </tr>
  <tr>
    <td>input48v</td>
    <td>None</td>
    <td>Exits with 0 and prints 1 if 48 volt PoE is supported (the opposite if not)</td>
  </tr>
  <tr>
    <td>wdGetStats</td>
    <td>Interface name or number (?)</td>
    <td>Prints PoE watchdog statistics</td>
  </tr>
</table>


<h2>/usr/sbin/ubnt-hal-e</h2>
<table>
  <tr>
    <th> <strong>Option</strong> </th>
    <th> <strong>Arguments</strong></th>
    <th> <strong>Description</strong></th>
  </tr>
  <tr>
    <td>getBoardIdE</td>
    <td>None</td>
    <td>Same to "ubnt-hal getBoardIdE"</td>
  </tr>
  <tr>
    <td>showEthPhysicalE</td>
    <td>Interface name (e.g. eth0) or number (e.g. 0)</td>
    <td>Show physical port information (link status, speed etc.)</td>
  </tr>
  <tr>
    <td>onSwitch</td>
    <td>None</td>
    <td>Exits with 0 and prints 1 if the interface is a switch port (the opposite if not)</td>
  </tr>
  <tr>
    <td>allSwitchPortsSwitchedE</td>
    <td>None</td>
    <td>Returns 0 if all switch ports are in switch mode, 1 otherwise</td>
  </tr>
</table>

<h2>/usr/sbin/ubnt-ifctl</h2>
<table>
  <tr>
    <th> <strong>Option</strong> </th>
    <th> <strong>Arguments</strong></th>
    <th> <strong>Description</strong></th>
  </tr>
  <tr>
    <td>set-poe</td>
    <td>interface name, value (24v|48v|off)</td>
    <td>Sets PoE</td>
  </tr>
  <tr>
    <td>show-poe-status</td>
    <td>None</td>
    <td>Shows PoE status</td>
  </tr>
  <tr>
    <td>list-sys-intfs</td>
    <td>Interface type (e.g. ethernet, bridge, switch, vif etc.) or "all"</td>
    <td>Prints list of interfaces of specified type</td>
  </tr>
  <tr>
    <td>list-non-switch-intfs</td>
    <td>None</td>
    <td>Returns a list of non-switch interfaces</td>
  </tr>
  <tr>
    <td>show-switch</td>
    <td>None</td>
    <td>Shows switch information</td>
  </tr>
  <tr>
    <td>valid-mac</td>
    <td>MAC address</td>
    <td>Exits with 0 if the argument is a valid MAC address, 1 otherwise (also prints error message)</td>
  </tr>
  <tr>
    <td>valid-intf</td>
    <td>Interface name, interface type (e.g. "eth0 ethernet")</td>
    <td>Exits with 0 if interface name is a valid and existing interface of that type</td>
  </tr>
  <tr>
    <td>check-non-switch-intf</td>
    <td>Interface name</td>
    <td>Returns 0 is given interface is not a switch port</td>
  </tr>
  <tr>
     <td>eth-begin</td>
     <td>?</td>
     <td>?</td>
  </tr>
  <tr>
    <td>eth-end</td>
    <td>?</td>
    <td>?</td>
  </tr>
  <tr>
    <td>eth-commit</td>
    <td>?</td>
    <td>?</td>
  </tr>
</table>
