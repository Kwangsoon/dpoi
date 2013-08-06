/**
 * @file
 * JS Implementation of OpenLayers behavior.
 */

(function($) {
	/**
	 * Geofield Behavior
	 */
	Drupal.behaviors.openlayers_behavior_dpoi = {
			'attach': function(context, settings) {
				var data = $(context).data('openlayers'),
				behavior = data && data.map.behaviors['openlayers_behavior_dpoi'],
				dataProjection = new OpenLayers.Projection('EPSG:4326'),
				features, wktFormat;

				// helper to create a WKT format object with the right projections
				function initWktFormat (inp, outp) {
					var WktWriter = new OpenLayers.Format.WKT();
					WktWriter.internalProjection = inp;
					WktWriter.externalProjection = outp || dataProjection;
					return WktWriter;
				}

				// populate our wkt input field
				function updateWKTField (features) {
					var WktWriter = initWktFormat(features.object.map.projection);
					// limits are to be checked server-side, not here.
					// for a single shape avoid GEOMETRYCOLLECTION
					var toSerialize = features.object.features;
					// don't serialize empty feature

					// define projection for transformation
					var sProj = new OpenLayers.Projection('EPSG:900913');
					var tProj = new OpenLayers.Projection('EPSG:4326');

					if (features.feature.geometry.CLASS_NAME === "OpenLayers.Geometry.Point") {
						// get origin x, y from object
						var pointX = features.feature.geometry.x;
						var pointY = features.feature.geometry.y;

						// transform target x, y (WGS84)
						var pointCoord = new OpenLayers.LonLat(pointX, pointY);
						var proj_lonlat = pointCoord.transform(sProj, tProj);
						var poiX = proj_lonlat.lon;
						var poiY = proj_lonlat.lat;
						var cZoom = features.object.map.getZoom() * 5;
						
						var geohashcode = Fgh.encode(poiX, poiY, cZoom);
						
						console.log("**** Point ****");
						console.log("Current point : " + poiX+ ", " + poiY + ". Current Zoom level : " + cZoom/5);
						console.log("Geohash code : " + geohashcode + "\nLetters of Geohash code : " + cZoom/5);
						
						var text_val = document.getElementById("edit-field-geohash-und-0-value").value;
						
						if (text_val === "") {
							document.getElementById("edit-field-geohash-und-0-value").value = geohashcode;
						} else {
							document.getElementById("edit-field-geohash-und-0-value").value +=  ", " + geohashcode;
						}
					} else if (features.feature.geometry.CLASS_NAME === "OpenLayers.Geometry.Polygon") {
						var centerPointX = features.feature.geometry.bounds.getCenterLonLat().lon;
						var centerPointY = features.feature.geometry.bounds.getCenterLonLat().lat;
						
						var polygonCoord = new OpenLayers.LonLat(centerPointX, centerPointY);
						var proj_centerPoint = polygonCoord.transform(sProj, tProj);
						var cZoom = features.object.map.getZoom() * 5;
						
						var geohashcode = Fgh.encode(proj_centerPoint.lon, proj_centerPoint.lat, cZoom);
						
//						var bounds = new OpenLayers.Bounds();
						
//						var center = bounds.getCenterLonLat();
//						console.log(center);
//						features.object.map.setCenter(center, features.object.map.getZoomForExtent(bounds) -1);
						
						console.log(features);
//						console.log(data.openlayers.getZoomForExtent());
						console.log(data.openlayers);
						
						console.log("**** Polygon ****");
						console.log("Center point of current polygon : " + proj_centerPoint.lon + ", " + proj_centerPoint.lat);
						console.log("Geohash code : " + geohashcode + "\nLetters of Geohash code : " + cZoom/5);
						
						var text_val = document.getElementById("edit-field-geohash-und-0-value").value;
						
						if (text_val === "") {
							document.getElementById("edit-field-geohash-und-0-value").value = geohashcode;
						} else {
							document.getElementById("edit-field-geohash-und-0-value").value +=  ", " + geohashcode;
						}
					}
/*
						var pos = location.href.indexOf("/node/");
//						var getGeohash;
//						getGeohash = location.href.substr(0,pos) + "/sites/all/modules/dpoi/plugins/behaviors/dbconnection.php?poiX=" + poiX + "&poiY=" + poiY + "&cZoom=" + cZoom;
//						getGeohash = location.href.substr(0,pos) + "/dpoi/dpoi_search/dbconnection.php?poiX=" + poiX + "&poiY=" + poiY + "&cZoom=" + cZoom;
						getGeohash = Drupal.settings.basePath + "sites/all/modules/dpoi/plugins/behaviors/dbconnection.php?poiX=" + poiX + "&poiY=" + poiY + "&cZoom=" + cZoom;
						
						$.ajax ({
							type : 'GET',
							url : getGeohash,
//							url : Drupal.settings.basePath + 'dpoi/dpoi_search?poiX=' + poiX + '&poiY=' + poiY + '&cZoom' + cZoom,
							data : {
								poiX : poiX,
								poiY : poiY,
								cZoom : cZoom
							},
							success : function(data) {
								console.log(data);
								console.log("Geohash code is " + data + "\nGeohash code digit is " + cZoom);
								var text_val = document.getElementById("edit-field-geohash-und-0-value").value;
								if (text_val === "") {
									document.getElementById("edit-field-geohash-und-0-value").value = data;
								} else {
									document.getElementById("edit-field-geohash-und-0-value").value +=  ", " + data;
								}
							},
							error: function(e) {
								console.log(e);
							}
						});
					} else if (features.feature.geometry.CLASS_NAME === "OpenLayers.Geometry.Polygon"){
						position = new Array();
						for (var i=0; i < features.feature.geometry.components[0].components.length; i++) {
							var pointX = features.feature.geometry.components[0].components[i].x;
							var pointY = features.feature.geometry.components[0].components[i].y;

							// transform target x, y (WGS84)
							var pointCoord = new OpenLayers.LonLat(pointX, pointY);
							var proj_lonlat = pointCoord.transform(sProj, tProj);
							
							var pos_row = new Object();
							pos_row.x = proj_lonlat.lon;
							pos_row.y = proj_lonlat.lat;
							pos_row.cZoom = features.object.map.getZoom();
							
							position.push(pos_row);

							console.log(position);
//							console.log("x : " + poiX + ", y : " + poiY + ". Current Zoom level : " + cZoom);
						}
						
						var pos = location.href.indexOf("/node/");
						var phpLoc = "/sites/all/modules/dpoi/plugins/behaviors/dbconnection.php?";	
						var getGeohash = location.href.substr(0,pos) + phpLoc;

						$.ajax ({
							url : getGeohash,
							type : 'GET',
							data : {
								position : position
							},
							success : function(data) {
//								console.log("Geohash code is " + data + "\nGeohash code digit is " + cZoom);
								var text_val = document.getElementById("edit-field-geohash-und-0-value").value;
								if (text_val === "") {
									document.getElementById("edit-field-geohash-und-0-value").value = data;
								} else {
									document.getElementById("edit-field-geohash-und-0-value").value +=  ", " + data;
								}
							}
						});
					}
					*/

					if (toSerialize.length) {
						if (toSerialize.length === 1) { toSerialize = toSerialize[0]; }
						this.val(WktWriter.write(toSerialize));
					}
					// but clear the value
					else {
						this.val('');
					}
				}

				// keep only one features for each map input
				function limitFeatures (features) {
					// copy a list of features
					var copyFeatures = features.object.features.slice();
					// only keep the last one
					var lastFeature = copyFeatures.pop();
					// we remove a lot of features, don't trigger events
					features.object.destroyFeatures(copyFeatures, {silient: true});
				}

				if (behavior && !$(context).hasClass('geofield-processed')) {
					// we get the .form-item wrapper which is a slibling of our hidden input
					var $wkt = $(context).closest('.form-item').parent().find('input.geofield_wkt');
					// if there is no form input this shouldn't be activated
					if ($wkt.length) {
						var dataLayer = new OpenLayers.Layer.Vector(Drupal.t('Feature Layer'), {
							projection: dataProjection,
							drupalID: 'openlayers_behavior_dpoi'
						});

						dataLayer.styleMap = Drupal.openlayers.getStyleMap(data.map, 'openlayers_behavior_dpoi');
						data.openlayers.addLayer(dataLayer);
						
						dpoi_map = data.openlayers;
						
						markers = new OpenLayers.Layer.Markers("Markers");
			            dpoi_map.addLayer(markers);

						// only one feature on each map register before adding our data
						if (Drupal.settings.geofield.data_storage == 'single') {
							dataLayer.events.register('featureadded', $wkt, limitFeatures);
						}

						if ($wkt.val() != '') {
							wktFormat = initWktFormat(data.openlayers.projection);
							features = wktFormat.read($wkt.val());
							dataLayer.addFeatures(features);
						}

						// registering events late, because adding data
						// would result in a reprojection loop
						dataLayer.events.register('featureadded', $wkt, updateWKTField);
						dataLayer.events.register('featureremoved', $wkt, updateWKTField);
						dataLayer.events.register('afterfeaturemodified', $wkt, updateWKTField);

						// transform options object to array
						behavior.tools = [];
						// add a new 'tools' key which is an array of enabled features
						$.each(behavior.feature_types, function (key, value) {
							if (value) {
								behavior.tools.push(key);
							}
						});
						// create toolbar
						var geofieldControl = new OpenLayers.Control.GeofieldEditingToolbar(dataLayer, behavior);
						data.openlayers.addControl(geofieldControl);

						// on submit recalculate everything to be up to date
						var formData = {
								'control': geofieldControl,
								'dataLayer': dataLayer
						};
						function handleSubmit (e) {
							$.map(e.data.control.controls, function(c) { c.deactivate(); });
							dataLayer.events.triggerEvent('featuremodified');
						}
						$(context).parents('form').bind('submit', formData, handleSubmit);
					}
					$(context).addClass('geofield-processed');
				} // if
			}
	};
})(jQuery);
