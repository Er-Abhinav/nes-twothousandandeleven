Meetings jeden Mo. 09:30 ab dem 07.11.2011 möglich<br />
http://ti.tuwien.ac.at/rts/teaching/courses/networked-embedded-systems

# Thema #
# Projektthema #
  * Entwicklungsprozess ist auch ok
  * Ist das mit dem Board realisierbar?
  * 6ECTS sollten hin kommen
  * Muss keine hoch komplexe Sache am uC Board sein
# Spezialisierung #
= Gruppenthema<br>
Uhrensync, Fehlertoleranz (TMR), Mathematische Beweise, ...<br>
z.B.:<br>
<ul><li>Projektentwicklug -> Schwerpunkt Projekt komplett durch einen Entwicklungszyklus führen</li></ul>

<h1>Ablauf</h1>
<h2>Abstract</h2>
Erarbeiten:<br>
<ul><li>Was wollen wir machen<br>
<ul><li>Was ist dabei zu tun?<br>
</li></ul></li><li>Wie soll das ablaufen<br>
<ul><li>Aufgabenoptimiert (Rad nicht neu erfinden)<br>
</li></ul></li><li>Projektplan (Gantt Chart)<br>
<ul><li>Arbeitspakete (Doku, Visualisierung, Regelung) Thematische Sachen  (Ohne Anfang und Ende)<br>
</li><li>Tasks (Definierter Start und Ende, [Urlaub, Pausen])<br>
<ul><li><b>Tasks haben subprodukte<br>
</li></ul></li><li>Meilensteine (Extern das Projekt beobachtbar)<br>
<ul><li></b> Zeitpunkte für fixe Fortschrittspunke<br>
</li><li><b>1. Spezifikation Fertig (erste Workshop day)<br>
</li><li></b> 2. zweiter Workshop day, alles fertig</li></ul></li></ul></li></ul>

<h2>Doku</h2>
Output als pdf<br>
<ul><li>Fünf Einzelteile schon auf der Homepage<br>
<h2>Workshop days</h2>
Datum 1.WorkSD: 3.11.2011<br>
</li><li>Codeaustausch ist auch begrenzt ok<br>
</li><li>Vorstellung des eigenen Projekts<br>
<ul><li>Was machen wir und die Anderen?<br>
</li></ul></li><li>Vorstellung des Gruppenthemas<br>
</li><li>30min Zeit/Gruppe<br>
<h3>Benefit</h3>
</li><li>Was machen die Anderen<br>
</li><li>Untschiedliche Ansätze kennenlernen</li></ul>

<hr />

<h1>1. Workshop Day</h1>
20 - 25 Slides<br>
<br>
Es ist ein Übergang vom Projekt(thema) zum Gruppethema zu wählen:<br>
<ul><li>Übergang<br>
<ul><li>Vom Projekt zum Gruppethema<br>
</li><li>Vom Gruppethema zum Projekt<br>
</li><li>Beide komplett trennen<br>
Folien Inhalt<br />
Folgendes muss am ersten Workshop day präsentiert werden</li></ul></li></ul>

<h2>Projektidee</h2>
Folgendes gehört im Projekt gemacht; was ist unsere Idee dabei<br>
<ul><li>Realisierbarkeit / Aufwandsabschätzung<br>
</li><li>Günstiges Busprotokoll für Automation im Semi-Profi Bereich<br>
</li><li>Einarbeitungszeit/Migrationsaufwand soll gering sein<br>
</li><li>Geringe Sicherheitsmaßnahmen</li></ul>

<h2>Gruppenthema/Spezialisierung</h2>
<ul><li>Spezifizieren eines geeigneten Kommunikationsprotokolls für die später besprochenen Bus-Eigenschaften<br>
</li><li>Definition geeigneter Abstraktionslayer um einfache Erweiterbarkeit und modulorientierte Entwicklung zu gewährleisten.<br>
</li><li>Uhrensynchronisation</li></ul>

