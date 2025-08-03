
<?php
date_default_timezone_set('NZ');

include 'sql.inc.php';

if ($_GET['dev'])
{
    $device = $_GET['dev'];       // Gathers user input
    strip_tags($device);             // Strips any tags from user input (Security)

    try
    {
        $selectString = "SELECT * FROM ppm WHERE devID = '".$device."' ORDER BY pTime DESC LIMIT 1";
        $data = $pdo->query($selectString);       // Verification selection
    }

    catch (PDOException $e)
    {
        $error = 'Select statement error';
        include 'error.html.php';
        exit();
    }

    $output = '<div id="chart_div" style="width: 400px; height: 400px;"></div>';
}

else
{
    $output = 'No device specified!';
}
?>

<html>
    <head>
    <meta http-equiv="refresh" content="300">
    <script type="text/javascript" src="https://www.gstatic.com/charts/loader.js"></script>
    <script type="text/javascript">
      google.charts.load('current', {'packages':['gauge']});
      google.charts.setOnLoadCallback(drawChart);

      function drawChart() {

        var data = google.visualization.arrayToDataTable([
          ['Label', 'Value'],
          ['Co2 Level', <?php foreach($data as $row) { echo($row['ppm']); }?>]
        ]);

        var options = {
          width: 400, height: 400,
          greenFrom:400, greenTo:1000,
          yellowFrom:1000, yellowTo: 1500,
          redFrom: 1500, redTo: 3000,
          minorTicks: 10,
          min:400, max:3000
        };

        var chart = new google.visualization.Gauge(document.getElementById('chart_div'));

        chart.draw(data, options);
      }
    </script>
    </head>


    <body>
        <?php echo($output) ?>
    </body>

</html>
