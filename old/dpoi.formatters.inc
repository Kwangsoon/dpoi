<?php 
/**
 * @file
 * Module implementing a field-collection openlayers formatter.
 */

/**
 * Implements hook_field_formatter_info().
 */
function dpoi_field_formatter_info() {
	
	if (module_exists('openlayers')) {
		$formatters['dpoii_openlayers'] = array (
				'label' => t('OpenLayers for DPOI'),
				'field types' => array('geofield'),
				'settings' => array('data' => 'full', 'map_preset' => 'dpoi_formatter_map'),
		);
	}
	return $formatters;
}

/**
 * Implements hook_field_formatter_settings_form().
 */
function dpoi_formatter_settings_form($field, $instance, $view_mode, $form, &$form_state) {
	$display = $instance['display'][$view_mode];
	$settings = $display['settings'];

	// Map preset formatter
	if ($display['type'] == 'dpoii_openlayers' && module_exists('openlayers')) {
		$presets = openlayers_presets();
		$preset_options = array();
		foreach ($presets as $preset) {
			if (in_array('dpoi_formatter', $preset->data['layers'])) {
				$preset_options[$preset->name] = $preset->title;
			}
		}

		$element['map_preset'] = array(
				'#title' => t('OpenLayers Preset'),
				'#type' => 'select',
				'#default_value' => $settings['map_preset'] ? $settings['map_preset'] : 'dpoi_formatter_map',
				'#required' => TRUE,
				'#options' => $preset_options,
				'#description' => t('Select which OpenLayers map you would like to use. Only maps which have the dpoi placeholder layer may be selected. If your preferred map is not here, add the dpoi placeholder layer to it first.'),
		);
	}

	if ($display['type'] == 'geofield_latlon') {
		$element['labels'] = array(
				'#title' => t('Display Labels'),
				'#type' => 'checkbox',
				'#default_value' => $settings['labels'],
		);
	}
	return $element;
}


/**
 * Implements hook_field_formatter_view().
 */
function dpoi_field_formatter_view($entity_type, $entity, $field, $instance, $langcode, $items, $display) {
	$element = array();

	// Transform into centroid or bounding if needed
	if ($display['settings']['data'] != 'full') {
		geophp_load();
		if ($display['settings']['data'] == 'centroid') {
			foreach ($items as $delta => $item) {
				$centroid_wkt = 'POINT(' . $item['lon'] . ' ' . $item['lat'] . ')';
				$centroid = geoPHP::load($centroid_wkt, 'wkt');
				$items[$delta] = geofield_get_values_from_geometry($centroid);
			}
		}
		if ($display['settings']['data'] == 'bounding') {
			foreach ($items as $delta => $item) {
				$envelope_wkt = 'POLYGON ((' . $item['left'] . ' ' . $item['top'] . ', ' . $item['right'] . ' ' . $item['top'] . ', ' . $item['right'] . ' ' . $item['bottom'] . ', ' . $item['left'] . ' ' . $item['bottom'] . ', ' . $item['left'] . ' ' . $item['top'] . ')))';
				$envelope = geoPHP::load($envelope_wkt, 'wkt');
				$items[$delta] = geofield_get_values_from_geometry($envelope);
			}
		}
	}

	if ($display['type'] == 'dpoii_openlayers') {
		$map_name = $display['settings']['map_preset'] ? $display['settings']['map_preset'] : 'dpoi_formatter_map';
		$element[0] = array('#markup' => _dpoi_openlayers_formatter($map_name, $items)
				. '<label for="name">Input keyword whatever you want</label>'
		);
		return $element;
	}
	return $element;
}

function _dpoi_openlayers_formatter($map_name, $items) {
	$features = array();

	foreach ($items as $delta) {
		$features[] = array(
				'wkt' => $delta['wkt'],
				'projection' => '4326',
		);
	}

	$preset = openlayers_preset_load($map_name);
	$map = openlayers_build_map($preset->data);

	if (!isset($map['layers']['dpoi_formatter'])) {
		drupal_set_message(t('Trying to render a dpoi formatter on a map without the placeholder layer'), 'error');
	}

	$map['layers']['dpoi_formatter']['features'] = $features;

	if (empty($map['errors'])) {
		$js = array('openlayers' => array('maps' => array($map['id'] => $map)));
		drupal_add_js($js, 'setting');

		$output = theme('openlayers_map', array(
				'map' => $map,
				'map_name' => $map_name
		));
	}
	return $output;
}