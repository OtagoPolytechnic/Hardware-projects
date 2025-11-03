<?php
    date_default_timezone_set('NZ');

    include 'sql.inc.php';

    if(isset($_POST['single']))
    {
        $device = $_POST['device'];
        strip_tags($device);             // Strips any tags from user input (Security)
        $time = $_POST['time'];
        strip_tags($time);             // Strips any tags from user input (Security)

        $sqltime = NULL; 

        switch ($time) {
            case 0:
                $sqltime = NULL; 
                break;
            case 1:
                $sqltime = "12 HOUR"; 
                break;
            case 2:
                $sqltime = "24 HOUR"; 
                break;
            case 3:
                $sqltime = "7 DAY"; 
                break;
            case 4:
                $sqltime = "30 DAY"; 
                break;
        }

        showdata($pdo, $device, $sqltime);
    }

    else if(isset($_POST['compare']))
    {
        $device1 = $_POST['device1'];
        strip_tags($device1);             // Strips any tags from user input (Security)
        $device2 = $_POST['device2'];
        strip_tags($device2);             // Strips any tags from user input (Security)
        $time = $_POST['time'];
        strip_tags($time);             // Strips any tags from user input (Security)

        $sqltime = NULL; 

        switch ($time) {
            case 0:
                $sqltime = NULL; 
                break;
            case 1:
                $sqltime = "12 HOUR"; 
                break;
            case 2:
                $sqltime = "24 HOUR"; 
                break;
            case 3:
                $sqltime = "7 DAY"; 
                break;
            case 4:
                $sqltime = "30 DAY"; 
                break;
        }

        comparedata($pdo, $device1, $device2, $sqltime);
    }

    else if(isset($_POST['rename']))
    {
        $device = $_POST['device'];
        strip_tags($device);             // Strips any tags from user input (Security)
        $name = $_POST['name'];
        strip_tags($name);             // Strips any tags from user input (Security)

        $sql = "UPDATE devices SET name=? WHERE devID=?";
        $stmt= $pdo->prepare($sql);
        $stmt->execute([$name, $device]);

        try
        {
            $statement= $pdo->prepare('SELECT devID, name FROM devices');
            $statement->execute();
            $results = $statement->fetchAll(PDO::FETCH_ASSOC);
        }

        catch (PDOException $e)
        {
            $error = 'Select statement error';
            include 'error.html.php';
            exit();
        }

        $renamed = true;
        include 'main.inc.php';
    }

    else if(isset($_POST['register']))
    {
        $device = $_POST['devID'];
        strip_tags($device);             // Strips any tags from user input (Security)
        $name = $_POST['name'];
        strip_tags($name);             // Strips any tags from user input (Security)

        $sql = "INSERT INTO devices (dnum, devID, name) VALUES (NULL, :d, :n)";
        $stmt =$pdo->prepare($sql);
        $stmt->bindParam(':d', $device);
        $stmt->bindParam(':n', $name);

        $stmt->execute();       // Inserts row into table

        try
        {
            $statement= $pdo->prepare('SELECT devID, name FROM devices');
            $statement->execute();
            $results = $statement->fetchAll(PDO::FETCH_ASSOC);
        }

        catch (PDOException $e)
        {
            $error = 'Select statement error';
            include 'error.html.php';
            exit();
        }

        $registered = true;
        include 'main.inc.php';
    }

    else
    {
        try
        {
            $statement= $pdo->prepare('SELECT devID, name FROM devices');
            $statement->execute();
            $results = $statement->fetchAll(PDO::FETCH_ASSOC);
        }

        catch (PDOException $e)
        {
            $error = 'Select statement error';
            include 'error.html.php';
            exit();
        }

        include 'main.inc.php';
    }

    function showdata($pdo, $dev, $time)
    {
        if($time == NULL)
        {
            try
            {
                $selectString = "SELECT * FROM ppm WHERE devID = '".$dev."' ORDER BY pTime ASC";
                $posts = $pdo->query($selectString);       // Verification selection
            }

            catch (PDOException $e)
            {
                $error = 'Select statement error';
                include 'error.html.php';
                exit();
            }
        }

        else
        {
            try
            {
                $selectString = "SELECT * FROM ppm WHERE pTime > DATE_SUB(NOW(), INTERVAL ".$time.") AND pTime <= NOW() AND devID = '".$dev."' ORDER BY pTime ASC";
                $posts = $pdo->query($selectString);       // Verification selection
            }

            catch (PDOException $e)
            {
                $error = 'Select statement error';
                include 'error.html.php';
                exit();
            }
        }
        
        include 'data.inc.php';
    }

    function comparedata($pdo, $dev1, $dev2, $time)
    {
        if($time == NULL)
        {
            try
            {
                $selectString = "SELECT * FROM ppm WHERE devID = '".$dev1."' OR devID = '".$dev2."' ORDER BY pTime ASC";
                $posts = $pdo->query($selectString);       // Verification selection
            }

            catch (PDOException $e)
            {
                $error = 'Select statement error';
                include 'error.html.php';
                exit();
            }
        }

        else
        {
            try
            {
                $selectString = "SELECT * FROM ppm WHERE pTime > DATE_SUB(NOW(), INTERVAL ".$time.") AND pTime <= NOW() AND devID = '".$dev1."' OR devID = '".$dev2."' ORDER BY pTime ASC";
                $posts = $pdo->query($selectString);       // Verification selection
            }

            catch (PDOException $e)
            {
                $error = 'Select statement error';
                include 'error.html.php';
                exit();
            }
        }
        
        include 'compare.inc.php';
    }
?>