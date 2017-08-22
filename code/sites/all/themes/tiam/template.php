<?php

/**
 * Page alter.
 */
function tiam_page_alter($page) {
	$mobileoptimized = array(
		'#type' => 'html_tag',
		'#tag' => 'meta',
		'#attributes' => array(
		'name' =>  'MobileOptimized',
		'content' =>  'width'
		)
	);
	$handheldfriendly = array(
		'#type' => 'html_tag',
		'#tag' => 'meta',
		'#attributes' => array(
		'name' =>  'HandheldFriendly',
		'content' =>  'true'
		)
	);
	$viewport = array(
		'#type' => 'html_tag',
		'#tag' => 'meta',
		'#attributes' => array(
		'name' =>  'viewport',
		'content' =>  'width=device-width, initial-scale=1'
		)
	);
	drupal_add_html_head($mobileoptimized, 'MobileOptimized');
	drupal_add_html_head($handheldfriendly, 'HandheldFriendly');
	drupal_add_html_head($viewport, 'viewport');
}


/**
 * Preprocess variables for html.tpl.php
 */
function tiam_preprocess_html(&$variables) {
}



/**
 * Preprocess variables for page template.
 */
function tiam_preprocess_page(&$variables) {

	/**
	 * insert variables into page template.
	 */
	if($variables['page']['sidebar_first'] && $variables['page']['sidebar_second']) { 
		$variables['sidebar_grid_class'] = 'col-md-3';
		$variables['main_grid_class'] = 'col-md-6';
	} elseif ($variables['page']['sidebar_first'] || $variables['page']['sidebar_second']) {
		$variables['sidebar_grid_class'] = 'col-md-4';
		$variables['main_grid_class'] = 'col-md-8';		
	} else {
		$variables['main_grid_class'] = 'col-md-12';			
	}

	if($variables['page']['header_top_left'] && $variables['page']['header_top_right']) { 
		$variables['header_top_left_grid_class'] = 'col-md-8';
		$variables['header_top_right_grid_class'] = 'col-md-4';
	} elseif ($variables['page']['header_top_right'] || $variables['page']['header_top_left']) {
		$variables['header_top_left_grid_class'] = 'col-md-12';
		$variables['header_top_right_grid_class'] = 'col-md-12';		
	}	
}



/**
 * Preprocess variables for block.tpl.php
 */
function tiam_preprocess_block(&$variables) {
	$variables['classes_array'][]='clearfix';
}



/**
 * Override theme_breadrumb().
 *
 * Print breadcrumbs as a list, with separators.
 */
function tiam_breadcrumb($variables) {
	$breadcrumb = $variables['breadcrumb'];

	if (!empty($breadcrumb)) {
		$breadcrumb[] = drupal_get_title();
		$breadcrumbs = '<ol class="breadcrumb">';

		$count = count($breadcrumb) - 1;
		foreach ($breadcrumb as $key => $value) {
		$breadcrumbs .= '<li>' . $value . '</li>';
		}
		$breadcrumbs .= '</ol>';

		return $breadcrumbs;
	}
}



/**
 * Search block form alter.
 */
function tiam_form_alter(&$form, &$form_state, $form_id) {
	if ($form_id == 'search_block_form') {
	    unset($form['search_block_form']['#title']);
	    $form['search_block_form']['#title_display'] = 'invisible';
		$form_default = t('Search this website...');
	    $form['search_block_form']['#default_value'] = $form_default;

		$form['actions']['submit']['#attributes']['value'][] = '';

	 	$form['search_block_form']['#attributes'] = array('onblur' => "if (this.value == '') {this.value = '{$form_default}';}", 'onfocus' => "if (this.value == '{$form_default}') {this.value = '';}" );
	}
}