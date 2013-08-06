<?php
$poiX = $_GET["poiX"];
$poiY = $_GET["poiY"];
$cZoom = $_GET["cZoom"];

$dbconn = pg_connect("host=161.122.38.17 port=5432 dbname=presence user=procarrie password=pro1459")
	or die("Error Connecting to database : " . pg_last_error());

$query = "select ST_GeoHash(GeomFromText('POINT(" . $poiX . " " . $poiY . ")', 4326)," . $cZoom . ")";
$result = pg_query($dbconn, $query);
$return_val;

while ($row = pg_fetch_assoc($result)) {
	foreach ($row as $val) {
			$return_val = $val;
			echo $return_val;
	}
}

pg_free_result($result);
pg_close($dbconn);
/*
$position = [];
$position = $_GET["position"];

$dbconn = pg_connect("host=161.122.38.17 port=5432 dbname=presence user=procarrie password=pro1459")
	or die("Error Connecting to database : " . pg_last_error());

$temp = "";
$coom = 11;

for ($i = 0; $i < count($position); $i++) {
	$temp .= $position[$i]->$x . " " . $position[$i]->$y;
	if ($i != count($position) - 1) {
		$temp .= ",";
	}
}
$query = "select ST_GeoHash(GeomFromText('POLYGON(" . $temp . ")', 4326)," . $coom . ")";

$result = pg_query($dbconn, $query);
$return_val;

while ($row = pg_fetch_assoc($result)) {
	foreach ($row as $val) {
			$return_val = $val;
			echo $return_val;
	}
}
pg_free_result($result);
pg_close($dbconn);
*/
?>