<?php3
  $Temperatur = $_GET["t"];
  $datei = fopen("daten.txt","w");
  echo fwrite($datei,$Temperatur,100);
  fclose($datei);
?>
