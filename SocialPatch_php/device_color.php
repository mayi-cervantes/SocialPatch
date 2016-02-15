<?php

//push data into database: Socialpatch
//http://ixd-socialpatch.uqcloud.net/phpmyadmin

require_once 'utils.php';

$sth = $db ->prepare(" SELECT light FROM Sensor_Reading WHERE datetime = (SELECT MAX (datetime) FROM Sensor_Reading) LIMIT 1");
$sth -> execute();

print("Helloooo");

$result = $sth -> fetchALL();
print_r($result);
?>