<h3>Wichtiges</h3>
<h3>Problemstellungen und Lösungen</h3>
<h2>Time plan</h2>
<ul><li>Mitte Oktober: Project outline including project idea, time plan, milestones, roles<br>
</li><li>Ende Oktober: Spezifikation und Design des Busprotokolls<br>
</li><li>Ende November: Spezifikation und Design der Steuerapplikation<br>
</li><li>Weihnachten: Implementierung und Validierung des Busprotokolls<br>
</li><li>Ende Jänner: Implementierung und Validierung der Steuerapplikation<br>
<h2>Gantt Chart</h2>
<h3>Milestones</h3>
</li><li>Fertigstellung Pflichtenheft (Need-to-Haves, Nice-to-Haves)<br>
</li><li>Fertigstellung des Bus Protokolls<br>
</li><li>Fertigstellung der Demo-Application Nodes<br>
</li><li>Fertigstellung der Dokumentation<br>
</li><li>Abschlusspräsentation<br>
<h3>Activities</h3>
<h3>Tasks</h3>
</li><li>Project Repository einrichten / Entwicklungsumgebung einrichten<br>
</li><li>Technische Spezifikation erstellen<br>
</li><li>Pflichtenheft (Need-to-Haves, Nice-to-Haves)<br>
</li><li>Implementierung des Bus Protokolls<br>
</li><li>Implementierung einzelner Application Nodes<br>
</li><li>Dokumentation<br>
</li><li>Testing<br>
<h3>SubTasks</h3>
<h2>Rollenverteilungen</h2>
</li><li>Project Manager ... Robert<br>
</li><li>Technischer Manager ... Alexanda<br>
</li><li>Doku Manager ... Nick<br>
<blockquote>(Rechtschreibung, etc. aber jeder schreibt selbst Doku)<br>
<h2>Kommunikationsprotokoll</h2>
<h3>Claims / Anfoderungen</h3>
</blockquote></li><li>Günstig<br>
</li><li>Einfach zu verwenden (für Hobbyisten/Bastler)<br>
</li><li>Geringer Resourcenbedarf<br>
<h1>Ideen</h1>
<h2>Protokoll Eigenschaften</h2>
Was soll es können:<br>
<pre><code>Difinitionen bitte so angeben: |&lt;Vorname[0]&gt; | &lt;Priorität(Low...High) | &lt;Eigenschaft&gt;|<br>
</code></pre></li></ul>

<table><thead><th> N </th><th> Mid </th><th> Checksum response ->  (CRC, Retransmission, Fehlertoleranz)</th></thead><tbody>
<tr><td> N </td><td> Low </td><td> Automatische Geschwindigkeitswahl ODER Geschwindigkeitsstufen </td></tr>
<tr><td> N </td><td> High </td><td> 500m Reichweite </td></tr></tbody></table>

<ul><li>Repeated-Retransmission (ID,Daten,Repeat,Daten,Repeat,Daten)<br>
</li><li>A | High Multicast/Broadcast<br>
<ul><li>Angebotene Services<br>
</li><li>Servicegruppen<br>
</li><li>Automatische (optionale) An-/Abmeldung vom Netzwerk (mit Refresh)<br>
</li></ul></li><li>Authentizität (Secrete on bus)<br>
</li><li>Krypto<br>
</li><li>Echtzeitfähigkeit (Uhrensync)<br>
</li><li>Beides: Echtzeit und Asynchrone Komm. möglich?<br>
</li><li>Reichweite/Latenzzeit<br>
</li><li>großteils selbstkonfigurierend (autodetect einzelner nodes und services)<br>
</li><li>robust<br>
</li><li>4-layered:<br>
<ul><li>application layer<br>
</li><li>service layer<br>
<ul><li><b>Note: app layer lt. osi = ( app layer + service layer)<br>
</li><li></b> Service definiert in welcher Art und Weise die Messages aufgebaut werden für verschiedene Applikationen<br>
</li><li><b>Service legt fest welche Fähigkeiten einzelne Nodes haben<br>
</li><li></b> Service ermöglicht Gruppenbildung mehrerer Nodes<br>
</li></ul></li><li>data link layer layer (Bus-Protokoll-Spezifisch)<br>
</li><li>physical layer (Bus-Protokoll-Spezifisch)</li></ul></li></ul>

<h2>random shit</h2>
<ul><li>one wire<br>
</li><li>i2c nutzen<br>
</li><li>uart nutzen<br>
</li><li>time triggered<br>
</li><li>autoconfig (nodes dazustöpseln)<br>
</li><li>master-slave (master könnte auch als kleiner avr zb attiny implementiert werden)<br>
</li><li>slave library für avr bauen</li></ul>

<h1>Protokoll</h1>
<h2>Physical Layer</h2>
encoding<br>
<ul><li>rz (none return to 0) <br />fehlerekrennung/korrektur<br>
</li><li>arity<br>
<ul><li>pro:<br>
<ul><li><b>einfach zu realisieren<br>
</li><li></b> bei uart quasi geschenkt<br>
</li></ul></li><li>contra:<br>
<ul><li><b>retransmission benötigt da man keine fehlerkorrektur auf  empfängerseite realisieren kann<br>
</li></ul></li></ul></li><li>crc<br>
<ul><li>pro:<br>
<ul><li></b> rückgewinnung<br>
<h2>Statische Slots</h2>
</li></ul></li></ul></li><li>Wann darf letzte Nachricht gesendet werden<br>
<ul><li>Statischen Nodes kümmern sich darum<br>
</li></ul></li><li>Anmeldung mit Dynamischer Adresse<br>
<ul><li>Anmeldung nach einer Zyklendauer möglich</li></ul></li></ul>

<h1>2. Workshop Day</h1>

<ul><li>Verwendete Software<br>
<ul><li>Vorteile / Nachteile<br>
</li><li>Ist Bestandsaufnahme