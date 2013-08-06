<?php

/**
 * @file
 * Provides field widget hooks for DPOI module.
 */

/**
 * Implements hook_field_widget_info().
 */
function dpoi_field_widget_info() {
	$widgets = array();

	// OpenLayers dependant widget
	if (module_exists('openlayers')) {
		$widgets['dpoi_openlayers'] = array(
				'label' => t('DPOI Openlayers Map'),
				'field types' => array('geofield'),
		);
	}
	return $widgets;
}

/**
 * Implements hook_field_widget_settings_form().
 */
function dpoi_field_widget_settings_form($field, $instance) {
	$widget = $instance['widget'];
	$settings = $widget['settings'];
	$form = array();

	if ($widget['type'] == 'dpoi_openlayers') {

		// Get preset options, filtered to those which have the GeoField behavior and *don't* have the draw features behavior, which is incompatible
		$maps = openlayers_maps();
		$map_options = array();
		foreach ($maps as $map) {
			if (array_key_exists('openlayers_behavior_dpoi', $map->data['behaviors']) && !array_key_exists('openlayers_behavior_drawfeatures', $map->data['behaviors'])) {
				$map_options[$map->name] = $map->title;
			}
		}

		if (empty($map_options)) {
			form_set_error('openlayers_map', "Error: You have no compatible openlayers maps. Make sure that at least one preset has the 'DPOI' behavior enabled and that it does not have the 'Draw Features' behavior enabled (which is incompatible).");
		}

		$form['openlayers_map'] = array(
				'#type' => 'select',
				'#title' => t('OpenLayers Map'),
				'#default_value' => isset($settings['openlayers_map']) ? $settings['openlayers_map'] : 'dpoi_widget_map',
				'#options' => $map_options,
				'#description' => t('Select which OpenLayers map you would like to use. Only maps which have the DPOI behavior may be selected. If your preferred map is not here, add the DPOI behavior to it first. The "Draw Features" bahavior is incompatible - presets with this behavior are not shown.'),
		);

		$form['data_storage'] = array(
				'#type' => 'radios',
				'#title' => t('Storage Options'),
				'#description' => t('Should the widget only allow simple features (points, lines, or polygons), or should the widget allow for complex features? Note that changing this setting from complex to simple after data has been entered can lead to data loss.'),
				'#options' => array(
						'collection' => 'Store as a single collection.',
						'single' => 'Store each simple feature as a separate field.',
				),
				'#default_value' => (isset($settings['data_storage'])) ? $settings['data_storage'] : 'collection',
		);
	}
	return $form;
}

/**
 * Implements hook_field_widget_form().
 */
function dpoi_field_widget_form(&$form, &$form_state, $field, $instance, $langcode, $items, $delta, $base) {

	$widget = $instance['widget'];
	$settings = $widget['settings'];
	$element = dpoi_get_base_element($base, $items, $delta);

	if ($widget['type'] == 'dpoi_openlayers') {
		$openlayers_map_id = !empty($instance['widget']['settings']['openlayers_map']) ? $instance['widget']['settings']['openlayers_map'] : 'dpoi_widget_map';
		$element['#openlayers_mapname'] = $openlayers_map_id;
		$element['#after_build']= array('dpoi_widget_openlayers_afterbuild');
	}

	return $element;
}

/**
 * Callback for afterbuild for widget for js addition to
 */

