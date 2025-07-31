<!--
     Dev: Henry Greenslade - (GREEHR1)
    Desc: Error Output HTML
-->

<!DOCTYPE html>
<html lang="en">
    <head>
        <meta charset="UTF-8">
        <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <meta http-equiv="X-UA-Compatible" content="ie=edge">
        <title>ERROR PAGE</title>
    </head>

    <body>
        <p>
        <?php
            echo ($error . "<br>" . $e->getMessage()); 
        ?>
        </p>
    </body>
</html>