<?php

/*
function dpoi_menu() {
	$items = array();
	$items['dpoi/dpoi_search'] = array(
			'page callback' => 'dpoi_dpoi_search_callback',
			'access callback' => TRUE,
 			'access arguments' => array('access content'),
			'type' => MENU_CALLBACK,
	);
	echo "aaaaa";
	return $items;
}

function dpoi_dpoi_search_callback() {
	$target_url = "http://play.imrc.kist.re.kr/dpoi2/sites/all/modules/dpoi/plugins/behaviors/dbconnection.php?";
	$param = "";
	
	echo $param;
	
	if (isset($_GET['poiX']) && isset($_GET['poiY']) && isset($_GET['cZoom'])) {
		$param = $param . "poiX=" . $_GET['poiX'] . "&poiY=" . $_GET['poiY'] . "&cZoom=" . $_GET['cZoom'];
	}
	if ($param != "") {
		$output = file_get_contents($target_url . $param);
	}
	else {
		$output = "[]";
	}
	print $output;
}
*/

/*
function dpoi_menu() {
	$items = array();
	$items['dpoi/dpoi_search'] = array(
			'page callback' => 'dpoi_dpoi_search_callback',
			'access arguments' => array('access content'),
			'type' => MENU_CALLBACK,
	);
	return $items;
}

function dpoi_access_callback() {
	return TRUE;
}

function dpoi_dpoi_search_callback() {
	$target_url = "http://www.dpoi.org/rest/search_kto/retrieve.json?";
	$param = "";

	if (isset($_GET['keys'])) {
		$param = $param . "keys=" . $_GET['keys'];
	}

	if (isset($_GET['cat'])) {
		if ($param != "") {
			$param = $param . "&";
		}
		$param = $param . "cat=" . $_GET['cat'];
	}

	if (isset($_GET['pos'])) {
		if ($param != "") {
			$param = $param . "&";
		}
		$param = $param . "pos=" . $_GET['pos'];
	}

	if (isset($_GET['radius'])) {
		if ($param != "") {
			$param = $param . "&";
		}
		$param = $param . "radius=" . $_GET['radius'];
	}

	if (isset($_GET['bbox'])) {
		if ($param != "") {
			$param = $param . "&";
		}
		$param = $param . "bbox=" . $_GET['bbox'];
	}

	if (isset($_GET['num'])) {
		if ($param != "") {
			$param = $param . "&";
		}
		$param = $param . "num=" . $_GET['num'];
	}

	if (isset($_GET['page'])) {
		if ($param != "") {
			$param = $param . "&";
		}
		$param = $param . "page=" . $_GET['page'];
	}

	if ($param != "") {
		$output = file_get_contents($target_url . $param);
	}
	else {
		$output = "[]";
	}

	print $output;
}*/
 
/**
 * Implements hook_menu(). - for ajax framework
*/
function dpoi_menu() {
	$items = array();
	$items['smart_trail/dpoi_search'] = array(
			'page callback' => 'dpoi_dpoi_search_callback',
			'access callback' => TRUE,
			'access arguments' => array('access content'),
			'type' => MENU_CALLBACK,
	);
	return $items;
}

function dpoi_dpoi_search_callback() {
	$target_url = "http://www.dpoi.org/rest/search_kto/retrieve.json?";
	$param = "";

	if (isset($_GET['keys'])) {
		$param = $param . "keys=" . $_GET['keys'];
	}

	if (isset($_GET['cat'])) {
		if ($param != "") {
			$param = $param . "&";
		}
		$param = $param . "cat=" . $_GET['cat'];
	}

	if (isset($_GET['pos'])) {
		if ($param != "") {
			$param = $param . "&";
		}
		$param = $param . "pos=" . $_GET['pos'];
	}

	if (isset($_GET['radius'])) {
		if ($param != "") {
			$param = $param . "&";
		}
		$param = $param . "radius=" . $_GET['radius'];
	}

	if (isset($_GET['bbox'])) {
		if ($param != "") {
			$param = $param . "&";
		}
		$param = $param . "bbox=" . $_GET['bbox'];
	}

	if (isset($_GET['num'])) {
		if ($param != "") {
			$param = $param . "&";
		}
		$param = $param . "num=" . $_GET['num'];
	}

	if (isset($_GET['page'])) {
		if ($param != "") {
			$param = $param . "&";
		}
		$param = $param . "page=" . $_GET['page'];
	}

	if ($param != "") {
		$output = file_get_contents($target_url . $param);
	}
	else {
		$output = "[]";
	}

	print $output;
}