<?php

function merchant_list() {
  
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
          ->fields('tm', ['id', 'shop_name', 'state', 'city', 'phone_number', 'address']);
  $merchant_list_query->leftJoin('tiam_province', 'tps', 'tm.state=tps.id');
  $merchant_list_query->leftJoin('tiam_province', 'tpc', 'tm.city=tpc.id');
  $merchant_list_query->leftJoin('tiam_merchant_group', 'tmg', 'tm.merchant_group=tmg.id');
  $merchant_list_query->leftJoin('tiam_merchant_group', 'tmsg', 'tm.merchant_sub_group=tmsg.id');
  $merchant_list = $merchant_list_query->fields('tps', ['id', 'name'])
          ->fields('tpc', ['id', 'name'])
          ->fields('tmg', ['id', 'name'])
          ->fields('tmsg', ['id', 'name'])
          ->extend('TableSort')
          ->orderByHeader($header)
          ->extend('PagerDefault')
          ->limit(10)
          ->execute();
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
      l('ویرایش', url('admin/merchant/edit/'.$merchant->id, ['absolute' => true])),
    ];
  }
  $table_output = theme('table', [
    'header' => $header, 
    'rows' => $rows, 
  ]);
  $page_output = $table_output.theme('pager');
  return $page_output;
}




