<?php 
$command = $_GET['command'];
list($name,$action)=explode(',', $command);
$servername = "localhost";
$username = "root";
$password = "1234";
$dbname = "place";
try {
    $conn = new PDO("mysql:host=$servername;dbname=$dbname", $username, $password);
    $conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
	$sql = "insert into command(name, action)values('".$name."','".$action."');";
	$conn->exec($sql);
	echo "Sent:".$name." ".$action;
}
catch(PDOException $e)
{
    echo $sql.$e->getMessage();
}
?> 

