<?php
/**
 * @file
 * DPOI Layer Type
 */

/**
 * Define the Ctools plugin options.
 */
$plugin = array(
		'title' => t('DPOI'),
		'description' => t('layer type for DPOI'),
		'layer_type' => array(
				'file' => 'openlayers_layer_type_dpoi.inc',
				'class' => 'openlayers_layer_type_dpoi',
				'parent' => 'openlayers_layer_type',
		),
);

/**
 * OpenLayers Google Layer Type class
*/
class openlayers_layer_type_dpoi extends openlayers_layer_type {
	/**
	 * Provide initial values for options.
	 */
	function options_init() {
		return array(
				'layer_type' => 'openlayers_layer_type_dpoi',
				'layer_handler' => 'openlayers_dpoi',
				'vector' => TRUE,
				'baselayer' => FALSE,
// 				'baselayer' => TRUE,
				'animationEnabled' => TRUE,
		);
	}

	/**
	 * Options form which generates layers
	 */
	function options_form($defaults = array()) {
		$dpoi_layer_types = array(
				'normal' => 'Normal',
		);
		return array(
				'type' => array(
						'#title' => t('DPOI Layer Type'),
						'#type' => 'select',
						'#default_value' => isset($this->data['type']) ? $this->data['type'] : 'normal',
						'#options' => $dpoi_layer_types
				),
				'layer_type' => array(
						'#type' => 'hidden',
						'#value' => 'openlayers_layer_type_dpoi'
				),
				'numZoomLevels' => array(
						'#type' => 'textfield',
						'#title' => t('Number of Zoom Levels'),
						'#description' => t('Satellite and hybrid maps are occasionally
								unavailable at higher zoom levels.'),
						'#default_value' => isset($this->data['numZoomLevels']) ?
						$this->data['numZoomLevels'] : '23'
				),
				'animationEnabled' => array(
						'#type' => 'checkbox',
						'#title' => t('Animation Enabled'),
						'#description' => t('This enables the Google Maps API zooming animation.
								If you are having issues with your Google layer, it may be helpful
								to turn this off.'),
						'#default_value' => isset($this->data['animationEnabled']) ?
						$this->data['animationEnabled'] : TRUE
				),
		);
	}

	/**
	 * Render.
	 */
	function render(&$map) {
		drupal_add_js(drupal_get_path('module', 'dpoi') . '/plugins/layer_types/openlayers_layer_type_dpoi.js');
		
		return $this->options;
	}
}
