<?php
    $host = "localhost";
    $userMS = "DBUSER";
    $passwordMS = "DBPASS";
    $database = "carbon";
        
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
?>