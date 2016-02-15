<?php

//connection

$db = new PDO('mysql:host=localhost;dbname=test;charset=utf8', 'photon_data', 'MayiNew01%');
$db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);
$db->setAttribute(PDO::ATTR_EMULATE_PREPARES, false);

// test connection

try{
	$testresult = $db->query("SELECT * FROM old_values");
	echo "Connected";
}
catch(PDOException $ex)
{
	echo "An Error occured!";
	echo $ex->getMessage();
}
?>