function merchant_add_form($form, &$form_state, $id=0) {
  
  $all_state = db_select('tiam_province', 'tp')
          ->fields('tp', ['id', 'name'])
          ->condition('parent', 0)
          ->execute()
          ->fetchAll();
  
  $all_state_option = [
    '0' => 'همه'
  ];
  foreach($all_state as $state) {
    $all_state_option[$state->id] = $state->name;
  }
  
  $all_group = db_select('tiam_merchant_group', 'tmg')
          ->fields('tmg', ['id', 'name'])
          ->condition('parent', 0)
          ->execute()
          ->fetchAll();
  
  $all_group_option = [
    '0' => 'همه'
  ];
  foreach($all_group as $group) {
    $all_group_option[$group->id] = $group->name;
  }
  
  $merchant = [];
  if($id) {
    $merchant = db_select('tiam_merchant', 'tm')
            ->fields('tm')
            ->condition('id', $id)
            ->execute()
            ->fetch();
    $form['id'] = [
      '#type' => 'hidden',
      '#value' => $id,
    ];
  }
  $selected_state = isset($form_state['values']['state']) ? $form_state['values']['state'] : key($all_state_option);
  $selected_city = isset($form_state['values']['city']) ? $form_state['values']['city'] : 0;
  $selected_group = isset($form_state['values']['merchant_group']) ? $form_state['values']['merchant_group'] : key($all_group_option);
  
  $form['shop_name'] = [
    '#type' => 'textfield',
    '#title' => 'نام فروشگاه',
    '#default_value' => isset($merchant->shop_name) ? $merchant->shop_name : '',
    '#maxlength' => 200, 
    '#required' => TRUE,
  ];
  
  $form['merchant_area'] = [
    '#type' => 'fieldset',
    '#title' => 'منطقه جغرافیایی',
  ];
  
  $form['merchant_area']['state'] = [
    '#type' => 'select',
    '#title' => 'استان',
    '#options' => $all_state_option,
    '#ajax' => [
      'event' => 'change',
      'callback' => 'merchant_city_ajax_callback',
      'wrapper' => 'dropdown-city-replace',
    	'method' => 'replace',
    ],
    '#default_value' => isset($merchant->state) ? $merchant->state : 0,
  ];

  $form['merchant_area']['city'] = [
    '#type' => 'select',
    '#title' => 'شهرستان',
    '#prefix' => '<div id="dropdown-city-replace">',
    '#suffix' => '</div>',
    '#options' => _merchant_ajax_city_dropdown_options($selected_state),
    '#ajax' => [
      'event' => 'change',
      'callback' => 'merchant_city_region_ajax_callback',
      'wrapper' => 'dropdown-city-region-replace',
    	'method' => 'replace',
    ],
    '#default_value' => isset($merchant->city) ? $merchant->city : 0,
  ];
  
  $form['merchant_area']['city_region'] = [
  		'#type' => 'select',
  		'#title' => 'ناحیه',
  		'#prefix' => '<div id="dropdown-city-region-replace">',
  		'#suffix' => '</div>',
   		'#options' => _merchant_ajax_city_dropdown_options($selected_city),
  		'#default_value' => isset($merchant->city_region) ? $merchant->city_region : 0,
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
      'callback' => 'merchant_group_ajax_callback',
      'wrapper' => 'dropdown-group-replace',
    	'method' => 'replace',
    ],
    '#default_value' => isset($merchant->group) ? $merchant->group : 0,
  ];

  $form['merchant_grouping']['merchant_sub_group'] = [
    '#type' => 'select',
    '#title' => 'گروه',
    '#prefix' => '<div id="dropdown-group-replace">',
    '#suffix' => '</div>',
    '#options' => _merchant_ajax_group_dropdown_options($selected_group),
    '#default_value' => isset($merchant->sub_group) ? $merchant->sub_group : 0,
  ];
  
  $form['phone_number'] = [
    '#type' => 'textfield',
    '#title' => 'شماره تماس',
    '#default_value' => isset($merchant->phone_number) ? $merchant->phone_number : '',
    '#maxlength' => 13, 
    '#required' => TRUE,
  ];
  
  $form['address'] = [
    '#type' => 'textarea',
    '#title' => 'آدرس',
    '#default_value' => isset($merchant->address) ? $merchant->address : '',
    '#required' => TRUE,
  ];
  
  $form['map_x'] = [
    '#type' => 'hidden',
    '#value' => 0,
    '#default_value' => isset($merchant->map_x) ? $merchant->map_x : 0,
  ];
  
  $form['map_y'] = [
    '#type' => 'hidden',
    '#value' => 0,
    '#default_value' => isset($merchant->map_y) ? $merchant->map_y : 0,
  ];
  
  $form['description'] = [
    '#type' => 'textarea',
    '#title' => 'توضیحات',
    '#default_value' => isset($merchant->description) ? $merchant->description : '',
  ];

  $form['merchant_image_fieldset'] = [
    '#type' => 'fieldset',
    '#title' => 'تصاویر اسلایدر',
  ];
 
  $form['merchant_image_fieldset']['image_fid_1'] = [
    '#type' => 'managed_file',
    '#title' => 'عکس شماره 1',
    '#required' => TRUE,
    '#default_value' => isset($merchant->image_fid_1) ? $merchant->image_fid_1 : '',
    '#upload_location' => 'public://merchant_images/',
  ];
  
  $form['merchant_image_fieldset']['image_fid_2'] = [
    '#type' => 'managed_file',
    '#title' => 'عکس شماره 2',
    '#default_value' => isset($merchant->image_fid_2) ? $merchant->image_fid_2 : '',
    '#upload_location' => 'public://merchant_images/',
  ];
  
  $form['merchant_image_fieldset']['image_fid_3'] = [
    '#type' => 'managed_file',
    '#title' => 'عکس شماره 3',
    '#default_value' => isset($merchant->image_fid_3) ? $merchant->image_fid_3 : '',
    '#upload_location' => 'public://merchant_images/',
  ];
  
  $form['merchant_image_fieldset']['image_fid_4'] = [
    '#type' => 'managed_file',
    '#title' => 'عکس شماره 4',
    '#default_value' => isset($merchant->image_fid_4) ? $merchant->image_fid_4 : '',
    '#upload_location' => 'public://merchant_images/',
  ];
  
  $form['merchant_image_fieldset']['image_fid_5'] = [
    '#type' => 'managed_file',
    '#title' => 'عکس شماره 5',
    '#default_value' => isset($merchant->image_fid_5) ? $merchant->image_fid_5 : '',
    '#upload_location' => 'public://merchant_images/',
  ];
  
  $form['merchant_image_fieldset']['image_fid_6'] = [
    '#type' => 'managed_file',
    '#title' => 'عکس شماره 6',
    '#default_value' => isset($merchant->image_fid_6) ? $merchant->image_fid_6 : '',
    '#upload_location' => 'public://merchant_images/',
  ];

  $form['submit'] = [
    '#type' => 'submit',
    '#value' => 'ذخیره',
  ];
  
  return $form;
}



