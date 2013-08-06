<?php

/**
 * @file
 * Provides hooks for integration with OpenLayers (http://drupal.org/project/openlayers)
 */

/**
 * Implements hook_ctools_plugin_type().
 */
function dpoi_ctools_plugin_type() {
	return array(
			'behaviors' => array(
					'use hooks' => TRUE,
			)
	);
}

/**
 * Implements hook_ctools_plugin_api().
 */
function dpoi_ctools_plugin_api($module, $api) {
	return array('version' => 1);
}

function dpoi_ctools_plugin_directory($module, $plugin) {
	if ($plugin == 'content_types' && !empty($plugin)) {
		return 'includes/' . $plugin;
	}
	if ($module == 'openlayers' && !empty($plugin)) {
		return 'plugins/' . $plugin;
	}
}

/**
 * Implements hook_openlayers_maps().
 */
function dpoi_openlayers_maps() {
	// Create full preset array
	$widget = new stdClass;
	$widget->disabled = FALSE; /* Edit this to true to make a default openlayers_maps disabled initially */
	$widget->api_version = 1;
	$widget->name = 'dpoi_widget_map';
	$widget->title = 'DPOI Widget Map';
	$widget->description = 'A Map Used for DPOI Input';
	$widget->data = array(
			'width' => '600px',
			'height' => '400px',
			'image_path' => 'sites/all/modules/openlayers/themes/default_dark/img/',
			'css_path' => 'sites/all/modules/openlayers/themes/default_dark/style.css',
			'proxy_host' => '',
			'hide_empty_map' => array(),
			'center' => array(
					'initial' => array(
							'centerpoint' => '126.97551511841516, 37.575534636514234',
							'zoom' => '14',
					),
					'restrict' => array(
							'restrictextent' => 0,
							'restrictedExtent' => '',
					),
			),
			'behaviors' => array(
					'openlayers_behavior_dpoi' => array(
							'feature_types' => array(
									'point' => 'point',
									'path' => 'path',
									'polygon' => 'polygon',
							),
							'allow_edit' => 1,
					),
					'openlayers_behavior_geofield' => array(
							'feature_types' => array(
									'point' => 'point',
									'path' => 'path',
									'polygon' => 'polygon',
							),
							'allow_edit' => 1,
					),
					'openlayers_behavior_scaleline' => array(),
					'openlayers_behavior_navigation' => array(
							'zoomWheelEnabled' => 0,
							'zoomBoxEnabled' => 1,
							'documentDrag' => 0,
					),
					'openlayers_behavior_panzoombar' => array(
							'zoomWorldIcon' => 0,
							'panIcons' => 1,
					),
			),
			'default_layer' => 'google_normal',
			'layers' => array(
					'google_normal' => 'google_normal',
			),
			'layer_weight' => array(
					'openlayers_geojson_picture_this' => '0',
					'dpoi_formatter' => '0',
			),
			'layer_styles' => array(
					'dpoi_formatter' => '0',
					'openlayers_geojson_picture_this' => '0',
			),
			'layer_styles_select' => array(
					'dpoi_formatter' => '0',
					'openlayers_geojson_picture_this' => '0',
			),
			'layer_activated' => array(
					'dpoi_formatter' => 0,
					'openlayers_geojson_picture_this' => 0,
			),
			'layer_switcher' => array(
					'dpoi_formatter' => 0,
					'openlayers_geojson_picture_this' => 0,
			),
			'projection' => '900913',
			'displayProjection' => '4326',
			'styles' => array(
					'default' => 'default',
					'select' => 'default',
					'temporary' => 'default',
			),
			'map_name' => 'dpoi_widget_map',
	);

	$formatter = new stdClass();
	$formatter->api_version = 1;
	$formatter->name = 'dpoi_formatter_map';
	$formatter->title = t('DPOI Formatter Map');
	$formatter->description = t('A Map Used for DPOI Output');
	$formatter->data = array(
			'width' => '600px',
			'height' => '400px',
			'image_path' => 'sites/all/modules/openlayers/themes/default_dark/img/',
			'css_path' => 'sites/all/modules/openlayers/themes/default_dark/style.css',
			'proxy_host' => '',
			'hide_empty_map' => 0,
			'center' => array(
					'initial' => array(
							'centerpoint' => '126.973007, 37.5671',
							'zoom' => '15',
					),
					'restrict' => array(
							'restrictextent' => 0,
							'restrictedExtent' => '',
					),
			),
			'behaviors' => array(
					'openlayers_behavior_navigation' => array(
							'zoomWheelEnabled' => 0,
							'zoomBoxEnabled' => 1,
							'documentDrag' => 0,
					),
					'openlayers_behavior_panzoombar' => array(
							'zoomWorldIcon' => 0,
							'panIcons' => 1,
					),
					'openlayers_behavior_zoomtolayer' => array(
							'zoomtolayer' => array(
									'google_normal' => '0',
									'dpoi_formatter' => '0',
							),
					),
					'point_zoom_level' => '5',
					'zoomtolayer_scale' => '2',
			),
			'default_layer' => 'google_normal',
			'layers' => array(
					'google_normal' => 'google_normal',
					'dpoi_formatter' => 'dpoi_formatter',
			),
			'layer_weight' => array(
					'dpoi_formatter' => '0',
					'openlayers_geojson_picture_this' => '0',
			),
			'layer_styles' => array(
					'openlayers_geojson_picture_this' => '0',
					'dpoi_formatter' => 'default_marker_red',
			),
			'layer_styles_select' => array(
					'openlayers_geojson_picture_this' => '0',
					'dpoi_formatter' => 'default_marker_gold',
			),
			'layer_activated' => array(
					'openlayers_geojson_picture_this' => 0,
					'dpoi_formatter' => 'dpoi_formatter',
			),
			'layer_switcher' => array(
					'dpoi_formatter' => 0,
					'openlayers_geojson_picture_this' => 0,
			),
			'projection' => '900913',
			'displayProjection' => '4326',
			'styles' => array(
					'default' => 'default',
					'select' => 'default',
					'temporary' => 'default',
			),
			'map_name' => 'dpoi_formatter_map',
	);

	return array(
			'dpoi_widget_map' => $widget,
			'dpoi_formatter_map' => $formatter,
	);
}

/**
 * Implements hook_openlayers_behaviors().
 */
function dpoi_openlayers_behaviors() {
	return array(
			'openlayers_behavior_dpoi' => array(
					'title' => t('DPOI'),
					'description' => t('Fuels the dpoi map-input form.'),
					'type' => 'layer',
					'behavior' => array(
							'path' => drupal_get_path('module', 'dpoi') . '/plugins/behaviors',
							'file' => 'openlayers_behavior_dpoi.inc',
							'class' => 'openlayers_behavior_dpoi',
							'parent' => 'openlayers_behavior',
					),
			),
	);
}

/**
 * Formatter layers
 */
function dpoi_openlayers_layers() {
	$layers = array();
	
	$layer = new stdClass();
	$layer->api_version = 1;
	$layer->name = 'dpoi_formatter';
	$layer->title = 'Placeholder for DPOI Formatter';
	$layer->description = '';
	$layer->data = array(
			'layer_type' => 'openlayers_layer_type_dpoi',
			'projection' => array('900913'),
			'features' => array()
	);
	$layers[$layer->name] = $layer;
	return $layers;
}