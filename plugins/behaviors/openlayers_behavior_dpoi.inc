<?php
/**
 * @file
 * Implementation of OpenLayers behavior.
 */

/**
 * Ctools plugin definition.
 */
$plugin = array(
		'title' => t('DPOI'),
		'description' => t('Provides functionality for adding features to a map.'),
		'type' => 'layer',
		'behavior' => array(
				'file' => 'openlayers_behavior_dpoi.inc',
				'class' => 'openlayers_behavior_dpoi',
				'parent' => 'openlayers_behavior',
		),
);

/**
 * Draw Features behavior
*/
class openlayers_behavior_dpoi extends openlayers_behavior {
	/**
	 * Provide initial values for options.
	 */
	function options_init() {
		return array(
				'feature_types' => array(),
				'allow_edit' => 1,
		);
	}

	function options_form($defaults = array()) {
		$features = array(
				'point' => t('Point'),
				'path' => t('Path'),
				'polygon' => t('Polygon'),
		);

		return array(
				'feature_types' => array(
						'#title' => t('Available Features'),
						'#type' => 'checkboxes',
						'#options' => $features,
						'#description' => t('Select what features are available to draw.'),
						'#default_value' => isset($defaults['feature_types']) ? $defaults['feature_types'] : array(),
				),
				'allow_edit' => array(
						'#title' => t('Allow shape modification'),
						'#type' => 'checkbox',
						'#description' => t('Can you edit and delete shapes.'),
						'#default_value' => isset($defaults['allow_edit']) ? $defaults['allow_edit'] : 1,
				)
		);
	}

	/**
	 * Render.
	 */
	function render(&$map) {
		drupal_add_js(drupal_get_path('module', 'dpoi') . '/plugins/behaviors/openlayers_behavior_dpoi.js');
		drupal_add_js(drupal_get_path('module', 'dpoi') . '/plugins/behaviors/fgh.js');
		drupal_add_js(drupal_get_path('module', 'dpoi') . '/plugins/behaviors/search.js');
		drupal_add_js(drupal_get_path('module', 'dpoi') . '/plugins/behaviors/codeAddress.js');
		drupal_add_js(drupal_get_path('module', 'dpoi') . '/plugins/behaviors/geohash.js');
		
		drupal_add_js(drupal_get_path('module', 'dpoi') . '/libs/jquery/jquery.pajinate.js');
		
		return $this->options;
	}
}