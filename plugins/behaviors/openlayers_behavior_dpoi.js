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
					map_features = features;
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
//						var cZoom = features.object.map.getZoom() * 5;
						
//						var geohashcode = Fgh.encode(poiX, poiY, cZoom);
						var geohashcode = pointEncodeGeoHash(poiY, poiX);
						
						console.log("**** Point ****");
						
						var geocodeUrl = "http://maps.googleapis.com/maps/api/geocode/json?latlng=" + poiY + "," + poiX + "&sensor=false";
						
						$.ajax ({
							type : 'GET',
							url : geocodeUrl,
							success : function(data) {
								var addr = data.results[0].formatted_address.split(" ");
								
								console.log(data);
								
								if (addr[0] === '대한민국' || addr[addr.length-1] === 'Korea') {
									var placeid_con = "placeId:410.";
								}
								if (addr[1] === '서울특별시' || addr[addr.length-3] === 'Seoul,') {
									var placeid_city = "11.";
									
									if (addr[2] === '종로구' || addr[addr.length-4] === 'Jongno-gu,') {
										var placeid_gun = "110";
									} else if (addr[2] === '성북구' || addr[addr.length-4] === 'Seongbuk-gu,') {
										var placeid_gun = "290";
									} else if (addr[2] === '중구' || addr[addr.length-4] === 'Jung-gu,') {
										var placeid_gun = "140";
									} else if (addr[2] === '용산구' || addr[addr.length-4] === 'Yongsan-gu,') {
										var placeid_gun = "170";
									} else if (addr[2] === '성동구' || addr[addr.length-4] === 'Seongdong-gu,') {
										var placeid_gun = "200";
									} else if (addr[2] === '광진구' || addr[addr.length-4] === 'Gwangjin-gu,') {
										var placeid_gun = "215";
									} else if (addr[2] === '동대문구' || addr[addr.length-4] === 'Dongdarmun-gu,') {
										var placeid_gun = "230";
									} else if (addr[2] === '중랑구' || addr[addr.length-4] === 'Jungnang-gu,') {
										var placeid_gun = "260";
									} else if (addr[2] === '강북구' || addr[addr.length-4] === 'Gangbuk-gu,') {
										var placeid_gun = "305";
									} else if (addr[2] === '도봉구' || addr[addr.length-4] === 'Dobong-gu,') {
										var placeid_gun = "320";
									} else if (addr[2] === '노원구' || addr[addr.length-4] === 'Nowon-gu,') {
										var placeid_gun = "350";
									} else if (addr[2] === '은평구' || addr[addr.length-4] === 'Eunpyeong-gu,') {
										var placeid_gun = "380";
									} else if (addr[2] === '서대문구' || addr[addr.length-4] === 'Seodeamun-gu,') {
										var placeid_gun = "410";
									} else if (addr[2] === '마포구' || addr[addr.length-4] === 'Mapo-gu,') {
										var placeid_gun = "440";
									} else if (addr[2] === '양천구' || addr[addr.length-4] === 'Yangcheon-gu,') {
										var placeid_gun = "470";
									} else if (addr[2] === '강서구' || addr[addr.length-4] === 'Gangseo-gu,') {
										var placeid_gun = "500";
									} else if (addr[2] === '구로구' || addr[addr.length-4] === 'Guro-gu,') {
										var placeid_gun = "530";
									} else if (addr[2] === '금천구' || addr[addr.length-4] === 'Geumcheon-gu,') {
										var placeid_gun = "545";
									} else if (addr[2] === '영등포구' || addr[addr.length-4] === 'Yeongdeungpo-gu,') {
										var placeid_gun = "560";
									} else if (addr[2] === '동작구' || addr[addr.length-4] === 'Dongjak-gu,') {
										var placeid_gun = "590";
									} else if (addr[2] === '관악구' || addr[addr.length-4] === 'Gwanak-gu,') {
										var placeid_gun = "620";
									} else if (addr[2] === '서초구' || addr[addr.length-4] === 'Seocho-gu,') {
										var placeid_gun = "650";
									} else if (addr[2] === '강남구' || addr[addr.length-4] === 'Gangnam-gu,') {
										var placeid_gun = "680";
									} else if (addr[2] === '송파구' || addr[addr.length-4] === 'Songpa-gu,') {
										var placeid_gun = "710";
									} else {
										var placeid_gun = "740";
									}
								}
								
								var placeid = placeid_con + placeid_city + placeid_gun;
								document.getElementById("edit-field-placeid-und-0-value").value = placeid;
								
								if (addr[5]) {
									document.getElementById("edit-field-address-und-0-value").value = addr[0] + " " + addr[1] + " " + addr[2] + " " + addr[3] + " "
																									+ addr[4] + " " + addr[5];
								} else {
									document.getElementById("edit-field-address-und-0-value").value = addr[0] + " " + addr[1] + " " + addr[2] +
																									" " + addr[3] + " " +	addr[4];
								}
							},
							error: function(e) {
								console.log(e);
							}
						});
						
						// Fill geocode field
