
var Dpoi_search = {

		poi_setCenter : function (poi_x, poi_y) {	
			var lonlat = new OpenLayers.LonLat(poi_x, poi_y);
			dpoi_map.setCenter(lonlat);
		},

		keywordsView : function () {
			var sProj = new OpenLayers.Projection('EPSG:900913');
			var tProj = new OpenLayers.Projection('EPSG:4326');
			
			var keywords = document.getElementById("input_value").value;
			var pos = location.href.indexOf("/node/");
			var ajaxSearch;
			
			var mapBbox = dpoi_map.getExtent();
			mapBbox.transform(sProj, tProj);

			var topLeft_l = mapBbox.left;
			var topLeft_t = mapBbox.top;
			var topLeft = new OpenLayers.Geometry.Point(topLeft_l, topLeft_t);
			var tl = topLeft.x + "," + topLeft.y;

			var bottomRight_r = mapBbox.right;
			var bottomRight_b = mapBbox.bottom;
			var bottomRight = new OpenLayers.Geometry.Point(bottomRight_r, bottomRight_b);

			var br = bottomRight.x + "," + bottomRight.y;

			ajaxSearch = location.href.substr(0,pos) + "/smart_trail/dpoi_search?keys=" + keywords + "&num=8&page=1&bbox=" + tl + "," + br;
			
			var form_text = "<div id='poiList'><div class='page_navigation'></div><ul class='content'>";
			var html_form = document.getElementById("poiList");
			
			if (markers !== null) {
				markers.clearMarkers();
			}
			
			var http = new XMLHttpRequest();
			http.open("GET", ajaxSearch, true);
			http.onreadystatechange = function() {
				if (http.readyState == 4 && http.status == 200) {
					var result = JSON.parse(http.responseText);
					
					if (result === undefined) {
						alert("No result. Check your keyword.")
						return;
					}
					
					for (var i=0; i<result.length; i++) {
						var poi_x = result[i].poi_mapx;
						var poi_y = result[i].poi_mapy;

						var point = new OpenLayers.Geometry.Point(poi_x, poi_y);
						point.transform(tProj, sProj);
						
			            var size = new OpenLayers.Size(21,25);
			            var offset = new OpenLayers.Pixel(-(size.w/2), -size.h);
			            var icon = new OpenLayers.Icon('http://www.imrc.kist.re.kr/~procarrie/icons/firstaid.png',size,offset);
			            markers.addMarker(new OpenLayers.Marker(new OpenLayers.LonLat(point.x, point.y),icon));
//			            dpoi_map.zoomToMaxExtent();

						form_text += "<li onclick='javascript:Dpoi_search.poi_setCenter(" + point.x + ", " + point.y + ");'><label>" + result[i].poi_poititle + "</label>" +
												"<span>" + result[i].poi_address + "</span></li>";
						html_form.innerHTML = form_text;
					}
					form_text += "</ul></div>";
				}
			};
			http.send(null);
			return;
		},
};