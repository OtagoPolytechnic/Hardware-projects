<html>
  <head>
    <script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
    <script type="text/javascript">
      google.charts.load('current', {'packages':['corechart']});
      google.charts.setOnLoadCallback(drawChart);

      function drawChart() {
        var data = google.visualization.arrayToDataTable([
          ['Time', 'Carbon Dioxide Level (ppm)'],
			 <?php
				foreach($posts as $row)
				{
          $ppm = $row['ppm'];
					$date=date_create("$row[pTime]");
					$fmdt=date_format($date,"H:i d/m");
					echo("\n['$fmdt', $ppm],");
				}
			?>
        ]);

        var options = {
          title: 'Co2 Levels (<?php echo($time); ?> Overview)',
          legend: { position: 'bottom' },
          vAxis: {minValue: 300, maxValue: 3000}
        };

        var chart = new google.visualization.LineChart(document.getElementById('curve_chart'));

        chart.draw(data, options);
      }
    </script>
  </head>
  <body>
    <div id="curve_chart" style="width: 100%; height: 100%"></div>
  </body>
</html>