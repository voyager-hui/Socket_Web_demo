<?php
$servername = "localhost";
$username = "root";
$password = "1234";
$dbname = "place";
try {
    $conn = new PDO("mysql:host=$servername;dbname=$dbname", $username, $password);
    $conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
	$stmt = $conn->prepare("select * from data order by no desc limit 1;"); 
    $stmt->execute();
	$stmt->setFetchMode(PDO::FETCH_ASSOC);
	$row = $stmt->fetch();
	echo "no.".$row['no']." name:".$row['name']." lon:".$row['lon']." lat:".$row['lat'];
}
catch(PDOException $e)
{
    echo $sql.$e->getMessage();
}
?> 

