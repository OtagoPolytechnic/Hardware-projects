<!DOCTYPE html>
<html>
    <head>
        <meta name="viewport" content="width=device-width, initial-scale=1">
        <style>
        body {font-family: Arial, Helvetica, sans-serif;}
        * {box-sizing: border-box;}

        input[type=text], select, textarea {
        width: 100%;
        padding: 12px;
        border: 1px solid #ccc;
        border-radius: 4px;
        box-sizing: border-box;
        margin-top: 6px;
        margin-bottom: 16px;
        resize: vertical;
        }

        input[type=submit] {
        background-color: #4CAF50;
        color: white;
        padding: 12px 20px;
        border: none;
        border-radius: 4px;
        cursor: pointer;
        }

        input[type=submit]:hover {
        background-color: #45a049;
        }

        .container {
        border-radius: 5px;
        background-color: #f2f2f2;
        padding: 20px;
        }

        table {
        font-family: arial, sans-serif;
        border-collapse: collapse;
        width: 100%;
        }

        td, th {
        border: 1px solid #dddddd;
        text-align: left;
        padding: 8px;
        }

        tr:nth-child(even) {
        background-color: #dddddd;
        }
        </style>
    </head>
    <body>
        <h2>Overview</h2>

        <table>
        <tr>
            <th>Device</th>
            <th>Carbon Dioxide Value</th>
            <th>Temperature</th>
            <th>Last seen</th>
        </tr>
            <?php
                $selectString = "SELECT devices.name, ppm.devID, ppm.ppm, ppm.temp, ppm.pTime
                FROM (ppm RIGHT JOIN devices ON ppm.devID = devices.devID)
                INNER JOIN (SELECT ppm.devID, max(pTime) as max_time from ppm GROUP by devID) devices
                on ppm.pTime = max_time
                ORDER BY pTime DESC";
                $sqld = $pdo->query($selectString);       // Verification selection
                foreach($sqld as $row)
                {
                    $t = strtotime($row['pTime']);
                    $ts = (time() - $t) / 60;
                    $s = (time() - $t);

                    if($ts < 1440)  // If last seen within 24hrs, otherwise data not shown
                    {
                        echo("<tr><td><a href='/gauge.php?dev=".$row['devID']."'>".$row['name']."</a></td><td>".$row['ppm']."</td><td>".$row['temp']."</td><td>".$row['pTime']);
                        if (round($ts) == 0)
                        {
                            echo(" ($s seconds ago)</td></tr>");
                        }
                        else if (round($ts) == 1)
                        {
                            echo(" (".round($ts)." minute ago)</td></tr>");
                        }
                        else
                        {
                            echo(" (".round($ts)." minutes ago)</td></tr>");
                        }
                    }
                }
            ?>
        </table>
        
        <hr>

        <h3>One Sensor</h3>

        <div class="container">
        <form action="<?php echo $_SERVER['PHP_SELF']; ?>" method="post">

            <label for="device">Device</label>
            <select id="device" name="device">
                <?php
                    foreach($results as $row)
                    {
                        $val = $row['devID'];
                        $name = $row['name'];

                        echo('<option value="'.$val.'">'.$name.'</option>');
                    }
                ?>
            </select>

            <label for="time">Timespan</label>
            <select id="time" name="time">
            <option value="1">12 hours</option>
            <option value="2">24 hours</option>
            <option value="3">7 days</option>
            <option value="4">30 days</option>
            <option value="0">All time</option>
            </select>


            <input type="submit" name="single" value="Show Data">
        </form>
        </div>

        <hr>

        <h3>Compare Sensors</h3>

        <div class="container">
        <form action="<?php echo $_SERVER['PHP_SELF']; ?>" method="post">

            <label for="device1">Device 1</label>
            <select id="device1" name="device1">
                <?php
                    foreach($results as $row)
                    {
                        $val = $row['devID'];
                        $name = $row['name'];

                        echo('<option value="'.$val.'">'.$name.'</option>');
                    }
                ?>
            </select>

            <label for="device2">Device 2</label>
            <select id="device2" name="device2">
                <?php
                    foreach($results as $row)
                    {
                        $val = $row['devID'];
                        $name = $row['name'];

                        echo('<option value="'.$val.'">'.$name.'</option>');
                    }
                ?>
            </select>

            <label for="time">Timespan</label>
            <select id="time" name="time">
            <option value="1">12 hours</option>
            <option value="2">24 hours</option>
            <option value="3">7 days</option>
            <option value="4">30 days</option>
            <option value="0">All time</option>
            </select>


            <input type="submit" name="compare" value="Show Data">
        </form>
        </div>

        <hr>

        <h3>Rename Sensor</h3>

        <div class="container">
        <form action="<?php echo $_SERVER['PHP_SELF']; ?>" method="post">

            <label for="device">Device</label>
            <select id="device" name="device">
                <?php
                    foreach($results as $row)
                    {
                        $val = $row['devID'];
                        $name = $row['name'];

                        echo('<option value="'.$val.'">'.$val.' ['.$name.']</option>');
                    }
                ?>
            </select>

            <label for="name">New Name</label>
            <input type="text" id="name" name="name" placeholder="Device Name..." maxlength="20">
            <input type="submit" name="rename" value="Update Name">
        </form>
        <?php if ($renamed) { echo("<p style='color:green'>Device Successfully Renamed!</p>"); } ?>
        </div>

        <hr>

        <h3>Register New Sensor</h3>

        <div class="container">
        <form action="<?php echo $_SERVER['PHP_SELF']; ?>" method="post">
            <label for="name">Name</label>
            <input type="text" id="name" name="name" placeholder="Device Name" maxlength="20">
            <label for="devID">Device EUI or MAC Address</label>
            <input type="text" id="devID" name="devID" placeholder="FF:FF:FF:FF:FF:FF" maxlength="20">
            <input type="submit" name="register" value="Register Device">
        </form>
        <?php if ($registered) { echo("<p style='color:green'>Device Successfully Registered!</p>"); } ?>
        </div>

        <hr>
    </body>
</html>