//						var text_val = document.getElementById("edit-field-geohash-und-0-value").value;
//						if (text_val === "") {
							document.getElementById("edit-field-geohash-und-0-value").value = geohashcode;
//						} else {
//							document.getElementById("edit-field-geohash-und-0-value").value +=  ", " + geohashcode;
//						}
					} else if (features.feature.geometry.CLASS_NAME === "OpenLayers.Geometry.Polygon") {
						var sProj = new OpenLayers.Projection('EPSG:900913');
						var tProj = new OpenLayers.Projection('EPSG:4326');
						
//						console.log(feature_bound);
						
						var centerPointX = features.feature.geometry.bounds.getCenterLonLat().lon;
						var centerPointY = features.feature.geometry.bounds.getCenterLonLat().lat;
						
						var polygonCoord = new OpenLayers.LonLat(centerPointX, centerPointY);
						var proj_centerPoint = polygonCoord.transform(sProj, tProj);

//						console.log(proj_centerPoint);
						
//						console.log(features);
						/*
						var polySize = features.feature.geometry.components[0].components.length;
						
						for (var i=0; i < polySize-1; i++) {
							var pVertaxX = features.feature.geometry.components[0].components[i].x;
							var pVertaxY = features.feature.geometry.components[0].components[i].y;
							
							var pVertax = new OpenLayers.LonLat(pVertaxX, pVertaxY);
							var proj_pVertax = pVertax.transform(sProj, tProj);
							
							var geocodeUrl = "http://maps.googleapis.com/maps/api/geocode/json?latlng=" + proj_pVertax.lat + "," + proj_pVertax.lon + "&sensor=false";
							
							$.ajax ({
								type : 'GET',
								url : geocodeUrl,
								success : function(data) {
//									var addr = data.results[0].formatted_address.split(" ");
									
//									console.log(addr);
						    		var results = new Array();
									var idx = 0;
									var flag = false;
									
									for (var i=0; i<data.results.length-1; i++) {
										results.push(data.results[i].formatted_address.split(" "));
										for (var j=1; j<results.length-1; j++) {
											if (results[j-1][i] !== results[j][i]) {
												flag = true;
												break;
											}
										}
										if (flag) {
											break;
										} else {
											idx = i;
										}
									}
//						    		console.log(results[0][idx]);
								},
								error: function(e) {
									console.log(e);
								}
							});
						}*/
						
						var bl_l = features.feature.geometry.bounds.left;
						var bl_b = features.feature.geometry.bounds.bottom;
						var bottomLeft = new OpenLayers.Geometry.Point(bl_l, bl_b);
						bottomLeft.transform(sProj, tProj);
						
						var tr_r = features.feature.geometry.bounds.right;
						var tr_t = features.feature.geometry.bounds.top;
						var topRight = new OpenLayers.Geometry.Point(tr_r, tr_t);
						topRight.transform(sProj, tProj);
						
						features.object.map.zoomToExtent(features.feature.geometry.getBounds());
						
//						var cZoom = features.object.map.getZoom() * 5;
//						var geohashcode = Fgh.encode(proj_centerPoint.lon, proj_centerPoint.lat, cZoom);
						feature_bound = map_features.feature.geometry.bounds.transform(sProj, tProj);
						
						var geohashcode = polyEncodeGeoHash(proj_centerPoint.lat, proj_centerPoint.lon);
						console.log(geohashcode);
						
						console.log("**** Polygon ****");
						
						var geocodeUrl = "http://maps.googleapis.com/maps/api/geocode/json?latlng=" + proj_centerPoint.lat + "," + proj_centerPoint.lon + "&sensor=false";
						
						$.ajax ({
							type : 'GET',
							url : geocodeUrl,
							success : function(data) {
								var addr = data.results[0].formatted_address.split(" ");
								
								if (addr[0] === '대한민국' || addr[addr.length-1] === 'Korea') {
									var placeid_con = "placeId:410.";
								}
								if (addr[1] === '서울특별시' || addr[addr.length-3] === 'Seoul,') {
									var placeid_city = "11.";
									
									if (addr[2] === '종로구' || addr[addr.length-4] === 'Jongno-gu,') {
										var placeid_gun = "110";
									} else if (addr[2] === '성북구' || addr[addr.length-4] === 'Seongbuk-gu,') {
										var placeid_gun = "290";
									} else if (addr[2] === '중구' || addr[addr.length-4] === 'Jung-gu,') {
										var placeid_gun = "140";
									} else if (addr[2] === '용산구' || addr[addr.length-4] === 'Yongsan-gu,') {
										var placeid_gun = "170";
									} else if (addr[2] === '성동구' || addr[addr.length-4] === 'Seongdong-gu,') {
										var placeid_gun = "200";
									} else if (addr[2] === '광진구' || addr[addr.length-4] === 'Gwangjin-gu,') {
										var placeid_gun = "215";
									} else if (addr[2] === '동대문구' || addr[addr.length-4] === 'Dongdarmun-gu,') {
										var placeid_gun = "230";
									} else if (addr[2] === '중랑구' || addr[addr.length-4] === 'Jungnang-gu,') {
										var placeid_gun = "260";
									} else if (addr[2] === '강북구' || addr[addr.length-4] === 'Gangbuk-gu,') {
										var placeid_gun = "305";
									} else if (addr[2] === '도봉구' || addr[addr.length-4] === 'Dobong-gu,') {
										var placeid_gun = "320";
									} else if (addr[2] === '노원구' || addr[addr.length-4] === 'Nowon-gu,') {
										var placeid_gun = "350";
									} else if (addr[2] === '은평구' || addr[addr.length-4] === 'Eunpyeong-gu,') {
										var placeid_gun = "380";
									} else if (addr[2] === '서대문구' || addr[addr.length-4] === 'Seodeamun-gu,') {
										var placeid_gun = "410";
									} else if (addr[2] === '마포구' || addr[addr.length-4] === 'Mapo-gu,') {
										var placeid_gun = "440";
									} else if (addr[2] === '양천구' || addr[addr.length-4] === 'Yangcheon-gu,') {
										var placeid_gun = "470";
									} else if (addr[2] === '강서구' || addr[addr.length-4] === 'Gangseo-gu,') {
										var placeid_gun = "500";
									} else if (addr[2] === '구로구' || addr[addr.length-4] === 'Guro-gu,') {
										var placeid_gun = "530";
									} else if (addr[2] === '금천구' || addr[addr.length-4] === 'Geumcheon-gu,') {
										var placeid_gun = "545";
									} else if (addr[2] === '영등포구' || addr[addr.length-4] === 'Yeongdeungpo-gu,') {
										var placeid_gun = "560";
									} else if (addr[2] === '동작구' || addr[addr.length-4] === 'Dongjak-gu,') {
										var placeid_gun = "590";
									} else if (addr[2] === '관악구' || addr[addr.length-4] === 'Gwanak-gu,') {
										var placeid_gun = "620";
									} else if (addr[2] === '서초구' || addr[addr.length-4] === 'Seocho-gu,') {
										var placeid_gun = "650";
									} else if (addr[2] === '강남구' || addr[addr.length-4] === 'Gangnam-gu,') {
										var placeid_gun = "680";
									} else if (addr[2] === '송파구' || addr[addr.length-4] === 'Songpa-gu,') {
										var placeid_gun = "710";
									} else {
										var placeid_gun = "740";
									}
								}
								var placeid = placeid_con + placeid_city + placeid_gun;
								document.getElementById("edit-field-placeid-und-0-value").value = placeid;
								
								if (addr[5]) {
									document.getElementById("edit-field-address-und-0-value").value = addr[0] + " " + addr[1] + " " + addr[2] + " " + addr[3] + " "
																									+ addr[4] + " " + addr[5];
								} else {
									document.getElementById("edit-field-address-und-0-value").value = addr[0] + " " + addr[1] + " " + addr[2] +
																									" " + addr[3] + " " +	addr[4];
								}
							},
							error: function(e) {
								console.log(e);
							}
						});
						document.getElementById("edit-field-geohash-und-0-value").value = geohashcode;
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
