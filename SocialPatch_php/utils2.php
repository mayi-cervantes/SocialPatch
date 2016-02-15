<?php

//connection

$db = new PDO('mysql:host=localhost;dbname=Socialpatch;charset=utf8', 'root', '9UktAB)AnYcO');

$db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

$db->setAttribute(PDO::ATTR_EMULATE_PREPARES, false);


// test connection

try{
  $testresult = $db->query("SELECT * FROM Sensor_Reading_CGarden");
  
//echo "Connected";
}

catch(PDOException $ex)
{
  echo "An Error occured!";
  echo $ex->getMessage();
}
?>