
(function () {
	var _tr = "0123456789bcdefghjkmnpqrstuvwxyz";
	var _dm = [0, 1, 0, 1, 2, 3, 2, 3, 0, 1, 0, 1, 2, 3, 2, 3, 
	           4, 5, 4, 5, 6, 7, 6, 7, 4, 5, 4, 5, 6, 7, 6, 7];

	var _dr = [0, 1, 4, 5, 16, 17, 20, 21, 64, 65, 68, 69, 80,
	           81, 84, 85, 256, 257, 260, 261, 272, 273, 276, 277,
	           320, 321, 324, 325, 336, 337, 340, 341];

	function _cmb (str, pos) {
		return (_tr.indexOf(str.charAt(pos)) << 5) | (_tr.indexOf(str.charAt(pos+1)));
	};

	function _unp(v) {
		return _dm[v & 0x1F] | (_dm[(v >> 6) & 0xF] << 3);
	}

	function _sparse (val) {
		var acc = 0, off = 0;

		while (val > 0) {
			low = val & 0xFF;
			acc |= _dr[low] << off;
			val >>= 8;
			off += 16;
		}
		return acc;
	}

	window['Fgh'] = {
			decode: function (str) {
	            var L = str.length, i, w, ln = 0.0, lt = 0.0;

	            // Get word; handle odd size of string.
	            if (L & 1) {
	                w = (_tr.indexOf(str.charAt(L-1)) << 5);
	            } else {
	                w = _cmb(str, L-2);
	            }
	            lt = (_unp(w)) / 32.0;
	            ln = (_unp(w >> 1)) / 32.0;
	            
	            for (i=(L-2) & ~0x1; i>=0; i-=2) {
	                w = _cmb(str, i);
	                lt = (_unp(w) + lt) / 32.0;
	                ln = (_unp(w>>1) + ln) / 32.0;
	            }
	            return {lat:  180.0*(lt-0.5), lon: 360.0*(ln-0.5)};
	        },
			encode: function (lon, lat, bits) {
				lat = lat/180.0+0.5;
				lon = lon/360.0+0.5;

				var r = '', l = Math.ceil(bits/10), hlt, hln, b2, hi, lo, i;

				for (i = 0; i < l; ++i) {
					lat *= 0x20;
					lon *= 0x20;

					hlt = Math.min(0x1F, Math.floor(lat));
					hln = Math.min(0x1F, Math.floor(lon));

					lat -= hlt;
					lon -= hln;

					b2 = _sparse(hlt) | (_sparse(hln) << 1);

					hi = b2 >> 5;
					lo = b2 & 0x1F;

					r += _tr.charAt(hi) + _tr.charAt(lo);
				}

				r = r.substr(0, Math.ceil(bits/5));
				return r;
			},

			checkValid: function(str) {
				return !!str.match(/^[0-9b-hjkmnp-z]+$/);
			}
	}
})();