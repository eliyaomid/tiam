<?php

function merchant_list_page() {

  $header = [
    ['data' => 'شناسه', 'field' => 'id'],
    ['data' => 'نام فروشگاه', 'field' => 'shop_name'],
    ['data' => 'استان', 'field' => 'state'],
    ['data' => 'شهرستان', 'field' => 'city'],
    ['data' => 'شماره تماس'],
    ['data' => 'آدرس'],
    ['data' => 'جزئیات'],
  ];
  $merchant_list_query = db_select('tiam_merchant', 'tm')
          ->fields('tm', ['id', 'shop_name', 'phone_number', 'address']);
  $merchant_list_query->innerJoin('tiam_province', 'tps', 'tm.state=tps.id');
  $merchant_list_query->innerJoin('tiam_province', 'tpc', 'tm.city=tpc.id');
  $merchant_list = $merchant_list_query->fields('tps', ['id', 'name'])
          ->fields('tpc', ['id', 'name'])
          ->extend('TableSort')
          ->orderByHeader($header);
  if(isset($_GET['state'])) {
    $merchant_list->condition('tps.id', $_GET['state']);
  }
  if(isset($_GET['city'])) {
    $merchant_list->condition('tpc.id', $_GET['city']);
  }
  $merchant_list = $merchant_list->execute();
  $rows = [];
  foreach($merchant_list as $merchant) {
    $rows[] = [
      ['data' => $merchant->id],
      ['data' => $merchant->shop_name],
      ['data' => $merchant->name ? $merchant->name:''],
      ['data' => $merchant->tpc_name ? $merchant->tpc_name:''],
      ['data' => $merchant->phone_number],
      ['data' => $merchant->address],
      l('مشاهده جزئیات', url('merchant/'.$merchant->id)),
    ];
  }
  $page_output['merchant_list'] = [
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#attributes' => ['class' => ['table-bordered']]
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
  $selected = isset($form_state['values']['state']) ? $form_state['values']['state'] : 0;
  if(!$selected) {
    $selected = isset($_GET['state']) ? $_GET['state'] : 0;
  }
  
  $form['state'] = [
    '#type' => 'select',
    '#title' => 'استان',
    '#options' => $all_state_option,
    '#ajax' => array(
      'event' => 'change',
      'callback' => 'merchant_ajax_callback',
      'wrapper' => 'dropdown-city-replace',
    ),
    '#default_value' => isset($_GET['state']) ? $_GET['state'] : 0,
  ];

  $form['city'] = [
    '#type' => 'select',
    '#title' => 'شهرستان',
    '#prefix' => '<div id="dropdown-city-replace">',
    '#suffix' => '</div> <br>',
    '#options' => _ajax_city_dropdown_options($selected),
    '#default_value' => isset($_GET['city']) ? $_GET['city'] : 0,
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

  $form_state['redirect'] = ['merchant', ['query' => $query]];
}



function merchant_ajax_callback($form, $form_state) {
  return $form['city'];
}



function _ajax_city_dropdown_options($key = 0) {

  return province_city($key);
}