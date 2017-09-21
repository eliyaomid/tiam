<?php

function contact_us_info_admin_form($form, &$form_state) {
  
  $form = [];
  $form['contact_us_email'] = [
    '#type' => 'textfield',
    '#title' => t('e-mail'),
    '#maxlength' => 255,
    '#default_value' => variable_get('contact_us_email', 'eliyaomid@gmail.com'),
    '#required' => TRUE,
  ];
  $form['contact_us_phone'] = [
    '#type' => 'textfield',
    '#title' => 'شماره تماس',
    '#maxlength' => 11,
    '#default_value' => variable_get('contact_us_phone', ' 09134311348'),
    '#required' => TRUE,
  ];
  $form['contact_us_fax'] = [
    '#type' => 'textfield',
    '#title' => 'شماره فکس',
    '#maxlength' => 11,
    '#default_value' => variable_get('contact_us_fax', ' 09162392091'),
    '#required' => TRUE,
  ];
  $form['contact_us_address'] = [
    '#type' => 'textarea',
    '#title' => 'آدرس',
    '#default_value' => variable_get('contact_us_address', 'isfahan, iran'),
    '#required' => TRUE,
  ];
  $form['actions'] = ['#type' => 'actions'];
  $form['actions']['submit'] = [
    '#type' => 'submit',
    '#value' => 'ذخیره',
  ];
  
  return system_settings_form($form);  
}