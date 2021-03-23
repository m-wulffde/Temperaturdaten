<html>
<head>
  <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
  <title>ESP - Messwerte</title>
</head>
<body>
 
<table>
  <tr>
    <td width="100"><b>Datum</b></td>
    <td width="70"><b>Zeit</b></td>
    <td width="120"><b>Temperatur</b></td>
  </tr>
 
<?php
// Mysqldatenbank öffnen
$mysqli = new mysqli("localhost", "user", "password", "database");
if ($mysqli->connect_errno) {
    printf("Connect failed: %s\n", $mysqli->connect_error);
    exit();
}
$Sortierung =  "ASC"; // Desc Absteigend, ASC Aufsteigend
$query = "
    SELECT DATE_FORMAT(datumzeit,'%d.%m.%Y') AS DATUM, DATE_FORMAT(datumzeit,'%H:%i') AS ZEIT, Temp 
    FROM `messwerte`
    WHERE `datumzeit` >= date_sub(now(), interval 24 hour) AND `datumzeit` <= now()
    GROUP BY DATE_FORMAT(datumzeit, '%Y-%m-%d %H')
    ORDER BY datumzeit $Sortierung
    ";
$result = $mysqli->query($query);  // Ausgabe des SQL-String

// Array erzeugen
while($row = $result->fetch_array()){
  $rows[] = $row;
}

// Array ausgeben
foreach($rows as $row)
{
  echo "<tr>\n";
  echo "  <td>".$row['DATUM']."</td>\n";
  echo "  <td>".$row['ZEIT']."</td>\n";
  echo "  <td>".$row['Temp']."</td>\n";
  echo "</tr>\n";
}
?>
    </table>
  </body>
</html>
