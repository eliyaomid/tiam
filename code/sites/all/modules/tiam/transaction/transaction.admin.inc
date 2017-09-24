<?php

function user_transaction_list_form($form, &$form_state) {
  
  $form = [];
  $form['filters'] = [
    '#type' => 'fieldset',
    '#title' => 'جستجو در تراکنش ها',
  ];
//  $form['filters']['filters']['date_from'] = [
//    '#type' => 'textfield',
//    '#title' => 'از',
//    '#default_value' => '',
//  ];
//  $form['filters']['filters']['date_to'] = [
//    '#type' => 'textfield',
//    '#title' => 'تا',
//    '#default_value' => '',
//  ];
//  $form['filters']['actions'] = [
//    '#type' => 'actions',
//    '#attributes' => ['class' => ['container-inline']],
//  ];
  $form['filters']['actions']['submit'] = [
    '#type' => 'submit',
    '#value' => 'جستجو',
  ];
  
  $form['transaction'] = user_transaction_list();

  return $form;
}



function merchant_transaction_list_form($form, &$form_state) {

  $form = [];
  $form['filters'] = [
    '#type' => 'fieldset',
    '#title' => 'جستجو در تراکنش ها',
  ];
//  $form['filters']['filters']['date_from'] = [
//    '#type' => 'textfield',
//    '#title' => 'از',
//    '#default_value' => '',
//  ];
//  $form['filters']['filters']['date_to'] = [
//    '#type' => 'textfield',
//    '#title' => 'تا',
//    '#default_value' => '',
//  ];
//  $form['filters']['actions'] = [
//    '#type' => 'actions',
//    '#attributes' => ['class' => ['container-inline']],
//  ];
  $form['filters']['actions']['submit'] = [
    '#type' => 'submit',
    '#value' => 'جستجو',
  ];
  $form['transaction'] = merchant_transaction_list();

  return $form;
}



function user_transaction_list() {
  
  $header = [
    'id' => ['data' => 'شناسه', 'field' => 'id'],
    'transaction_date' => ['data' => 'تاریخ تراکنش', 'field' => 'transaction_date'],
    'transaction_amount' => ['data' => 'مبلغ تراکنش', 'field' => 'transaction_amount'],
    'merchant_id' => ['data' => 'پذیرنده', 'field' => 'merchant_id'],
    'transaction_discount_percent' => ['data' => 'تخفیف واریز شده به حساب', 'field' => 'transaction_discount_percent'],
  ];
  $rows = [];
  
  $query = db_select('tiam_transaction_user', 'ttu');
  $query->leftJoin('tiam_merchant', 'tm', 'ttu.merchant_id = tm.id');
  $transactions = $query->extend('PagerDefault')
    ->extend('TableSort')
    ->fields('ttu')
    ->fields('tm', ['shop_name'])
    ->limit(10)
    ->orderByHeader($header)
    ->execute()
    ->fetchAll();
  
  foreach ($transactions as $transaction) {
    $rows[] = [
      'id' => $transaction->id,
      'transaction_date' => $transaction->transaction_date,
      'transaction_amount' => $transaction->transaction_amount,
      'merchant_id' => $transaction->shop_name,
      'transaction_discount_percent' => $transaction->transaction_discount_percent,      
    ];
  }
  $form['nodes'] = [
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => 'تراکنشی وجود ندارد.',
  ];
  
  return $form;  
}



function merchant_transaction_list() {
  
  $header = [
    'id' => ['data' => 'شناسه', 'field' => 'id'],
    'transaction_date' => ['data' => 'تاریخ تراکنش', 'field' => 'transaction_date'],
    'transaction_amount' => ['data' => 'مبلغ تراکنش', 'field' => 'transaction_amount'],
    'cart_number' => ['data' => 'شماره کارت', 'field' => 'cart_number'],
    'customer_name' => ['data' => 'نام خریدار', 'field' => 'customer_name'],
  ];
  $rows = [];
  
  $transactions = db_select('tiam_transaction_merchant', 'ttm')
    ->extend('PagerDefault')
    ->extend('TableSort')
    ->fields('ttm')
    ->limit(10)
    ->orderByHeader($header)
    ->execute()
    ->fetchAll();
  
  foreach ($transactions as $transaction) {
    $rows[] = [
      'id' => $transaction->id,
      'transaction_date' => $transaction->transaction_date,
      'transaction_amount' => $transaction->transaction_amount,
      'cart_number' => $transaction->cart_number,
      'customer_name' => $transaction->customer_name,      
    ];
  }
  $form['nodes'] = [
    '#theme' => 'table',
    '#header' => $header,
    '#rows' => $rows,
    '#empty' => 'تراکنشی وجود ندارد.',
  ];
  
  return $form;
}