<?php

/**
 * @file
 * The admin form for the Islandora Object Clone module.
 */

/**
 * Defines the admin settings form.
 *
 * @param array $form
 *   The Drupal form definition.
 * @param array $form_state
 *   The Drupal form state.
 *
 * @return array
 *   The Drupal form definition.
 */
function islandora_object_clone_admin_form(array $form, array &$form_state) {
  module_load_include('inc', 'islandora_object_clone', 'includes/utilities');
  $get_default_value = function($name, $default) use(&$form_state) {
    return isset($form_state['values'][$name]) ? $form_state['values'][$name] : variable_get($name, $default);
  };

  $cmodels = islandora_object_clone_get_cmodels();
  $form['islandora_object_clone_allowed_cmodels'] = array(
    '#type' => 'checkboxes',
    '#options' => $cmodels,
    '#title' => t("Allowed content models"),
    '#default_value' => variable_get('islandora_object_clone_allowed_cmodels', array()),
    '#description' => t("The Clone tab will only show up for objects with the selected content models. Note that cloning objects that have sequence relationships to parent objects, such as pages and compound children, can have unpredictable effects."),
  );

  return system_settings_form($form);
}
