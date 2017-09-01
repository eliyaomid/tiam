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
      l('ویرایش', url('admin/merchant/'.$merchant->id)),
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
  if($id) {
    
  }
  $selected = isset($form_state['values']['state']) ? $form_state['values']['state'] : key($all_state_option);
  
  $form['shop_name'] = [
    '#type' => 'textfield',
    '#title' => 'نام فروشگاه',
    '#default_value' => '',
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
    '#default_value' => 0,
  ];

  $form['city'] = [
    '#type' => 'select',
    '#title' => 'شهرستان',
    '#prefix' => '<div id="dropdown-city-replace">',
    '#suffix' => '</div> <br>',
    '#options' => _ajax_city_dropdown_options($selected),
    '#default_value' => 0,
  ];
  
  $form['phone_number'] = [
    '#type' => 'textfield',
    '#title' => 'شماره تماس',
    '#default_value' => '',
    '#maxlength' => 13, 
    '#required' => TRUE,
  ];
  
  $form['address'] = [
    '#type' => 'textarea',
    '#title' => 'آدرس',
    '#default_value' => '',
    '#required' => TRUE,
  ];
  
  $form['map_x'] = [
    '#type' => 'hidden',
    '#value' => 0,
    '#default_value' => '',
  ];
  
  $form['map_y'] = [
    '#type' => 'hidden',
    '#value' => 0,
    '#default_value' => ''
  ];
  
  $form['description'] = [
    '#type' => 'textarea',
    '#title' => 'توضیحات',
    '#default_value' => '',
  ];
  
  $form['merchant_image_fid_1'] = [
    '#type' => 'managed_file',
    '#title' => 'عکس شماره 1',
    '#required' => TRUE,
    '#default_value' => variable_get('merchant_image_fid_1', ''),
    '#upload_location' => 'public://merchant_images/',
  ];
  
  $form['merchant_image_fid_2'] = [
    '#type' => 'managed_file',
    '#title' => 'عکس شماره 2',
    '#default_value' => variable_get('merchant_image_fid_2', ''),
    '#upload_location' => 'public://merchant_images/',
  ];
  
  $form['merchant_image_fid_3'] = [
    '#type' => 'managed_file',
    '#title' => 'عکس شماره 3',
    '#default_value' => variable_get('merchant_image_fid_3', ''),
    '#upload_location' => 'public://merchant_images/',
  ];
  
  $form['merchant_image_fid_4'] = [
    '#type' => 'managed_file',
    '#title' => 'عکس شماره 4',
    '#default_value' => variable_get('merchant_image_fid_4', ''),
    '#upload_location' => 'public://merchant_images/',
  ];
  
  $form['merchant_image_fid_5'] = [
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
  print_r($form_values);exit();
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
            'images_url' => implode([
                    $form_values['image_fid_1'],
                    $form_values['image_fid_2'],
                    $form_values['image_fid_3'],
                    $form_values['image_fid_4'],
                    $form_values['image_fid_5'],
             ]),
          ])
          ->execute();

  $form_state['redirect'] = ['merchant/'];  
}



function merchant_ajax_callback($form, $form_state) {
  return $form['city'];
}



function _ajax_city_dropdown_options($key = 0) {
  
  $city_list = province_city($key);
  unset($city_list[0]);
  return $city_list;
}