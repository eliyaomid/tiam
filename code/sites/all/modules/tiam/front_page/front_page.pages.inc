<?php

function faq_page_callback() {
  $items = [];
  $all_node = taxonomy_select_nodes(3, false);
  foreach ($all_node as $node) {
    $node = node_load($node);
    $items[] = [
      'title' => $node->title,
      'body' => $node->body['und'][0]['safe_value']
    ];
  }
  return theme('front_page_faq', ['items' => $items]);
}



function download_mobile_application() {
  $content = '<div class="col-sm-3">';
  $content .= l('اندروید', url(''), ['attributes'=>['class' => ['footer-app-link', 'app-android']]]);
  $content .= "<br>";
  $content .= l('آیفون', url(''), ['attributes'=>['class' => ['footer-app-link', 'app-android']]]);
  $content .= "<br></div>";
  
  return [
    '#markup' => $content,
    '#attached' => [
      'css' => [
        drupal_get_path('module', 'front_page') . '/css/download_app.css',
        'type' => 'file',
      ]
    ]
  ];
}