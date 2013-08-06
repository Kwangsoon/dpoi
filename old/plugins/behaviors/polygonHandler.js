OpenLayers.Handler.Dpoi = OpenLayers.Class(OpenLayers.Handler.Point, {
	/**
	 * Method: createFeature
	 * Add temporary features
	 *
	 * Parameters:
	 * pixel - {<OpenLayers.Pixel>} A pixel location on the map.
	 */
	createFeature: function(pixel) {
		var lonlat = this.layer.getLonLatFromViewPortPx(pixel); 
		var geometry = new OpenLayers.Geometry.Point(
				lonlat.lon, lonlat.lat
		);
		this.point = new OpenLayers.Feature.Vector(geometry);
		this.callback("create", [this.point.geometry, this.point]);
		this.point.geometry.clearBounds();
		this.layer.addFeatures([this.point], {silent: true});
		
		console.log("asdadadasda");
	},
});