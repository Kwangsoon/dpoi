
var Dpoi_search = {
//		map: null, 
		drawControls: null, 
		selectControl: null, 
		selectedFeature: null,
		
		poi_setCenter : function (poi_x, poi_y) {	
			var lonlat = new OpenLayers.LonLat(poi_x, poi_y);
			dpoi_map.setCenter(lonlat, 18);
			
			console.log(dpoi_map);
			console.log(map_features);
			
			selectedFeature = map_features;
            popup = new OpenLayers.Popup.FramedCloud("chicken", 
                                     lonlat,
                                     null,
                                     "<div style='font-size:.8em'>Feature: " + feature.id +"<br>Area: " + feature.geometry.getArea()+"</div>",
                                     null, true, onPopupClose);
            feature.popup = popup;
            dpoi_map.addPopup(popup);
		},
		
//        onPopupClose function (evt) {
//            selectControl.unselect(selectedFeature);
//        },
        
//        onFeatureUnselect function (feature) {
//            map.removePopup(feature.popup);
//            feature.popup.destroy();
//            feature.popup = null;
//        },
		
		keywordsView : function () {
			var sProj = new OpenLayers.Projection('EPSG:900913');
			var tProj = new OpenLayers.Projection('EPSG:4326');

			var keywords = document.getElementById("input_value").value;
			var ajaxSearch;

			var mapBbox = dpoi_map.getExtent();
			mapBbox.transform(sProj, tProj);

			var bottomLeft_l = mapBbox.left;
			var bottomLeft_b = mapBbox.bottom;
			var bottomLeft = new OpenLayers.Geometry.Point(bottomLeft_l, bottomLeft_b);
			var bl = bottomLeft.y + "," + bottomLeft.x;

			var topRight_r = mapBbox.right;
			var topRight_t = mapBbox.top;
			var topRight = new OpenLayers.Geometry.Point(topRight_r, topRight_t);

			var tr = topRight.y + "," + topRight.x;
			
			var html_form = document.getElementById("container");

			if (markers !== null) {
				markers.clearMarkers();
			}

			ajaxSearch = "http://dpoi.imrc.kist.re.kr/select/?q=title%3A" + keywords + "&fq=location_point%3A[" + bl + "%20TO%20" + tr + "]&version=2.2&start=0&rows=1000&indent=on&wt=json&json.wrf=?";

			jQuery.getJSON(ajaxSearch, function(result){
				console.log(result);
				
				if (result.response.docs.length === 0) {
					alert("No result. Check your keyword or location.")
					return;
				}

				var form_text = "<div class='page_navigation'></div><br><ul class='content'>";
				
				for (var i=0; i<result.response.numFound; i++) {
					var poiPoint = result.response.docs[i].location_point.split(",");
					var pos = new OpenLayers.Geometry.Point(poiPoint[1], poiPoint[0]);

					pos.transform(tProj, sProj);

					var size = new OpenLayers.Size(21,25);
					var offset = new OpenLayers.Pixel(-(size.w/2), -size.h);
					var icon = new OpenLayers.Icon('http://www.imrc.kist.re.kr/~procarrie/icons/' + (i+1) + '.png',size,offset);
					markers.addMarker(new OpenLayers.Marker(new OpenLayers.LonLat(pos.x, pos.y),icon));

					form_text += "<li type='none' onMouseOver=\"this.style.background='#E2E2F6'\" onMouseOut=\"this.style.background='#FFFFFF'\">"
							+ (i+1) + ". " + "<span class='label' onclick='javascript:Dpoi_search.poi_setCenter(" + pos.x + ", " + pos.y + ");'>"
							+ "<strong>" + result.response.docs[i].title + "</strong></span><br>"
							+ "<span>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;" + result.response.docs[i].dpoi_address + "</span></li>";
				}
				form_text += "</ul>";
				html_form.innerHTML = form_text;
				
				jQuery("#container").pajinate({
					num_page_links_to_display : 6,
					items_per_page : 6
				});
			});
			return;
		},
};