<?php

function merchant_list() {
  
  $header = [
    ['data' => 'شناسه', 'field' => 'id'],
    ['data' => 'نام فروشگاه', 'field' => 'shop_name'],
    ['data' => 'استان', 'field' => 'state'],
    ['data' => 'شهرستان', 'field' => 'city'],
    ['data' => 'شماره تماس'],
    ['data' => 'آدرس'],
    ['data' => 'جزئیات'],
  ];
  $merchant_list = db_select('tiam_merchant', 'tm')
    ->fields('tm', ['id', 'shop_name', 'state', 'city', 'phone_number', 'address'])
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
      ['data' => $merchant->state],
      ['data' => $merchant->city],
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
    ->fetchAllAssoc('id', PDO::FETCH_ASSOC);
  
  $all_state_option = [];
  foreach($all_state as $state) {
    $all_state_option[$state['id']] = $state['name']; 
  }
  $merchant = [];
  if($id) {
    $merchant = db_select('tiam_merchant', 'tm')
      ->fields('tm')
      ->condition('id', $id)
      ->execute()
      ->fetch();
  }
  $selected = isset($form_state['values']['state']) ? $form_state['values']['state'] : key($all_state_option);
  $form['shop_name'] = [
    '#type' => 'textfield',
    '#title' => 'نام فروشگاه',
    '#default_value' => $merchant->shop_name ? $merchant->shop_name : '',
    '#maxlength' => 200, 
    '#required' => TRUE,
  ];
  
  $form['state'] = [
    '#type' => 'select',
    '#title' => 'استان',
    '#options' => $all_state_option,
    '#ajax' => array(
      'event' => 'change',
      'callback' => 'merchant_ajax_callback',
      'wrapper' => 'dropdown-city-replace',
    ),
    '#default_value' => $merchant->state ? $merchant->state : 0,
  ];

  $form['city'] = [
    '#type' => 'select',
    '#title' => 'شهرستان',
    '#prefix' => '<div id="dropdown-city-replace">',
    '#suffix' => '</div> <br>',
    '#options' => _ajax_city_dropdown_options($selected),
    '#default_value' => $merchant->city ? $merchant->city : 0,
  ];
  
  $form['phone_number'] = [
    '#type' => 'textfield',
    '#title' => 'شماره تماس',
    '#default_value' => $merchant->phone_number ? $merchant->phone_number : '',
    '#maxlength' => 13, 
    '#required' => TRUE,
  ];
  
  $form['address'] = [
    '#type' => 'textarea',
    '#title' => 'آدرس',
    '#default_value' => $merchant->address ? $merchant->address : '',
    '#required' => TRUE,
  ];
  
  $form['map_x'] = [
    '#type' => 'hidden',
    '#value' => 0,
    '#default_value' => $merchant->map_x ? $merchant->map_x : 0,
  ];
  
  $form['map_y'] = [
    '#type' => 'hidden',
    '#value' => 0,
    '#default_value' => $merchant->map_x ? $merchant->map_x : 0,
  ];
  
  $form['description'] = [
    '#type' => 'textarea',
    '#title' => 'توضیحات',
    '#default_value' => $merchant->description ? $merchant->description : '',
  ];

   $form['merchant_image_fieldset'] = [
    '#type' => 'fieldset',
    '#title' => 'تصاویر اسلایدر',
  ];
 
  $form['merchant_image_fieldset']['merchant_image_fid_1'] = [
    '#type' => 'managed_file',
    '#title' => 'عکس شماره 1',
    '#required' => TRUE,
    '#default_value' => $merchant->merchant_image_fid_1 ? $merchant->merchant_image_fid_1 : '',
    '#upload_location' => 'public://merchant_images/',
  ];
  
  $form['merchant_image_fieldset']['merchant_image_fid_2'] = [
    '#type' => 'managed_file',
    '#title' => 'عکس شماره 2',
    '#default_value' => variable_get('merchant_image_fid_2', ''),
    '#upload_location' => 'public://merchant_images/',
  ];
  
  $form['merchant_image_fieldset']['merchant_image_fid_3'] = [
    '#type' => 'managed_file',
    '#title' => 'عکس شماره 3',
    '#default_value' => variable_get('merchant_image_fid_3', ''),
    '#upload_location' => 'public://merchant_images/',
  ];
  
  $form['merchant_image_fieldset']['merchant_image_fid_4'] = [
    '#type' => 'managed_file',
    '#title' => 'عکس شماره 4',
    '#default_value' => variable_get('merchant_image_fid_4', ''),
    '#upload_location' => 'public://merchant_images/',
  ];
  
  $form['merchant_image_fieldset']['merchant_image_fid_5'] = [
    '#type' => 'managed_file',
    '#title' => 'عکس شماره 5',
    '#default_value' => variable_get('merchant_image_fid_5', ''),
    '#upload_location' => 'public://merchant_images/',
  ];
  
  $form['submit'] = [
    '#type' => 'submit',
    '#value' => 'ذخیره',
  ];
  
  return $form;
}



function merchant_add_form_submit($form, &$form_state) {
  $form_values = $form_state['values'];
  db_insert('tiam_merchant')
          ->fields([
            'shop_name' => $form_values['shop_name'],
            'state' => $form_values['state'],
            'city' => $form_values['city'],
            'phone_number' => $form_values['phone_number'],
            'address' => $form_values['address'],
            'map_x' => $form_values['map_x'],
            'map_y' => $form_values['map_y'],
            'description' => $form_values['description'] ? $form_values['description'] : '',
	    'image_fid_1' => $form_values['merchant_image_fid_1'],
	    'image_fid_2' => $form_values['merchant_image_fid_2'],
	    'image_fid_3' => $form_values['merchant_image_fid_3'],
	    'image_fid_4' => $form_values['merchant_image_fid_4'],
 	    'image_fid_5' => $form_values['merchant_image_fid_5'],
          ])
          ->execute();

  $form_state['redirect'] = ['admin/merchant']; 
}



function merchant_ajax_callback($form, $form_state) {
  return $form['city'];
}



function _ajax_city_dropdown_options($key = 0) {
  
  $city_list = province_city($key);
  unset($city_list[0]);
  return $city_list;
}
