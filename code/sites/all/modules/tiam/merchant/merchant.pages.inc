<?php

function merchant_list_page() {

  $header = [
       ['data' => t('id'), 'field' => 'id'],
       ['data' => t('name'), 'field' => 'name'],
       ['data' => t('state'), 'field' => 'state'],
       ['data' => t('city'), 'field' => 'city'],
  ];
  $merchant_list = db_select('tiam_merchant', 'tm')
    ->fields('tm', ['id', 'name', 'state', 'city'])
    ->extend('TableSort')
    ->orderByHeader($header);
  if(isset($_GET['state'])) {
    $merchant_list->condition('state', $_GET['state']);
  }
  $merchant_list = $merchant_list->execute();
  $rows = [];
  foreach($merchant_list as $merchant) {
    $rows[] = [
      ['data' => $merchant->id],
      ['data' => $merchant->name],
      ['data' => $merchant->state],
      ['data' => $merchant->city],
    ];
  }
  $table_output = theme('table', [
    'header' => $header,
    'rows' => $rows,
    'attributes' => ['class' => ['mytable']]
  ]);
  $page_output = '';
  $page_output .= $table_output;
  $filter_form_output = '';//drupal_get_form('merchant_list_filter_form');
  $page_output .= $filter_form_output;
 

  return $page_output;
}



function merchant_list_filter_form($form, &$form_state) {

  $all_state = db_select('tiam_province', 'tp')
    ->fields('tp', ['id', 'name'])
    ->condition('parent', 0)
    ->execute()
    ->fetchAllAssoc('id', PDO::FETCH_ASSOC);
  
  $all_state_option = [
    '' => t('All')
  ];
  foreach($all_state as $state) {
    $all_state_option[$state['name']] = $state['name']; 
  }

  $form['state'] = [
    '#type' => 'select',
    '#title' => t('State'),
    '#options' => $all_state_option,
    '#default_value' => isset($_GET['state']) ? $_GET['state'] : '',
  ];

  $form['city'] = [
    '#type' => 'select',
    '#title' => t('City'),
    '#options' => ['' => t('All')],
    '#default_value' => isset($_GET['city']) ? $_GET['city'] : '',
  ];

  $form['submit'] = [
    '#type' => 'submit',
    '#value' => t('Search'),
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
