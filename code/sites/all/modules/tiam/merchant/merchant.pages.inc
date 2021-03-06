<?php

function merchant_list_page() {

  $header = [
    ['data' => 'شناسه', 'field' => 'id'],
    ['data' => 'نام فروشگاه', 'field' => 'shop_name'],
    ['data' => 'نام مدیر', 'field' => 'manager_name'],
    ['data' => 'استان', 'field' => 'state'],
    ['data' => 'شهرستان', 'field' => 'city'],
  	['data' => 'صنف', 'field' => 'merchant_group'],
    ['data' => 'گروه', 'field' => 'merchant_sub_group'],
    ['data' => 'درصد تخفیف', 'field' => 'discount_percent'],
    ['data' => 'شماره تماس'],
    ['data' => 'آدرس'],
    ['data' => 'جزئیات'],
  ];
  $merchant_list_query = db_select('tiam_merchant', 'tm')
          ->fields('tm', ['id', 'shop_name', 'manager_name', 'phone_number', 'discount_percent', 'address']);
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
      ['data' => $merchant->manager_name],
      ['data' => $merchant->name],
      ['data' => $merchant->tpc_name],
    	['data' => $merchant->tmg_name],
      ['data' => $merchant->tmsg_name],
      ['data' => $merchant->discount_percent],
      ['data' => $merchant->phone_number],
      ['data' => $merchant->address],
      l('مشاهده جزئیات', url('merchant/list/'.$merchant->id, ['absolute' => true])),
    ];
  }
  drupal_add_css('#main-content,.page-header{display:none;}', ['type'=>'inline']);
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

  $form_state['redirect'] = ['merchant/list', ['query' => $query]];
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
  $merchant_query = db_select('tiam_merchant', 'tm')
          ->fields('tm')
          ->condition('tm.id', $id);
  $merchant_query->leftJoin('tiam_province', 'tps', 'tm.state=tps.id');
  $merchant_query->leftJoin('tiam_province', 'tpc', 'tm.city=tpc.id');
  $merchant_query->leftJoin('tiam_merchant_group', 'tmg', 'tm.merchant_group=tmg.id');
  $merchant_query->leftJoin('tiam_merchant_group', 'tmsg', 'tm.merchant_sub_group=tmsg.id');
  $merchant = $merchant_query->fields('tps', ['id', 'name'])
          ->fields('tpc', ['id', 'name'])
          ->fields('tmg', ['id', 'name'])
          ->fields('tmsg', ['id', 'name'])
          ->execute()
          ->fetch();
  $images = [];
  for ($index = 1; $index <= 6; $index++) {
    $feild_name  = "image_fid_$index";
    if($merchant->$feild_name) {
      $uri = file_load($merchant->$feild_name)->uri;
      $images[] = [
        'uri' => $uri,
        'thumb' => $uri,    
      ];
    }
  }
  drupal_add_css('#main-content,.page-header{display:none;}', ['type'=>'inline']);
  drupal_set_title($merchant->shop_name);

  return [
    'slider' => [
      '#theme' => 'thumbnail_navigator_with_arrows',
      '#images' => $images,
      '#suffix' => '<br>',
    ],
    'merchant_info' => [
      '#markup' => "
        <div class='panel panel-default'>
          <div class='panel-heading'>$merchant->shop_name</div>
          <div class='panel-body'>
            <ul class='list-group'>
              <li class='list-group-item'><strong>نام مدیر:</strong> $merchant->manager_name</li>
              <li class='list-group-item'><strong>استان:</strong> $merchant->name</li>
              <li class='list-group-item'><strong>شهرستان:</strong> $merchant->tpc_name</li>
              <li class='list-group-item'><strong>صنف:</strong> $merchant->tmg_name</li>
              <li class='list-group-item'><strong>گروه:</strong> $merchant->tmsg_name</li>
              <li class='list-group-item'><strong>ویژگی ها:</strong> $merchant->feature</li>
              <li class='list-group-item'><strong>توضیحات:</strong> $merchant->description</li>
            </ul>
          </div>
        </div>",
      '#prefix' => '<div class="col-sm-8">',
      '#suffix' => '</div>',
    ],
    'merchant_location' => [
      '#markup' => "
        <div class='panel panel-default'>
          <div class='panel-body'>
            <ul class='list-group'>
              <li class='list-group-item'><strong>شماره تماس:</strong> $merchant->phone_number</li>
              <li class='list-group-item'><strong>آدرس:</strong> $merchant->address</li>
            </ul>
            <input id='map_position_x' name='map_x' value='$merchant->map_x' type='hidden'>
            <input id='map_position_y' name='map_y' value='$merchant->map_y' type='hidden'>
            <div id='map_location' style='height: 200px; width: 100%;'></div>
          </div>
        </div>",
      '#prefix' => '<div class="col-sm-4">',
      '#suffix' => '</div>',
      '#attached' => [
        'js' => [
          ['data' => 'http://maps.googleapis.com/maps/api/js?key=AIzaSyBIOCfIlB3NTW2pPx4T7dajJ9HZV57_BCM', 'type' => 'external'],
          ['data' => drupal_get_path('module', 'merchant').'/js/googlemap_page.js', 'type' => 'file'],
        ],
      ],
    ]
  ];
}



