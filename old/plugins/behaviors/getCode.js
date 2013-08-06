var dpoi_getCode = {
	temp_point : new Array(),

	var lonlat : new OpenLayers.LonLat(poi_x, poi_y),
	var proj_lonlat : lonlat.transform(new OpenLayers.Projection('EPSG:4326'), new OpenLayers.Projection('EPSG:900913')),

	getCode : function () {
		var pos = location.href.indexOf("/node/");
		var getGeohash;
		
		getGeohash = location.href.substr(0,pos) + "/dpoi?point=" + proj_lonlat.x + "," + proj_lonlat.y;
		console.log(getGeohash);
		
		$.ajax({
			type: 'GET',
			url: getGeohash,
			success: function(data) {
				var result = $.parseJSON(data);
				var geohashCode = result.geohash;
				console.log(geohashCode);
			},
			error: function(e) {
				console.log(e);
			}
		});
		return;
	},
};