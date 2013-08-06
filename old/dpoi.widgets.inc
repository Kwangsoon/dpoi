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
			. '<table style="width:1020px;"><tr><td valign="baseline" style="width:350px;">'
			. '<table>'
			. '<tr style="height:50px;">'
			. '<td><label for="name">Input keyword whatever you want</label>'
			. '<input type="text" id="input_value" style="border:1px solid #CCC;" size="30" onkeypress="if(event.keyCode == 13){Dpoi_search.keywordsView(); return false;}" />'
			. '<input type="button" id="set_area_search" style="border: 2px outset buttonface; padding: 0.5px 5px 0.5px;" value="Search" onclick="javascript:Dpoi_search.keywordsView();"/>'
			. '</td></tr></table>'
			. '<table style="height:300px;"><tr id="poiList"></tr></table>'
			. '</td>'
			. '<td>'
			. dpoi_form_latlon_map(array(), $element['#openlayers_mapname'])
			. '</td></tr></table>'
			. '</div>'
	);

	$element['helpmap_desc'] = array(
			'#markup' => t('<div class="description geofield-help">Use the icons to select what type of feature to draw. Each map can contain one simple feature. Pan and zoom with arrows and the zoom bar.</div>')
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