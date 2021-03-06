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
  module_load_include('inc', 'islandora_object_clone', 'includes/utilities');
  $object = menu_get_object('islandora_object', 2);

  $form['islandora_object_clone_confirm_message'] = array(
    '#markup' => t("You are about to clone this object. Children (pages, compound children, collection memebers, etc.) are not cloned."),
  );

  $form['islandora_object_clone_confirm_label'] = array(
    '#title' => t('Label of new object'),
    '#type' => 'textfield',
    '#default_value' => 'Clone of: ' . $object->label,
    '#description' => t("Label to assign to the object."),
    '#required' => TRUE,
  );

  $namespace = preg_replace('/:.*$/', '', $object->id);
  $form['islandora_object_clone_confirm_namespace'] = array(
    '#title' => t('Namespace of new object'),
    '#type' => 'textfield',
    '#size' => 30,
    '#default_value' => $namespace,
    '#description' => t("Namespace to assign to the object. Do not include the trailing colon."),
    '#required' => TRUE,
  );
  $form['islandora_object_clone_confirm_state'] = array(
    '#title' => t('State of new object'),
    '#type' => 'select',
    '#required' => TRUE,
    '#options' => array(
      'A' => 'Active',
      'I' => 'Inactive',
      'D' => 'Deleted',
    ),
    '#default_value' => $object->state,
  );
  $collections = islandora_object_clone_get_collections();
  $default_collections = islandora_object_clone_get_default_collections($object);
  $form['islandora_object_clone_confirm_collections'] = array(
    '#title' => t('Collections to put the new object in'),
    '#type' => 'select',
    '#required' => TRUE,
    '#multiple' => TRUE,
    '#options' => $collections,
    '#default_value' => $default_collections,
  );

  $form['islandora_object_clone_copy_dc'] = array(
    '#type' => 'checkbox',
    '#title' => t("Copy the source object's DC datastream"),
    '#description' => t("If checked, the source object's DC datastream will be copied to the new object."),
    '#default_value' => FALSE,
  );

  $form['islandora_object_clone_redirect_to_metadata'] = array(
    '#type' => 'checkbox',
    '#title' => t("Take me directly to the MODS edit form"),
    '#description' => t("If checked, you will be redirected to the MODS edit form."),
    '#default_value' => FALSE,
  );

  $form['islandora_object_clone_source_pid'] = array(
    '#type' => 'value',
    '#value' => $object->id,
  );
  $form['islandora_object_clone_confirm_clone'] = array(
    '#type' => 'submit',
    '#value' => t('Clone object'),
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
  $source_pid = $form_state['values']['islandora_object_clone_source_pid'];
  if ($clone_pid = islandora_object_clone_clone_object($form_state)) {
    if ($form_state['values']['islandora_object_clone_redirect_to_metadata']) {
      drupal_goto('islandora/object/' . $clone_pid . '/datastream/MODS/edit');
    }
    else {
      drupal_set_message(t("Object @clone_pid cloned from @source_pid.", array('@clone_pid' => $clone_pid, '@source_pid' => $source_pid)));
      drupal_goto('islandora/object/' . $clone_pid);
    }
  }
}
