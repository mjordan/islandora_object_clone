<?php

/**
 * @file
 * Handles the uploading of the XML object file as part of the ingest process.
 */

/**
 * Defines a file upload form for uploading the XML file.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 *
 * @return array
 *   The drupal form definition.
 */
function islandora_object_clone_confirm_form(array $form, array &$form_state) {
  $object = menu_get_object('islandora_object', 2);
  $form['islandora_object_clone_confirm_message'] = array(
    '#markup' => t("You are about to clone this object."),
  );

  $form['islandora_object_clone_confirm_title'] = array(
    '#title' => t('Title'),
    '#type' => 'textfield',
    '#default_value' => 'Clone: ' . $object->label,
    '#description' => t("Title to assign to the object."),
    '#required' => TRUE,
  );

  $namespace = preg_replace('/:.*$/', '', $object->id);
  $form['islandora_object_clone_confirm_namespace'] = array(
    '#title' => t('Namespace'),
    '#type' => 'textfield',
    '#size' => 30,
    '#default_value' => $namespace,
    '#description' => t("Namespace to assign to the object. Do not include the trailing colon."),
    '#required' => TRUE,
  );
  $form['islandora_object_clone_source_pid'] = array(
    '#type' => 'value',
    '#value' => $object->id,
  );
  $form['islandora_object_clone_confirm_clone'] = array(
    '#type' => 'submit',
    '#value' => 'Clone object',
  );

  return $form;
}

/**
 * Addes the uploaded file into the ingestable objects 'OBJ' datastream.
 *
 * @param array $form
 *   The drupal form.
 * @param array $form_state
 *   The drupal form state.
 */
function islandora_object_clone_confirm_form_submit(array $form, array &$form_state) {
  module_load_include('inc', 'islandora_object_clone', 'includes/utilities');
  $source_pid = $form['islandora_object_clone_source_pid']['#value'];
  if ($clone_pid = islandora_object_clone_clone_object($source_pid)) {
    // @todo: Redirect to new object.
    dsm("Source object's PID is " . $clone_pid);
  }
}
