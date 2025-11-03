<?php
    include 'sql.inc.php';
	
    $pass = "VerySpookySecretKey!"; // Known Auth key!

    // Receive the POST data.
    $content = trim(file_get_contents("php://input"));
 
    // Attempt to decode the incoming RAW post data to JSON.
    $json = json_decode($content);
	
	// Auth stuff
	$sent = $_SERVER['HTTP_AUTHORIZATION'];												// Sent Auth key from TTN
	if ($sent != $pass)	// If they're not equal
	{ 
		$json = NULL; 		// Destroys incoming data before it gets near DB. 
	}

    /////////////////////////////////////

    if ($_POST['key'] == $pass)        // If pass correct (WiFi Devices)
	{
        $ppm = $_POST['ppm'];       // Gathers user input
        strip_tags($ppm);             // Strips any tags from user input (Security)
        $temp = $_POST['temp'];       // Gathers user input
        strip_tags($temp);             // Strips any tags from user input (Security)
        $dev = $_POST['dev'];       // Gathers user input
        strip_tags($dev);             // Strips any tags from user input (Security)

        $insertQuery="INSERT INTO carbon.ppm (pID, pTime, ppm, temp, devID) VALUES (NULL, CURRENT_TIMESTAMP, :ppm, :temp, :dev)"; // Template SQL Query 
        $stmt =$pdo->prepare($insertQuery);
        $stmt->bindParam(':ppm', $ppm);
        $stmt->bindParam(':temp', $temp);
        $stmt->bindParam(':dev', $dev);

        $stmt->execute();       // Inserts row into table
    }
    
    else if ($json != NULL)         // If request sent by TTN
    {
        $payload = base64_decode($json->payload_raw);   // Converts Base64 payload to string
        $col = strpos($payload, ':');                   // Finds column in string, saves position as integer

        $ppm = substr($payload, 0, $col);           // Everything before the column (PPM)
        $temp = substr($payload, ($col+1), 5);      // Everything after the column (Temperature)

        $insertQuery="INSERT INTO carbon.ppm (pID, pTime, ppm, temp, devID) VALUES (NULL, CURRENT_TIMESTAMP, :ppm, :temp, :dev)"; // Template SQL Query 
        $stmt =$pdo->prepare($insertQuery);
        $stmt->bindParam(':ppm', $ppm);                     // PPM 
        $stmt->bindParam(':temp', intval($temp));           // Temperature as integer (Removes trailing empty bytes)
        $stmt->bindParam(':dev', $json->hardware_serial);   // Device EUI
		
        $stmt->execute();    // Inserts row into table
    }

    else
    {
        echo("No useful data sent!");
    }
    
?>