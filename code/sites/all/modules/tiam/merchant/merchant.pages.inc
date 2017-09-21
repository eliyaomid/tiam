<?php

function merchant_list_page() {

  $header = [
    ['data' => 'شناسه', 'field' => 'id'],
    ['data' => 'نام فروشگاه', 'field' => 'shop_name'],
    ['data' => 'استان', 'field' => 'state'],
    ['data' => 'شهرستان', 'field' => 'city'],
  	['data' => 'صنف', 'field' => 'merchant_group'],
    ['data' => 'گروه', 'field' => 'merchant_sub_group'],
    ['data' => 'شماره تماس'],
    ['data' => 'آدرس'],
    ['data' => 'جزئیات'],
  ];
  $merchant_list_query = db_select('tiam_merchant', 'tm')
          ->fields('tm', ['id', 'shop_name', 'phone_number', 'address']);
  $merchant_list_query->leftJoin('tiam_province', 'tps', 'tm.state=tps.id');
  $merchant_list_query->leftJoin('tiam_province', 'tpc', 'tm.city=tpc.id');
  $merchant_list_query->leftJoin('tiam_province', 'tpcr', 'tm.city_region=tpcr.id');
  $merchant_list_query->leftJoin('tiam_merchant_group', 'tmg', 'tm.merchant_group=tmg.id');
  $merchant_list_query->leftJoin('tiam_merchant_group', 'tmsg', 'tm.merchant_sub_group=tmsg.id');
  $merchant_list = $merchant_list_query->fields('tps', ['id', 'name'])
          ->fields('tpc', ['id', 'name'])
          ->fields('tpcr', ['id', 'name'])
          ->fields('tmg', ['id', 'name'])
          ->fields('tmsg', ['id', 'name'])
          ->extend('TableSort')
          ->orderByHeader($header)
          ->extend('PagerDefault')
          ->limit(10);
  if(isset($_GET['state'])) {
    $merchant_list->condition('tps.id', $_GET['state']);
  }
  if(isset($_GET['city'])) {
    $merchant_list->condition('tpc.id', $_GET['city']);
  }
  if(isset($_GET['city_region'])) {
    $merchant_list->condition('tpcr.id', $_GET['city_region']);
  }
  if(isset($_GET['merchant_group'])) {
  	$merchant_list->condition('tmg.id', $_GET['merchant_group']);
  }
  if(isset($_GET['merchant_sub_group'])) {
  	$merchant_list->condition('tmsg.id', $_GET['merchant_sub_group']);
  }
  $merchant_list = $merchant_list->execute();
  $rows = [];
  foreach($merchant_list as $merchant) {
    $rows[] = [
      ['data' => $merchant->id],
      ['data' => $merchant->shop_name],
      ['data' => $merchant->name],
      ['data' => $merchant->tpc_name],
    	['data' => $merchant->tmg_name],
      ['data' => $merchant->tmsg_name],
      ['data' => $merchant->phone_number],
      ['data' => $merchant->address],
      l('مشاهده جزئیات', url('merchant/'.$merchant->id, ['absolute' => true])),
    ];
  }
  $page_output['merchant_list'] = [
    'table' => [
      '#theme' => 'table',
      '#header' => $header,
      '#rows' => $rows,
      '#attributes' => ['class' => ['table-bordered']]
    ],
    'pager' => [
      '#theme' => 'pager'
    ]
  ];

  return $page_output;
}



