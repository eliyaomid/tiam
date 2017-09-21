<?php

function contact_us_page_form($form, &$form_state) {
  global $user;

  // Check if flood control has been activated for sending e-mails.
  $limit = variable_get('contact_threshold_limit', 5);
  $window = variable_get('contact_threshold_window', 3600);
  if (!flood_is_allowed('contact', $limit, $window) && !user_access('administer contact forms')) {
    drupal_set_message(t("You cannot send more than %limit messages in @interval. Try again later.", ['%limit' => $limit, '@interval' => format_interval($window)]), 'error');
    drupal_access_denied();
    drupal_exit();
  }

  // @TODO managment in admin panel
  $categories = [
    1 => 'پیشنهادات',
    2 => 'انتقادات',
    3 => 'شکایات',
    4 => 'درخواست پذیرندگی'
  ];
  // @TODO set default category
  $default_category = 1;


  // If there is more than one category available and no default category has
  // been selected, prepend a default placeholder value.
  if (!$default_category) {
    if (count($categories) > 1) {
      $categories = [0 => t('- Please choose -')] + $categories;
    }
    else {
      $default_category = key($categories);
    }
  }

  if (!$user->uid) {
    $form['#attached']['library'][] = ['system', 'jquery.cookie'];
    $form['#attributes']['class'][] = 'user-info-from-cookie';
  }

  $form['#attributes']['class'][] = 'contact-form';
  $form['#prefix'] = '<div class="col-sm-5">';
  $form['#suffix'] = '</div>';

  $form['name'] = [
    '#type' => 'textfield',
    '#title' => 'نام و نام خانوادگی',
    '#maxlength' => 255,
    '#default_value' => $user->uid ? format_username($user) : '',
    '#required' => TRUE,
  ];
  $form['mail'] = [
    '#type' => 'textfield',
    '#title' => t('Your e-mail address'),
    '#maxlength' => 255,
    '#default_value' => $user->uid ? $user->mail : '',
  ];
  $form['phone_number'] = [
    '#type' => 'textfield',
    '#title' => 'شماره تماس',
    '#maxlength' => 11,
  ];
  $form['cid'] = [
    '#type' => 'select',
    '#title' => t('Category'),
    '#default_value' => $default_category,
    '#options' => $categories,
    '#required' => TRUE,
    '#access' => count($categories) > 1,
  ];
  $form['message'] = [
    '#type' => 'textarea',
    '#title' => t('Message'),
    '#required' => TRUE,
  ];
  $form['actions'] = ['#type' => 'actions'];
  $form['actions']['submit'] = [
    '#type' => 'submit',
    '#value' => t('Send message'),
  ];

  return $form;
}



function contact_us_page_form_submit($form, &$form_state) {
  global $user, $language;

  $categories = [
    1 => 'پیشنهادات',
    2 => 'انتقادات',
    3 => 'شکایات',
    4 => 'درخواست پذیرندگی'
  ];
  
  $values = $form_state['values'];
  $values['sender'] = clone $user;
  $values['sender']->name = $values['name'];
  $values['sender']->mail = $values['mail'] ? $values['mail'] : 'no-email@email.com';
  $values['category'] = $categories[$values['cid']];

  // Save the anonymous user information to a cookie for reuse.
  if (!$user->uid) {
    user_cookie_save(array_intersect_key($values, array_flip(['name'])));
  }

  // Get the to and from e-mail addresses.
  $to = 'eliyaomid@gmail.com';//'info@eghtesademan.com';
  $from = $values['sender']->mail;

  // Send the e-mail to the recipients using the site default language.
  drupal_mail('contact', 'page_mail', $to, language_default(), $values, $from);

  flood_register_event('contact', variable_get('contact_threshold_window', 3600));
  watchdog('mail', '%sender-name (@sender-from) sent an e-mail regarding %category.', array('%sender-name' => $values['name'], '@sender-from' => $from, '%category' => $values['category']));

  // Jump to home page rather than back to contact page to avoid
  // contradictory messages if flood control has been activated.
  drupal_set_message(t('Your message has been sent.'));
}