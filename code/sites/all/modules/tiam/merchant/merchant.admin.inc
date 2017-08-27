<?php

function merchant_page() {
  
  $header = [
       ['data' => t('id'), 'field' => 'id'],
       ['data' => t('name'), 'field' => 'name'],
       ['data' => t('state'), 'field' => 'state'],
       ['data' => t('city'), 'field' => 'city'],
  ];
  $merchant_list = db_select('tiam_merchant', 'tm')
    ->fields('tm', ['id', 'name', 'state', 'city'])
    ->extend('TableSort')
    ->orderByHeader($header)
    ->extend('PagerDefault')
    ->limit(10)
    ->execute();
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
  $page_output = $table_output.theme('pager');
  return $page_output;
}