function merchant_add_form_validate($form, &$form_state) {
  
  for ($index = 1; $index <= 6; $index++) {
    if(isset($form_state['values']["image_fid_$index"]) && $form_state['values']["image_fid_$index"]) {
      $file = file_load($form_state['values']["image_fid_$index"]);
      $file->status = FILE_STATUS_PERMANENT;
      file_save($file);
      file_usage_add($file, 'merchant', 'file', $form_state['values']["image_fid_$index"]);
    }
  }  
}



function merchant_add_form_submit($form, &$form_state) {
  $form_values = $form_state['values'];
  $id = 0;
  if(array_key_exists('id', $form_values)) {
  	$id = $form_values['id'];
  }
  if($id) {
    db_update('tiam_merchant')
    				->fields([
              'shop_name' => $form_values['shop_name'],
              'state' => $form_values['state'],
              'city' => $form_values['city'],
            	'merchant_group' => $form_values['merchant_group'],
            	'merchant_sub_group' => $form_values['merchant_sub_group'],
              'phone_number' => $form_values['phone_number'],
              'address' => $form_values['address'],
              'map_x' => $form_values['map_x'],
              'map_y' => $form_values['map_y'],
              'description' => $form_values['description'] ? $form_values['description'] : '',
              'image_fid_1' => $form_values['image_fid_1'],
              'image_fid_2' => $form_values['image_fid_2'],
              'image_fid_3' => $form_values['image_fid_3'],
              'image_fid_4' => $form_values['image_fid_4'],
              'image_fid_5' => $form_values['image_fid_5'],
              'image_fid_6' => $form_values['image_fid_6'],
            ])
            ->condition('id', $id)
            ->execute();
  }
  else {
    db_insert('tiam_merchant')
            ->fields([
              'shop_name' => $form_values['shop_name'],
              'state' => $form_values['state'],
              'city' => $form_values['city'],
            	'merchant_group' => $form_values['merchant_group'],
            	'merchant_sub_group' => $form_values['merchant_sub_group'],
              'phone_number' => $form_values['phone_number'],
              'address' => $form_values['address'],
              'map_x' => $form_values['map_x'],
              'map_y' => $form_values['map_y'],
              'description' => $form_values['description'] ? $form_values['description'] : '',
              'image_fid_1' => $form_values['image_fid_1'],
              'image_fid_2' => $form_values['image_fid_2'],
              'image_fid_3' => $form_values['image_fid_3'],
              'image_fid_4' => $form_values['image_fid_4'],
              'image_fid_5' => $form_values['image_fid_5'],
              'image_fid_6' => $form_values['image_fid_6'],
            ])
            ->execute();    
  }
  drupal_set_message('مشخصات پذیرنده با موفقیت ذخیره شد.');
  $form_state['redirect'] = ['admin/merchant']; 
}



function merchant_city_ajax_callback($form, $form_state) {
  return $form['merchant_area']['city'];
}



function merchant_city_region_ajax_callback($form, $form_state) {
  return $form['merchant_area']['city_region'];
}



function merchant_group_ajax_callback($form, $form_state) {
  return $form['merchant_grouping']['merchant_sub_group'];
}



function _merchant_ajax_city_dropdown_options($key = 0) {
  
	$city_list = province_city($key);
	return $city_list;
}



function _merchant_ajax_group_dropdown_options($key = 0) {
  
	$group_list = merchant_sub_group($key);
	return $group_list;
}



function merchant_group_add_form($form, &$form_state, $id=0) {
  
  $form = [];
  
  return $form;  
}