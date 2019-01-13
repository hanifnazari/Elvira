<!DOCTYPE html>
<html>
    <head>
        <h1>My first PHP page</h1>
    </head>
    <body>
        <form action="" method="post">
            <button type="submit" name="red" value="call">RED</button>
            <button type="submit" name="green" value="call">GREEN</button>
            <button type="submit" name="blue" value="call">BLUE</button>
        </form>
    </body>
</html>

<?php
if(isset($_POST['red']))
    {
        echo shell_exec("red.exe");
    }

if(isset($_POST['green']))
    {
        echo shell_exec("green.exe");
    }

if(isset($_POST['blue']))
    {
        echo shell_exec("blue.exe");
    }
?>