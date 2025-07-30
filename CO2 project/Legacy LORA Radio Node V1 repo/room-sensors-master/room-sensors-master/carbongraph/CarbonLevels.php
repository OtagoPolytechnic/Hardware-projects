<?php
	$host = "FILLMEIN";
	$userMS = "FILLMEIN";
	$passwordMS = "FILLMEIN";
	$database = "things";
        
    // Establish connection to DB server
    try 
    {
        $pdo = new PDO("mysql:host=$host;dbname=$database", $userMS, $passwordMS);
        $pdo->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
        $pdo->exec('SET NAMES "utf8"');
    }

    catch (PDOException $e)
    {
        $error = 'Connection to the host failed!';
        include 'error.html.php';
        exit();
    }

    try
    {
        $selectString = "SELECT * FROM data WHERE dev_id = \"c02_01\"";
        $posts = $pdo->query($selectString);       // Verification selection
    }
    catch (PDOException $e)
    {
        $error = 'Select statement error';
        include 'error.html.php';
        exit();
    }

    $rowcount =  0;
    $total = 0;
    $highest = 0;
    $lowest = 100;
?>

<html>
  <head>
    <script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
    <script type="text/javascript">
      google.charts.load('current', {'packages':['corechart']});
      google.charts.setOnLoadCallback(drawChart);

      function drawChart() {
        var data = google.visualization.arrayToDataTable([
          ['Time', 'Carbon Dioxide Level'],
			 <?php
				foreach($posts as $row)
				{
					$payloadhex = base64_decode($row['payload']);
					$payloadstring = bin2hex($payloadhex);
					$payloadchopped = substr($payloadstring, 0, 4);
					$v = hexdec($payloadchopped);
					
					$rowcount++;
					$total = $total + $v;
					$date=date_create("$row[time]");
					$fmdt=date_format($date,"H:i d/m");
					echo("\n['$fmdt', $v],");
					  
					if ($v > $highest)
					{
						$highest = $v;
					}
					else{}

					if ($v < $lowest)
					{
						$lowest = $v;
					} 
				}
					
				$avg = $total / $rowcount;
				$avg = round($avg,3);
			?>
        ]);

        var options = {
          title: 'Carbon Dioxide Levels (7 Day Overview)',
          curveType: 'function',
          legend: { position: 'bottom' },
          vAxis: {minValue: 10, maxValue: 15}
        };

        var chart = new google.visualization.LineChart(document.getElementById('curve_chart'));

        chart.draw(data, options);
      }
    </script>
  </head>
  <body>
    <div id="curve_chart" style="width: 100%; height: 100%"></div>

    <h3>Overall Statistics</h3>
    <p>Highest Level: <?php echo("$highest");?></p>
    <p>Lowest Level: <?php echo("$lowest");?></p>
    <p>Average Level: <?php echo("$avg");?></p>
	<p>Rowcount: <?php echo("$rowcount");?></p>
  </body>
</html>
