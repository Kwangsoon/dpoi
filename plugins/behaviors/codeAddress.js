var geocoder;

function codeAddress() {
	geocoder = new google.maps.Geocoder();
	var address = document.getElementById("address").value;

	if (markers !== null) {
		markers.clearMarkers();
	}

	geocoder.geocode( {'address': address}, function(results, status) {
		if (status == google.maps.GeocoderStatus.OK) {
			var sProj = new OpenLayers.Projection('EPSG:900913');
			var tProj = new OpenLayers.Projection('EPSG:4326');

			var lonlat = new OpenLayers.LonLat(results[0].geometry.location.Za, results[0].geometry.location.Ya);
			lonlat.transform(tProj, sProj);

			dpoi_map.setCenter(lonlat);
			dpoi_map.addLayer(markers);

			var size = new OpenLayers.Size(21,25);
			var offset = new OpenLayers.Pixel(-(size.w/2), -size.h);
			var icon = new OpenLayers.Icon('http://www.imrc.kist.re.kr/~procarrie/icons/poi.png', size, offset);
			markers.addMarker(new OpenLayers.Marker(lonlat, icon));

//			console.log(address);

//			document.getElementById("edit-field-address-und-0-value").value = address;

		} else {
			alert("Geocode was not successful for the following reason: " + status);
		}
	});
}