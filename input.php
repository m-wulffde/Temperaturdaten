?php
// Messwert in Textdatei schreiben
$Temperatur = $_GET["t"];
$datei = fopen("daten.txt","w");
fwrite($datei, $Temperatur,100);
fclose($datei);

// Daten aufbereiten
$datum = date("Y-m-d H:i:s");

// Mysqldatenbank öffnen
$mysqli = new mysqli("localhost", "user", "password", "database");

if ($mysqli->connect_errno) {
    die("Verbindung fehlgeschlagen: " . $mysqli->connect_error);
}
// Messwert in MySQL-Datenbank schreiben
$sql = "INSERT INTO `messwerte` (`DatumZeit`, `Temp`) VALUES ('$datum', '$Temperatur')";
$mysqli->query($sql);
?>
