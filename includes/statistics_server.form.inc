<?php

/**
 * Give a list of clients to collect statistics from.
 */
function statistics_server_form_client_table() {
  $header = array('Name', 'Address', 'Description', 'Type');
  $rows = array();
  $result = db_query('SELECT id, name, url, description, type FROM {statistics_server_clients}');
  foreach ($result as $record) {
    $row = array($record->name, $record->url, $record->description, $record->type);
    $rows['id' . $record->id] = $row;
  }
  $form['client_table'] = array
  (
    '#type' => 'tableselect',
    '#header' => $header,
    '#options' => $rows,
    '#empty' => t('No client in list.'),
  );
  $form['submit'] = array(
    '#type' => 'submit', 
    '#value' => 'Delete',
  );
  return $form;
}

/**
 * Delete item from list of clients to collect statistics from.
 */
function statistics_server_form_client_table_submit($form, &$form_state) {
  $result = db_query('SELECT id FROM {statistics_server_clients}');
  foreach ($result as $record) {
    if ($form_state['values']['client_table']['id' . $record->id]) {
      db_delete('statistics_server_clients')->condition('id', $record->id)->execute();
    }
  }
}

/**
 * Form to add client from which to collect statistics.
 */
function statistics_server_form_add_client() {
  $form['name'] = array(
    '#type' => 'textfield',
    '#title' => t('Name'),
    '#size' => 40,
    '#maxlength' => 256,
    '#required' => TRUE,
  );
  $form['url'] = array(
    '#type' => 'textfield',
    '#title' => t('Address'),
    '#description' => t('The address of the client you want to collect statistics from.'),
    '#size' => 40,
    '#maxlength' => 256,
    '#required' => TRUE,
  );
  $form['description'] = array(
    '#type' => 'textfield',
    '#title' => t('Description'),
    '#size' => 40,
    '#maxlength' => 256,
    '#required' => FALSE,
  );
  $form['type'] = array(
    '#type' => 'textfield',
    '#title' => t('Type'),
    '#size' => 40,
    '#maxlength' => 256,
    '#required' => TRUE,
  );
  $form['submit'] = array(
    '#type' => 'submit', 
    '#value' => 'Add client',
  );
  return $form;
}

/**
 * Process form to add client from which to collect statistics.
 */
function statistics_server_form_add_client_submit($form, &$form_state) {
  $name = $form_state['values']['name'];
  $url = $form_state['values']['url'];
  $description = $form_state['values']['description'];
  $type = $form_state['values']['type'];
  $id = db_insert('statistics_server_clients')
    ->fields(array(
          'name' => $name,
          'url' => $url,
          'description' => $description,
          'type' => $type,
          ))
    ->execute();
}

/**
 * Form to manage clients to collect statistics from.
 */
function statistics_server_list() {
  $page['form1'] = drupal_get_form('statistics_server_form_client_table');
  $page['form2'] = drupal_get_form('statistics_server_form_add_client');
  return $page;
}

