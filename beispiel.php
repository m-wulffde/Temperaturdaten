<!DOCTYPE HTML>
<html>
    <head>
        <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <title>Temperatur www.M-Wulff.de</title>
        <style type="text/css">
        </style>
    </head>
    <body>
<script src="code/highcharts.js"></script>
<script src="code/modules/series-label.js"></script>
<script src="code/modules/exporting.js"></script>

<div id="container" style="min-width: 310px; height: 400px; margin: 0 auto"></div>
<script type="text/javascript">
    Highcharts.chart('container', {
        chart: {                            // Chart-Typ
            type: 'spline'                                        
        },
        title: {                            // Überschrift
            text: 'Temperatur Garten'
        },
        subtitle: {                         // Unterüberschrift
            text: 'die letzten 7 Tage'                
        },
        xAxis: {                            // X-Achse Zeitachse
            type: 'datetime',
            labels: {
                overflow: 'justify'
            }
        },
        yAxis: {                            // Y-Achse Beschriftung (Höhe / Links)
            title: {
                text: 'Temperatur (°C)'
            }
        },
        tooltip: {
            valueSuffix: ' °C'
        },
        plotOptions: {
            spline: {
                lineWidth: 4,
                states: {
                    hover: {
                        lineWidth: 5
                    }
                },
                marker: {
                    enabled: false
                },
                pointInterval: 3600000, // Alle Stunde ein Wert
            //  pintStart: Date.UTC(2018, 1, 15, 21, 0, 0)  // Datumsformat m für Monat zählt von 0 an.
                pointStart: Date.UTC(<?php  echo date('Y,m-1,d,H,0,0', time()-((60*60*24*7) - (60*60))); ?>)
            }
        },
        credits: {
            enabled: false
        },
        series: [{
            name: 'Garten',
         // data: [0.2, 0.8, 0.8, 20, 1, -1.3, 1.5, 2.9, 1.9, 2.6, 1.6, 3, 4, 3.6, 4.5, 4.2, 4.5, 4.5, 4, 3.1]
            data: [<?php include 'daten.php';?>]

//      }, {      // Weitere Linie
//          name: 'Büro',
//          data: [0, 0, 0.6, 0.9, 0.8, 0.2, 0, 0, 0, 0.1, 0.6, 0.7, 0.8, 0.6, 0.2, 0, 0.1, 0.3, 0.3, 0, 0.1]
        }],
            navigation: {
                menuItemStyle: {
                    fontSize: '10px'
                }
            }
        });
        </script>
    </body>
</html>
