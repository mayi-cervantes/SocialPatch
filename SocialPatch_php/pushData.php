<?php

//push data into database: Socialpatch
//http://ixd-socialpatch.uqcloud.net/phpmyadmin

include_once 'utils.php';

date_default_timezone_set('Australia/Brisbane');

//$p_particleDeviceID = $_POST['thisdevice']; (original)
$p_S_ID = $_POST['thisdevice'];

//$p_variable = $_POST['variable']; (original)
$p_Type = $_POST['Type'];

//$p_value = $_POST['value']; (original)
$p_Readings = $_POST['Readings'];

$location = 'Rooftop 207';
//$p_location = $_POST['location'];

$plant_name = 'Mint';

$p_light = $_POST['light'];

$p_eventID = $_POST['eventID']; 
$s_eventID = date("Y").date("m").date("d").date("h").date("i")."-".$p_eventID;


try{
	$stmt = $db->prepare("INSERT INTO Sensor_Reading(S_ID, Type, Readings, location, plant_name, light, eventID, datetime) VALUES (:S_ID, :Type, :Readings, :location, :plant_name, :light, :eventID, :datetime)");

	$stmt->bindValue(":S_ID", $p_S_ID, PDO::PARAM_STR);
	$stmt->bindValue(":Type", $p_Type, PDO::PARAM_STR);
	$stmt->bindValue(":Readings", $p_Readings, PDO::PARAM_STR);
	$stmt->bindValue(":location", $location, PDO::PARAM_STR);
	$stmt->bindValue(":plant_name", $plant_name, PDO::PARAM_STR);
	$stmt->bindValue(":light", $p_light, PDO::PARAM_STR);
	$stmt->bindValue(":eventID", $s_eventID, PDO::PARAM_STR);
	$stmt->bindValue(":datetime", date("Y-m-d H:i:s"), PDO::PARAM_STR);

	$stmt->execute();

	echo "data pushed to pbot db";	
}
catch (PDOException $e)
{
	echo "Error: ".$e->getMessage();
}

$db = null;

?>