function dpoi_widget_openlayers_afterbuild($element, &$form_state) {
	drupal_add_js(
	array('geofield' => array(
	'data_storage' => (!empty($settings['data_storage'])) ? $settings['data_storage'] : 'collection',
	),
	),
	'setting');

	$defaults = array();
	$element['helpmap'] = array(
			'#markup' => '<div class="form-item geotaxonomy-latlon-helpmap" style="display:block">'
			. '<table style="width:1020px;">'
			. '<tr><td colspan=2>'
			. '<label>* 주소 및 키워드 검색은 사용자가 찾고자 하는 위치로 지도를 손쉽게 이동시키기 위한 가이드기능입니다.<br>&nbsp;&nbsp;저작을 하고자 하는 위치는 지도의 우측상단에 있는 아이콘을 이용하여 저작하세요.</label>'
			. '<font color=#FF0000><h5>* 현재 다수의 관심지점을 저작하는 것은 지원되지 않습니다.</h5></font>'
			. '</td></tr>'
			. '<td valign="baseline" style="width:350px; height:490px;">'
			. '<div><label for="name">주소 검색</label>'
			. '<input type="text" id="address" style="border:1px solid #CCC;" size="30" onkeypress="if(event.keyCode == 13){codeAddress(); return false;}" />'
			. '<input type="button" style="border: 2px outset buttonface; padding: 0.5px 5px 0.5px;" value="검색" onclick="javascript:codeAddress();"/>'
// 			. '</td></tr></table>'
			. '</div><p>* 주소를 입력하면 해당 주소에 해당하는 지점으로 <br>&nbsp;&nbsp;지도의 중심이 이동합니다.</p><br>'
// 			. '<table>'
// 			. '<tr style="height:50px;">'
			. '<div><label for="name">키워드 검색</label>'
			. '<input type="text" id="input_value" style="border:1px solid #CCC;" size="30" onkeypress="if(event.keyCode == 13){Dpoi_search.keywordsView(); return false;}" />'
			. '<input type="button" id="set_area_search" style="border: 2px outset buttonface; padding: 0.5px 5px 0.5px;" value="검색" onclick="javascript:Dpoi_search.keywordsView();"/>'
// 			. '</td></tr></table>'
// 			. '<table>'
// 			. '<tr style="height:50px;">'
			. '</div><p>* 관심지점 이름이 입력한 키워드를 포함하는 경우<br>&nbsp;&nbsp;해당 결과들을 보여줍니다.(30번까지만 아이콘이 표시)<br>* 검색된 결과의 이름을 클릭하면 <br>&nbsp;&nbsp;해당 지점이 지도의 중심으로 이동합니다.</p>'
			. '<div id="container" style="height:300px;"></div></td>'
			. '<td>'
			. dpoi_form_latlon_map(array(), $element['#openlayers_mapname'])
			. '</td></tr></table>'
			. '</div>'
			/*
			// Category select (Triple dropdown)
			. '<div>'
			. '<select name="slist1" style="border:1px solid #CCC; margin-right:15px; margin-top:5px;" id="cat1" onchange="SList.getSelect(\'slist2\', this.value);">
					<option>Select Category</option>
					<option value="자연">자연</option>
					<option value="인문(문화/예술/역사)">인문(문화/예술/역사)</option>
					<option value="레포츠">레포츠</option>
					<option value="쇼핑">쇼핑</option>
					<option value="음식">음식</option>
					<option value="교통">교통</option>
					<option value="숙박">숙박</option>
				</select>'
			. '<span id="slist2"></span><span id="slist3"></span><div id="scontent"></div>'
			. '<script>
				var SList = new Object();

				SList.slist2 = {
				 "자연": [\'자연관광지\', \'관광자원\'],
				 "인문(문화/예술/역사)": [\'역사관광지\', \'휴양관광지\', \'체험관광지\', \'산업관광지\', \'건축/조형물\', \'문화시설\', \'축제\', \'공연/행사\'],
				 "레포츠": [\'레포츠소개\', \'육상 레포츠\', \'수상 레포츠\', \'항공 레포츠\', \'복합 레포츠\'],
				 "쇼핑": [\'쇼핑\'],
				 "음식": [\'음식소개\', \'음식점\'],
				 "교통": [\'교통수단\', \'교통시설\'],
				 "숙박": [\'숙박시설\'],
				};

				SList.slist3 = {
				 "자연관광지": [\'국립공원\', \'도립공원\', \'군립공원\', \'산\', \'자연생태관광지\', \'자연휴양림\', \'수목원\', \'폭포\', \'계곡\', \'약수터\', \'해안절경\', \'해수욕장\', \'섬\', \'항구/포구\', \'어촌\', \'등대\', \'호수 \', \'강\', \'동굴\'],
				 "관광자원": [\'희귀동,식물\', \'기암괴석\'],
				 "역사관광지": [\'고궁\', \'성\', \'문\', \'고택\', \'생가\', \'민속마을\', \'유적지/사적지\', \'사찰\', \'종교성지\', \'안보관광\'],
				 "휴양관광지": [\'유원지\', \'관광단지\', \'온천/욕장/스파\', \'이색찜질방\', \'헬스투어\', \'테마공원\', \'공원\', \'유람선/잠수함관광\'],
				 "체험관광지": [\'농.산.어촌체험\', \'전통체험\', \'산사체험\', \'이색체험\', \'관광농원\', \'이색거리\'],
				 "산업관광지": [\'제철소\', \'조선소\', \'공단\', \'발전소\', \'광산\'],
				 "건축/조형물": [\'다리/대교\', \'기념탑/기념비/전망대\', \'분수\', \'동산\', \'터널\', \'유명건물\'],
				 "문화시설": [\'박물관\', \'기념관\', \'전시관\', \'컨벤션센터\', \'미술관/화랑\', \'공연장\', \'문화원\', \'외국문화원\', \'도서관\', \'대형서점\', \'문화전수시설\', \'영화관\', \'어학당\', \'학교\'],
				 "축제": [\'문화관광축제\', \'일반축제\'],
				 "공연/행사": [\'전통공연\', \'연극\', \'뮤지컬\', \'오페라\', \'전시회\', \'박람회\', \'컨벤션\', \'무용\', \'클래식음악회\', \'대중콘서트\', \'영화\', \'스포츠경기\', \'기타행사\'],
				 "레포츠소개": [\'육상레포츠\', \'수상레포츠\', \'항공레포츠\'],
				 "육상 레포츠": [\'스포츠센터\', \'수련시설\', \'경기장\', \'인라인(실내 인라인 포함)\', \'자전거하이킹\', \'카트\', \'골프\', \'경마\', \'경륜\', \'카지노\', \'승마\', \'스키/스노보드\', \'스케이트\', \'썰매장\', \'수렵장\', \'사격장\', \'야영장/오토캠핑장\', \'암벽등반\', \'빙벽등반\', \'서바이벌게임\', \'ATV\', \'MTB\', \'오프로드\', \'번지점프\', \'자동차경주\'],
				 "수상 레포츠": [\'윈드서핑/제트스키\', \'카약/카누\', \'오트\', \'스노쿨링/스킨스쿠버다이빙\', \'민물낚시\', \'바다낚시\', \'수영\', \'래프팅\'],
				 "항공 레포츠": [\'스카이다이빙\', \'초경량비행\', \'행글라이딩/패러글라이딩\', \'열기구\'],
				 "복합 레포츠": [\'복합 레포츠\'],
				 "쇼핑": [\'5일장\', \'상설시장\', \'백화점\', \'면세점\', \'할인매장\', \'전문상가\', \'공예/공방\', \'관광기념품점\', \'특산물판매점\'],
				 "음식소개": [\'한국요리소개\', \'궁중음식\', \'전통차\', \'한과\', \'전통주/민속주\', \'지역음식\', \'조리법\', \'식사법\'],
				 "음식점": [\'한식\', \'서양식\', \'일식\', \'중식\', \'아시아식\', \'패밀리레스토랑\', \'이색음식점\', \'채식전문점\', \'바/카페\', \'클럽\'],
				 "교통수단": [\'항공\', \'철도\', \'지하철\', \'버스\', \'선박\', \'렌터카\', \'택시\'],
				 "교통시설": [\'공항\', \'기차역\', \'버스터미널\', \'여객선터미널\', \'고속도로휴게소\'],
				 "숙박시설": [\'관광호텔\', \'수상관광호텔\', \'전통호텔\', \'가족호텔\', \'콘도미니엄\', \'유스호스텔\', \'팬션\', \'여관\', \'모텔\', \'민박\', \'게스트하우스\', \'홈스테이\', \'서비스드레지던스\'],
				};
		
				SList.getSelect = function(slist, option) {
					document.getElementById(\'scontent\').innerHTML = \'\';

					if(SList[slist][option]) {
						if(slist == \'scontent\') { 
							document.getElementById(\'scontent\').innerHTML = SList[slist][option];
						} else {
							var addata = \'<option>Select Option</option>\';
							
							for(var i=0; i<SList[slist][option].length; i++) {
								addata += \'<option value="\'+SList[slist][option][i]+\'">\'+SList[slist][option][i]+\'</option>\';
							}

							switch(slist) {
								case \'slist2\':
									document.getElementById(\'slist2\').innerHTML = \'<select name="slist2" id="cat2" style="border:1px solid #CCC; margin-right:15px;" onchange="SList.getSelect(\\\'slist3\\\', this.value);">\'+addata+\'</select>\';
									document.getElementById(\'slist3\').innerHTML = \'\';
									break;
								case \'slist3\':
									document.getElementById(\'slist3\').innerHTML = \'<select name="slist3" id="cat3" style="border:1px solid #CCC;" onchange="getCat();">\'+addata+\'</select>\';
									break;
							}
						}
					} else {
						if(slist == \'slist2\') {
							document.getElementById(\'slist2\').innerHTML = \'\';
							document.getElementById(\'slist3\').innerHTML = \'\';
						} else if(slist == \'slist3\') {
							document.getElementById(\'slist3\').innerHTML = \'\';
						}
					}
				}
				function getCat(value) {
					var cat = document.getElementById("cat1").value + " / " + document.getElementById("cat2").value + " / " + document.getElementById("cat3").value;
					document.getElementById("edit-field-category-und-0-value").value = cat;
				}
				</script>'
			.'</div>'*/
	);
	return $element;
}

