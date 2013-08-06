<!DOCTYPE html>

<html>

<meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />

<script type="text/javascript" src="http://maps.googleapis.com/maps/api/js?sensor=false"></script>

<script type="text/javascript" src="http://www.imrc.kist.re.kr/~procarrie/jquery-1.9.0.js"></script>

<script type="text/javascript" src="http://google-maps-utility-library-v3.googlecode.com/svn/tags/markerclustererplus/2.0.12/src/markerclusterer_packed.js"></script>


<script type="text/javascript" charset="utf-8">

var markerClusterer = null;


var geohash_result = ['', '', '', '', '', 'wydm6', 'wydm6x', 'wydm6x0', '', ''];
var geohash_initial_num  = '';
var geohash_visit = [false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false, false];

function initialize() {
	var map_center = new google.maps.LatLng(37.577, 126.978);

	map = new google.maps.Map(document.getElementById("map_canvas"), {
		zoom: 10,
		center: map_center,
		mapTypeId: google.maps.MapTypeId.ROADMAP,
		panControl: false,
		streetViewControl: true,
		mapTypeControl: false
	});
}

function searchUtil(){
	var keywords = document.getElementById("input_value").value;
	var geohash_code = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32];

	$.getJSON("http://dpoi.imrc.kist.re.kr/select/?q=title%3A" + keywords + "%0D%0A&version=2.2&start=0&rows=100&indent=on&wt=json&json.wrf=?", function(result){
		geohash_initial_num = result.response.docs[0].geohash_code.toString().length;
		geohash_result[geohash_initial_num] = (result.response.docs[0].geohash_code.toString());

		poi_markers = [];

		for (i=0; i<result.response.numFound; i++) {
			var poi_poiPoint = result.response.docs[i].location_point.split(",");
			poi_pos = new google.maps.LatLng(poi_poiPoint[0], poi_poiPoint[1]);

			var poi_title = result.response.docs[i].title;
			var poi_category = result.response.docs[i].dpoi_category;
			var poi_poi_link = result.response.docs[i].entity_id;

			var cat;

			if (poi_category == 32) {
				cat = "Culture/Art/History";
			} else if (poi_category == 105) {
				cat = "Leports";
			} else if (poi_category == 152) {
				cat = "Shopping";
			} else if (poi_category == 199) {
				cat = "Hotel";
			} else if (poi_category == 163) {
				cat = "Restaurant";
			} else if (poi_category == 8) {
				cat = "Nature";
			} else if(poi_category == 184) {
				cat = "Transport";
			}

			poi_marker = new google.maps.Marker({
				position: poi_pos,
				icon: 'http://www.imrc.kist.re.kr/~procarrie/icons/poi.png'
			});

			var poi_infowindow = new google.maps.InfoWindow({
				content: 'Title : ' + poi_title + '<br>' +
				'Category : ' + cat + '<br>' +
				'<a href=\"http://play.imrc.kist.re.kr/dpoi3/node/' + poi_poi_link + '\">More view</a>'
			});

			makeInfoWindowEvent(map, poi_infowindow, poi_marker);
			closeInfoWindowEvent(map, poi_infowindow, poi_marker);

			poi_markers.push(poi_marker);
		}
		var poi_markerCluster = new MarkerClusterer(map, poi_markers);

		$.getJSON("http://dpoi.imrc.kist.re.kr/select/?q=geohash_code%3A" + geohash_result[geohash_initial_num] +"*&version=2.2&start=0&rows=30000&indent=on&wt=json&json.wrf=?", function(result){
			markers = [];

			for (i=0; i<result.response.numFound; i++) {
				var poiPoint = result.response.docs[i].location_point.split(",");
				pos = new google.maps.LatLng(poiPoint[0], poiPoint[1]);

				var poi_title = result.response.docs[i].title;
				var poi_category = result.response.docs[i].dpoi_category;

				var cat;

				if (poi_category == 32) {
					cat = "Culture/Art/History";
				} else if (poi_category == 105) {
					cat = "Leports";
				} else if (poi_category == 152) {
					cat = "Shopping";
				} else if (poi_category == 199) {
					cat = "Hotel";
				} else if (poi_category == 163) {
					cat = "Restaurant";
				} else if (poi_category == 8) {
					cat = "Nature";
				} else if(poi_category == 184) {
					cat = "Transport";
				}

				var poi_link = result.response.docs[i].entity_id;

				marker = new google.maps.Marker({
					position: pos,
					title: 'Title : ' + poi_title + ', Category : ' + cat,
					icon: 'http://www.imrc.kist.re.kr/~procarrie/icons/poi.png'
				});

				var infowindow = new google.maps.InfoWindow({
					content: 'Title : ' + poi_title + '<br>' +
					'Category : ' + cat + '<br>' +
					'<a href=\"http://play.imrc.kist.re.kr/dpoi3/node/' + poi_link + '\">More view</a>'
				});

				makeInfoWindowEvent(map, infowindow, marker);
				closeInfoWindowEvent(map, infowindow, marker);

				markers.push(marker);
			}
			var markerCluster = new MarkerClusterer(map, markers);

			var cnt = count.counter;
		});
	});
}

function poi_setCenter() {
	map.setCenter(pos);
}

function makeInfoWindowEvent(map, infowindow, marker) {
	google.maps.event.addListener(marker, 'click', function() {
		infowindow.open(map, this);
	});
}
function closeInfoWindowEvent(map, infowindow, marker) {
	google.maps.event.addListener(map, 'click', function() {
		infowindow.close();
	});
}

function count(){
	if( typeof count.counter == 'undefined' ) {
		count.counter = 0;
	}
	if (count.counter <= 3)
	count.counter++;
}

</script>
<body onload="initialize()"><h4>Enter keywords</h4>


<input id="input_value" type="text" placeholder="Input name of POI" size="30" />
<input onclick="javascript:searchUtil();" type="submit" value="Search"/>
<div>     </div>
<div id="map_canvas" style="height:400px; width:600px;"></div>
<table style="height:300px;"><tr id="poiList"></tr></table>
</body>
</html>