<?php 
$result = $_GET['command'];
$myfile = fopen("./sensor/command.txt", "w") or die("Unable to open file!");
fwrite($myfile, $result);
echo "Sent:".$result;
?> 

