<?php
// Kontrolle ob die Temparatur-Datei noch beschrieben wird
$Temp_Datei = 'daten.txt'; // Achtung: Ordner mit angeben.
If(intval(date("H", time() - filectime($Temp_Datei))) > 2){
  // Die Datei ist äter als 2 Stunden, Fehlermeldung per Mail
  $empfaenger = "empf@domain.de";
  $betreff = "Fehlermeldung: Temperatursensor";
  $from = "From: Matthias Wulff <noreplay@domain.de>";
  $text = "Fehlermeldung: Die Datei daten.txt ist zu ALT, Bitte Tempaturerfassung kontrolieren!";
  mail($empfaenger, $betreff, $text, $from);
  touch($Temp_Datei); // Dateizeit aktualisieren, nur eine Mail pro Stunde
}
?>