/**
 * Create LatLon Helper Map.
 */
function dpoi_form_latlon_map($defaults = array(), $map_name) {
	$map_data = openlayers_map_load($map_name);
	$map = $map_data->data;
	return openlayers_render_map($map);
}

function dpoi_get_base_element($base, $items, $delta) {
	$element = $base;

	$element['wkt'] = array(
			'#type' => 'hidden',
			'#attributes' => array('class' => array('geofield_wkt')),
			'#default_value' => isset($items[$delta]['wkt']) ? $items[$delta]['wkt'] : NULL,
	);
	$element['input_format'] = array(
			'#type' => 'value',
			'#attributes' => array('class' => array('geofield_input_format')),
			'#value' => 'wkt',
	);
	$element['geo_type'] = array(
			'#type' => 'hidden',
			'#attributes' => array('class' => array('geofield_geo_type')),
			'#default_value' => isset($items[$delta]['geo_type']) ? $items[$delta]['geo_type'] : NULL,
	);
	$element['lat'] = array(
			'#type' => 'hidden',
			'#attributes' => array('class' => array('geofield_lat')),
			'#default_value' => isset($items[$delta]['lat']) ? $items[$delta]['lat'] : NULL,
	);
	$element['lon'] = array(
			'#type' => 'hidden',
			'#attributes' => array('class' => array('geofield_lon')),
			'#default_value' => isset($items[$delta]['lon']) ? $items[$delta]['lon'] : NULL,
	);
	$element['left'] = array(
			'#type' => 'hidden',
			'#attributes' => array('class' => array('geofield_left')),
			'#default_value' => isset($items[$delta]['left']) ? $items[$delta]['left'] : NULL,
	);
	$element['right'] = array(
			'#type' => 'hidden',
			'#attributes' => array('class' => array('geofield_right')),
			'#default_value' => isset($items[$delta]['right']) ? $items[$delta]['right'] : NULL,
	);
	$element['bottom'] = array(
			'#type' => 'hidden',
			'#attributes' => array('class' => array('geofield_bottom')),
			'#default_value' => isset($items[$delta]['bottom']) ? $items[$delta]['bottom'] : NULL,
	);
	$element['top'] = array(
			'#type' => 'hidden',
			'#attributes' => array('class' => array('geofield_top')),
			'#default_value' => isset($items[$delta]['top']) ? $items[$delta]['top'] : NULL,
	);

	$element['description'] = array(
			'#markup' => (!empty($element['#description'])) ? '<div class="description">' . $element['#description'] . '</div>' : '',
	);

	// Master column is used by element-validate to decide which set of columns it should use to compute all other values.
	// By default, wkt is the master-column, all we compute all other values from it. For other widget (such as lat/lon) this will be different
	$element['master_column'] = array(
			'#type' => 'hidden',
			'#value' => 'wkt',
	);

	// This validate function computes all other columns from the master field
	$element['#element_validate'] = array('dpoi_element_validate');

	return $element;
}