function merchant_list_filter_form($form, &$form_state) {

  $all_state = db_select('tiam_province', 'tp')
    ->fields('tp', ['id', 'name'])
    ->condition('parent', 0)
    ->execute()
    ->fetchAllAssoc('id', PDO::FETCH_ASSOC);
  
  $all_state_option = [
    '0' => 'همه'
  ];
  foreach($all_state as $state) {
    $all_state_option[$state['id']] = $state['name']; 
  }
  $selected_state = isset($form_state['values']['state']) ? $form_state['values']['state'] : 0;
  if(!$selected_state) {
    $selected_state = isset($_GET['state']) ? $_GET['state'] : 0;
  }
  $selected_city = isset($form_state['values']['city']) ? $form_state['values']['city'] : 0;
  if(!$selected_city) {
    $selected_city = isset($_GET['city']) ? $_GET['city'] : 0;
  }
  
  $all_group = db_select('tiam_merchant_group', 'tmg')
	  ->fields('tmg', ['id', 'name'])
	  ->condition('parent', 0)
	  ->execute()
	  ->fetchAllAssoc('id', PDO::FETCH_ASSOC);
  
  $all_group_option = [
  		'0' => 'همه'
  ];
  foreach($all_group as $group) {
  	$all_group_option[$group['id']] = $group['name'];
  }
  $selected_group = isset($form_state['values']['merchant_group']) ? $form_state['values']['merchant_group'] : 0;
  if(!$selected_group) {
  	$selected_group = isset($_GET['merchant_group']) ? $_GET['merchant_group'] : 0;
  }
  
  $form['merchant_area'] = [
    '#type' => 'fieldset',
    '#title' => 'منطقه جغرافیایی',
  ];
  
  $form['merchant_area']['state'] = [
    '#type' => 'select',
    '#title' => 'استان',
    '#options' => $all_state_option,
    '#ajax' => array(
      'event' => 'change',
      'callback' => 'merchant_ajax_city_callback',
      'wrapper' => 'dropdown-city-replace',
    ),
    '#default_value' => isset($_GET['state']) ? $_GET['state'] : 0,
  ];

  $form['merchant_area']['city'] = [
    '#type' => 'select',
    '#title' => 'شهرستان',
    '#prefix' => '<div id="dropdown-city-replace">',
    '#suffix' => '</div>',
    '#options' => _merchant_ajax_city_dropdown_options($selected_state),
    '#ajax' => [
      'event' => 'change',
      'callback' => 'merchant_ajax_city_region_callback',
      'wrapper' => 'dropdown-city-region-replace',
    	'method' => 'replace',
    ],
    '#default_value' => isset($_GET['city']) ? $_GET['city'] : 0,
  ];
  
  $form['merchant_area']['city_region'] = [
  		'#type' => 'select',
  		'#title' => 'ناحیه',
  		'#prefix' => '<div id="dropdown-city-region-replace">',
  		'#suffix' => '</div>',
   		'#options' => _merchant_ajax_city_dropdown_options($selected_city),
  		'#default_value' => isset($_GET['city_region']) ? $_GET['city_region'] : 0,
  ];
  
  $form['merchant_grouping'] = [
    '#type' => 'fieldset',
    '#title' => 'گروه بندی صنفی',
  ];
  
  $form['merchant_grouping']['merchant_group'] = [
  		'#type' => 'select',
  		'#title' => 'صنف',
  		'#options' => $all_group_option,
  		'#ajax' => [
  				'event' => 'change',
  				'callback' => 'merchant_ajax_group_callback',
  				'wrapper' => 'dropdown-group-replace',
  				'method' => 'replace',
  		],
  		'#default_value' => isset($_GET['merchant_group']) ? $_GET['merchant_group'] : 0,
  ];
  
  $form['merchant_grouping']['merchant_sub_group'] = [
  		'#type' => 'select',
  		'#title' => 'گروه',
  		'#prefix' => '<div id="dropdown-group-replace">',
  		'#suffix' => '</div>',
  		'#options' => _merchant_ajax_group_dropdown_options($selected_group),
  		'#default_value' => isset($_GET['merchant_sub_group']) ? $_GET['merchant_sub_group'] : 0,
  ];

  $form['submit'] = [
    '#type' => 'submit',
    '#value' => 'جستجو',
  ];

  return $form;
}



function merchant_list_filter_form_submit($form, &$form_state) {

  $form_values = $form_state['values'];
  $query = [];
  if($form_values['state']) $query['state'] = $form_values['state'];
  if($form_values['city']) $query['city'] = $form_values['city'];
  if($form_values['city_region']) $query['city_region'] = $form_values['city_region'];
  
  if($form_values['merchant_group']) $query['merchant_group'] = $form_values['merchant_group'];
  if($form_values['merchant_sub_group']) $query['merchant_sub_group'] = $form_values['merchant_sub_group'];

  $form_state['redirect'] = ['merchant', ['query' => $query]];
}



function merchant_ajax_city_callback($form, $form_state) {
  return $form['merchant_area']['city'];
}



function merchant_ajax_city_region_callback($form, $form_state) {
  return $form['merchant_area']['city_region'];
}



function _merchant_ajax_city_dropdown_options($key = 0) {
  return province_city($key);
}


function merchant_ajax_group_callback($form, $form_state) {
	return $form['merchant_grouping']['merchant_sub_group'];
}



function _merchant_ajax_group_dropdown_options($key = 0) {
	return merchant_sub_group($key);
}



function merchant_info_page($id) {

  drupal_add_css('.navbar{margin-bottom: 5px !important;} .main-container{padding:0 !important;}', ['type' => 'inline']);
  $merchant = db_select('tiam_merchant', 'tm')
          ->fields('tm')
          ->condition('id', $id)
          ->execute()
          ->fetch();
  $images = [];
  for ($index = 1; $index <= 6; $index++) {
    $feild_name  = "image_fid_$index";
    $uri = file_load($merchant->$feild_name)->uri;
    $images[] = [
      'uri' => $uri,
      'thumb' => $uri,    
    ];
  }
  
  drupal_set_title($merchant->shop_name);
  return [
    'slider' => [
      '#theme' => 'thumbnail_navigator_with_arrows',
      '#images' => $images,
      '#suffix' => '<br>',
    ],
    'merchant_info' => [
      '#markup' => '
        <div class="panel panel-default">
          <div class="panel-body">
            Basic panel example
          </div>
        </div>',
      '#prefix' => '<div class="col-sm-8">',
      '#suffix' => '</div>',
    ],
    'merchant_location' => [
      '#markup' => '
        <div class="panel panel-default">
          <div class="panel-body">
            Basic panel example
          </div>
        </div>',
      '#prefix' => '<div class="col-sm-4">',
      '#suffix' => '</div>',      
    ]
  ];
}