function merchant_login_page_form($form, &$form_state) {
  $form = [
    '#prefix' => '<div class="container"><div id="merchant-login-form" class="col-sm-4 clearfix">',
    '#suffix' => '</div></div><br><br>',
  ];
  
  $form['name'] = [
    '#type' => 'textfield',
    '#title' => 'شماره موبایل',
    '#maxlength' => 12, 
    '#required' => TRUE,
  ];
  $form['pass'] = [
    '#type' => 'password',
    '#title' => 'کلمه عبور',
    '#maxlength' => 30, 
    '#required' => TRUE,
  ];
  $form['submit'] = [
    '#type' => 'submit',
    '#value' => 'ورود',
  ];
  $form['#validate'] = user_login_default_validators();
  
  return $form;
}



function merchant_login_page_form_submit($form, &$form_state) {
  global $user;
  $user = user_load($form_state['uid']);
  if(in_array('cart_holder', array_values($user->roles))) {
    $form_state['redirect'] = 'admin/transaction/user';
  }
  elseif(in_array('merchant', array_values($user->roles))) {
    $form_state['redirect'] = 'admin/transaction/merchant';
  }
  else {
    $form_state['redirect'] = 'admin';    
  }
  user_login_finalize($form_state);  
}



function user_register_cart_form($form, &$form_state) {
  $form = [
    '#prefix' => '<div class="container"><div id="register-cart-form" class="col-sm-4 clearfix">',
    '#suffix' => '</div></div><br><br>',
  ];
  
  $form['name'] = [
    '#type' => 'textfield',
    '#title' => 'نام و نام خانوادگی',
    '#maxlength' => 50, 
    '#required' => TRUE,
  ];
  $form['user_name'] = [
    '#type' => 'textfield',
    '#title' => 'شماره موبایل',
    '#maxlength' => 12, 
    '#required' => TRUE,
  ];
  $form['cart_nubmer'] = [
    '#type' => 'textfield',
    '#title' => 'شماره کارت',
    '#maxlength' => 16, 
    '#required' => TRUE,
  ];

  $form['submit'] = [
    '#type' => 'submit',
    '#value' => 'ثبت کارت',
  ];
  
  return $form;
}




function user_register_cart_form_submit($form, &$form_state) {
  
}



function reset_password_form($form, &$form_state) {
  
  drupal_add_css('#main-content,.page-header{display:none;}', ['type'=>'inline']);
  drupal_set_title('درخواست گذرواژه جدید');
  $form = [
    '#prefix' => '<div class="container"><div id="reset-password-form" class="col-sm-4 clearfix">',
    '#suffix' => '</div></div><br><br>',
  ];
  
  $form['user_name'] = [
    '#type' => 'textfield',
    '#title' => 'شماره موبایل',
    '#maxlength' => 12, 
    '#required' => TRUE,
  ];

  $form['submit'] = [
    '#type' => 'submit',
    '#value' => 'گذرواژه جدید را ارسال کن',
  ];
  
  return $form;  
}



function reset_password_form_submit($form, &$form_state) {
  
}