/**
 * DPOI Element Validate
 *
 */
function dpoi_element_validate($element, &$form_state) {
	$master_column = $element['master_column']['#value'];

	$values = array(
			'wkt' => $element['wkt']['#value'],
			'lat' => $element['lat']['#value'] ? geofield_latlon_DMStoDEC($element['lat']['#value']) : '',
			'lon' => $element['lon']['#value'] ? geofield_latlon_DMStoDEC($element['lon']['#value']) : '',
			'top' => $element['top']['#value'],
			'bottom' => $element['bottom']['#value'],
			'right' => $element['right']['#value'],
			'left' => $element['left']['#value'],
	);

	// Because we have an odd flow to filter potential GeoJSON, we need a flag to determine whether or not to process.
	// @TODO: This could be expanded to cover WKT as well.
	$geo_process = "PROCESS";

	if ($element['input_format']['#value'] == 'geojson') {
		// For geojson input, we've hijacked the wkt value.
		geophp_load();
		try {
			$geometry = geoPHP::load($values['wkt'], 'json');
			$values['wkt'] = $geometry->out('wkt');
		} catch (Exception $e) {
			form_set_error($element['wkt']['#name'], 'Please enter valid GeoJSON');
			$geo_process = '';
		}
	}
	
	if ($geo_process == "PROCESS") {
		geofield_compute_values($values, $master_column);
	
		// Set form values from the $values array
		if (isset($values['wkt']))       form_set_value($element['wkt'], $values['wkt'], $form_state);
		if (isset($values['geo_type']))  form_set_value($element['geo_type'], $values['geo_type'], $form_state);
		if (isset($values['lat']))       form_set_value($element['lat'], $values['lat'], $form_state);
		if (isset($values['lon']))       form_set_value($element['lon'], $values['lon'], $form_state);
		if (isset($values['top']))       form_set_value($element['top'], $values['top'], $form_state);
		if (isset($values['bottom']))    form_set_value($element['bottom'], $values['bottom'], $form_state);
		if (isset($values['right']))     form_set_value($element['right'], $values['right'], $form_state);
		if (isset($values['left']))      form_set_value($element['left'], $values['left'], $form_state);
	}
}